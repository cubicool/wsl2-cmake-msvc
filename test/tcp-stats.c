#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0,(x))
#define FREE(x) HeapFree(GetProcessHeap(), 0,(x))

int main(int argc, char** argv) {
	PMIB_TCPSTATS stats;
	DWORD r = 0;

	stats = (MIB_TCPSTATS*)(MALLOC(sizeof(MIB_TCPSTATS)));

	if(!stats) {
		printf("Error allocating memory\n");

		return 1;
	}

	if(!(r = GetTcpStatistics(stats))) {
		printf("ActiveOpens: %ld\n", stats->dwActiveOpens);
		printf("PassiveOpens: %ld\n", stats->dwPassiveOpens);
		printf("InSegs: %ld\n", stats->dwInSegs);
		printf("OutSegs: %ld\n", stats->dwOutSegs);
		printf("NumConns: %ld\n", stats->dwNumConns);
	}

	else {
		printf("GetTcpStatistics failed with error: %ld\n", r);

		LPVOID msg;

		if(FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			r,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)(&msg),
			0,
			NULL
		)) {
			printf("Error: %s", (const char*)(msg));
		}

		LocalFree(msg);
	}

	if(stats) FREE(stats);
}
