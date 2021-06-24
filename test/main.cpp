#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <bsp_sdram.h>

#define W5500_RESET_PIN PD3
#define W5500_CS PA15
#pragma pack(4)
uint16_t testValue[2048] __attribute__((section(".eram")));

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
static uint8_t mac[6];
IPAddress ip(172, 16, 0, 177);

// Enter the IP address of the server you're connecting to:
IPAddress server(172, 16, 0, 116);

EthernetClient client(0);

bool connected = false;

static uint8_t *MACAddressDefault(void)
{
	uint32_t baseUID = *(uint32_t *)UID_BASE;
	mac[0] = 0x00;
	mac[1] = 0x80;
	mac[2] = (baseUID & 0xFF000000) >> 24; //0xE1;
	mac[3] = (baseUID & 0x00FF0000) >> 16;
	mac[4] = (baseUID & 0x0000FF00) >> 8;
	mac[5] = (baseUID & 0x000000FF);

	return mac;
}

void setup()
{
  SDRAM_Init();
  pinMode(W5500_RESET_PIN, OUTPUT);
  digitalWrite(W5500_RESET_PIN, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  MACAddressDefault();
  Serial.begin(115200);
  Serial.printf("testValue address: %p\r\n", testValue);
  for (uint32_t i = 0; i < 2048; ++i)
  {
    testValue[i] = 0;
  }
  Serial.println("W5500 TEST");
  //reset W5500
  digitalWrite(W5500_RESET_PIN, LOW);
  delay(50);
  digitalWrite(W5500_RESET_PIN, HIGH);
  Ethernet.init(W5500_CS);
  Ethernet.begin(mac);
  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true)
    {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  while (Ethernet.linkStatus() == LinkOFF)
  {
    Serial.println("Ethernet cable is not connected.");
    delay(500);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);

  Serial.println("connecting...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 1500))
  {
    Serial.println("connected");
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (client.connected()) {
      client.print(inChar);
    }
  }

  // if the server's disconnected, stop the client:
  // static uint32_t last = 0;
  // if ((millis() - last) >= 500)
  // {
  //   Serial.print("status: ");
  //   Serial.println(client.status());
  //   last = millis();
  // }
  if (!client.connected())
  {
    if (connected)
    {
        Serial.println("Cient disconnected.");
        connected = false;
    }
  }
  else
  {
    if (!connected)
    {
      Serial.println("Cient connected.");
      connected = true;
    }
  }
  // if (!client.connected()) {
  //   Serial.println();
  //   Serial.println("disconnecting.");
  //   client.stop();
  //   // do nothing:
  //   while (true) {
  //     delay(1);
  //   }
  // }
}