// https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getipstatistics
// https://docs.microsoft.com/en-us/windows/win32/api/ipmib/ns-ipmib-mib_ipstats_lh

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0,(x))
#define FREE(x) HeapFree(GetProcessHeap(), 0,(x))

int main(int argc, char** argv) {
	MIB_IPSTATS* pStats = NULL;

	pStats = (MIB_IPSTATS*)(MALLOC(sizeof(MIB_IPSTATS)));

	if(!pStats) {
		printf("Unable to allocate memory for MIB_IPSTATS\n");

		return 1;
	}

	if(GetIpStatistics(pStats)) {
		printf("GetIpStatistics call failed\n");

		return 2;
	}

	else {
		printf("Forwarding: ");

		switch(pStats->dwForwarding) {
		case MIB_IP_FORWARDING:
			printf("Enabled\n");
			break;

		case MIB_IP_NOT_FORWARDING:
			printf("Disabled\n");
			break;

		default:
			printf("\n");
			break;
		}

		printf("DeafaultTTL: %u\n", pStats->dwDefaultTTL);
		printf("InRecieves: %u\n", pStats->dwInReceives);
		printf("InHdrErrors: %u\n", pStats->dwInHdrErrors);
		printf("InAddrErrors: %u\n", pStats->dwInAddrErrors);
		printf("ForwDatagrams: %ld\n", pStats->dwForwDatagrams);
		printf("InUnknownProtos: %u\n", pStats->dwInUnknownProtos);
		printf("InDiscards: %u\n", pStats->dwInDiscards);
		printf("InDelivers: %u\n", pStats->dwInDelivers);
		printf("OutRequests: %u\n", pStats->dwOutRequests);
		printf("RoutingDiscards: %u\n", pStats->dwRoutingDiscards);
		printf("OutDiscards: %u\n", pStats->dwOutDiscards);
		printf("OutNoRoutes: %u\n", pStats->dwOutNoRoutes);
		printf("ReasmTimeout: %u\n", pStats->dwReasmTimeout);
		printf("ReasmReqds: %u\n", pStats->dwReasmReqds);
		printf("ReasmOks: %u\n", pStats->dwReasmOks);
		printf("ReasmFails: %u\n", pStats->dwReasmFails);
		printf("FragOks: %u\n", pStats->dwFragOks);
		printf("FragFails: %u\n", pStats->dwFragFails);
		printf("FragCreates: %u\n", pStats->dwFragCreates);
		printf("NumIf: %u\n", pStats->dwNumIf);
		printf("NumAddr: %u\n", pStats->dwNumAddr);
		printf("NumRoutes: %u\n", pStats->dwNumRoutes);
	}

	if(pStats) FREE(pStats);

	return 0;
}
