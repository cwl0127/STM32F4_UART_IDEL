#include "ExpandEthernetClient.h"

size_t EthernetExpandClient::readBytesUntilWith(char terminator, char *buffer, size_t length)
{
    if (length < 1)
		{
			return 0;
		}
		size_t index = 0;
		while (index < length)
		{
			int c = timedRead();
			if (c < 0)
			{
				break;
			}
			else if (c == terminator)
			{
				*buffer = (char)c;
                index++;
				break;
			}
			else
			{
				*buffer++ = (char)c;
				index++;
			}
		}
		return index; // return number of characters, including null terminator
}