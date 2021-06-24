#ifndef __EXPANDETHERNETCLIENT_H
#define __EXPANDETHERNETCLIENT_H

#include <EthernetClient.h>

class EthernetExpandClient : public EthernetClient
{
public:
	EthernetExpandClient() : EthernetClient() {}
	EthernetExpandClient(uint8_t s) : EthernetClient(s) {}
	EthernetExpandClient(EthernetClient &client) : EthernetClient(client.getSocketNumber()) {}
	size_t readBytesUntilWith(char terminator, char *buffer, size_t length);
};

#endif