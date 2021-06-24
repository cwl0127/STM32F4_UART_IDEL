#include <Arduino.h>
#include <LwIP.h>
#include <STM32Ethernet.h>

#define MAX_REMOTE_CLIENT_CNT 8U
// Enter an IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:

struct ExClient
{
public:
  ExClient() : valid(0) {}
  EthernetClient client;
  uint8_t valid;
};

IPAddress ip(172, 16, 0, 178);
IPAddress myDns(211, 137, 58, 20);
IPAddress gateway(172, 16, 0, 1);
IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer server(8080);

ExClient clientsPool[MAX_REMOTE_CLIENT_CNT];

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Advanced Chat Server test");
  // initialize the Ethernet device
  Ethernet.begin(ip, subnet, gateway, myDns);
  while (Ethernet.linkStatus() != LinkON)
	{
		Serial.println("Ethernet link OFF");
		delay(500);
	}
	Serial.println("Ethernet link ON");
  // start listening for clients
  server.begin();
  // Open serial communications and wait for port to open:

  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

bool newClient = true;

void loop()
{
  newClient = true;
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client)
  {
    Serial.println("There is new a client");
    for (byte j = 0; j < MAX_REMOTE_CLIENT_CNT; j++)
    {
      //check whether this client refers to the same socket as one of the existing instances:
      if (clientsPool[j].valid == 1 && clientsPool[j].client == client)
      {
        Serial.printf("Exist client, remote port: %u\r\n", client.remotePort());
        newClient = false;
        break;
      }
    }
  }
  else
  {
    newClient = false;
  }

  if (newClient)
  {
    //check which of the existing clients can be overridden:
    byte i = 0;
    for (; i < MAX_REMOTE_CLIENT_CNT; i++)
    {
      if (clientsPool[i].valid == 0)  // && clients[i] != client
      {
        clientsPool[i].client = client;
        clientsPool[i].valid = 1;
        // clear out the input buffer:
        client.flush();
        Serial.println("We have a new client");
        client.print("Hello, client number: ");
        client.print(i);
        client.println();
        break;
      }
    }
    if (i == MAX_REMOTE_CLIENT_CNT)
    {
      Serial.println("clients pool is full, stop it");
      client.stop();
    }
  }
  for (byte i = 0; i < MAX_REMOTE_CLIENT_CNT; i++)
  {
    if ((clientsPool[i].valid == 1) && (!clientsPool[i].client.connected()))
    {
      // client.stop() invalidates the internal socket-descriptor, so next use of == will always return false;
      Serial.print(i);
      Serial.println(" disconnected");
      clientsPool[i].client.stop();
      clientsPool[i].valid = 0;
    }
    if (clientsPool[i].valid == 1)
    {
      String rcvStr;
      // read the bytes incoming from the client:
      while (clientsPool[i].client.available() > 0)
      {
        rcvStr += (char)clientsPool[i].client.read();
      }
      if (rcvStr.length() > 0)
        clientsPool[i].client.print(rcvStr);
    }
  }
}
