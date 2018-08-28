#include </usr/local/include/pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	pcap_if_t *alldevs;
	pcap_if_t *device;
	int inum =3;
	int i_open = 0;
	char errbuf_open[PCAP_ERRBUF_SIZE];
	if(pcap_findalldevs(&alldevs,errbuf_open)==-1)
	{
		fprintf(stderr, "Error in pcap_findalldevs:%s\n", errbuf_open);
		exit(EXIT_FAILURE);
	}
//	device = alldevs;
	for(device = alldevs,i_open = 0; i_open < inum-1; device = alldevs->next,i_open++)
	{
		printf("Device name:%s\n",device->name);
		printf("Description:%s\n",device->description);
	}

//	pcap_freealldevs(device);
	return 0;
}



