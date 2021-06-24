#include <Arduino.h>
#include <LwIP.h>
#include <STM32Ethernet.h>

// Enter an IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:

IPAddress ip(172, 16, 0, 177);
IPAddress myDns(211, 137, 58, 20);
IPAddress gateway(172, 16, 0, 1);
IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer server(23);

EthernetClient clients[4];

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
  // start listening for clients
  server.begin();
  // Open serial communications and wait for port to open:

  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop()
{
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client)
  {

    bool newClient = true;
    for (byte j = 0; j < 4; j++)
    {
      //check whether this client refers to the same socket as one of the existing instances:
      if (clients[j] == client)
      {
        newClient = false;
        break;
      }
    }

    if (newClient)
    {
      //check which of the existing clients can be overridden:
      byte i = 0;
      for (; i < 4; i++)
      {
        if (!clients[i] && clients[i] != client)
        {
          clients[i] = client;
          // clear out the input buffer:
          client.flush();
          Serial.println("We have a new client");
          client.print("Hello, client number: ");
          client.print(i);
          client.println();
          break;
        }
      }
      if (i == 4)
      {
        Serial.println("clients poll is full, stop it");
        client.stop();
      }
    }

    if (client.available() > 0)
    {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to all other connected clients:
      for (byte i = 0; i < 4; i++)
      {
        if (clients[i] && (clients[i] == client))
        {
          clients[i].write(thisChar);
        }
      }
      // echo the bytes to the server as well:
      Serial.write(thisChar);
    }
  }
  for (byte i = 0; i < 4; i++)
  {
    if (clients[i] && (!clients[i].connected()))
    {
      // client.stop() invalidates the internal socket-descriptor, so next use of == will always return false;
      Serial.print(i);
      Serial.println(" disconnected");
      clients[i].stop();
    }
  }
}
