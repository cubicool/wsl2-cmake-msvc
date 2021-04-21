// https://docs.microsoft.com/en-us/windows/win32/perfctrs/performance-counters-functions

// Performance Data Helper
#pragma comment(lib, "pdh.lib")

#define UNICODE

#include <windows.h>
#include <inttypes.h>
#include <stdio.h>
#include <conio.h>
#include <pdh.h>
#include <pdhmsg.h>

// https://docs.microsoft.com/en-us/windows/win32/perfctrs/pdh-error-codes
void pdherr(LPWSTR func, DWORD err) {
	LPWSTR msg = NULL;
	HANDLE lib = NULL;

	// TODO: The library handle should be static.
	if(!(lib = LoadLibrary(L"pdh.dll"))) {
		wprintf(L"LoadLibrary failed with %lu\n", GetLastError());

		return;
	}

	// https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-formatmessagew
	if(!FormatMessage(
		FORMAT_MESSAGE_FROM_HMODULE |
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		lib,
		err,
		0,
		(LPWSTR)(&msg),
		0,
		NULL
	)) {
		wprintf(L"Format message failed with 0x%x\n", GetLastError());

		return;
	}

	wprintf(L"%ls failed(0x%x): %ls", func, err, msg);

	LocalFree(msg);
}

int main(int argc, char** argv) {
	PDH_STATUS s;
	HQUERY q = NULL;
	HCOUNTER b_sent;
	HCOUNTER b_recv;
	PDH_FMT_COUNTERVALUE dv;
	DWORD ct;

	// https://docs.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhopenqueryw
	if((s = PdhOpenQuery(NULL, 0, &q))) {
		pdherr(L"PdhOpenQuery", s);

		goto fail;
	}

	// https://docs.microsoft.com/en-us/windows/win32/perfctrs/specifying-a-counter-path
	// https://docs.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhaddcounterw
	// https://docs.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhgetcounterinfow
	if((s = PdhAddCounter(q, L"\\Network Interface(*)\\Bytes Sent/sec", 0, &b_sent))) {
		pdherr(L"PdhAddCounter", s);

		goto fail;
	}

	if((s = PdhAddCounter(q, L"\\Network Interface(*)\\Bytes Received/sec", 0, &b_recv))) {
		pdherr(L"PdhAddCounter", s);

		goto fail;
	}

	// We need to call this once to "initialize" the counter, in a manner of speaking. More
	// specifically, the API needs two values to begin using the "formatted" functions.
	//
	// https://docs.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhcollectquerydata
	if((s = PdhCollectQueryData(q))) {
		pdherr(L"PdhCollectQueryData", s);

		goto fail;
	}

	while(!_kbhit()) {
		Sleep(5000);

		if((s = PdhCollectQueryData(q))) {
			pdherr(L"PdhCollectQueryData", s);

			continue;
		}

		// https://docs.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhgetformattedcountervalue
		if((s = PdhGetFormattedCounterValue(b_sent, PDH_FMT_LARGE, &ct, &dv))) {
			pdherr(L"PdhGetFormattedCounterValue", s);
		}

		else wprintf(L"Sent: %" PRIu64 "\n", dv.largeValue);

		if((s = PdhGetFormattedCounterValue(b_recv, PDH_FMT_LARGE, &ct, &dv))) {
			pdherr(L"PdhGetFormattedCounterValue", s);
		}

		else wprintf(L"Recv: %" PRIu64 "\n", dv.largeValue);
	}

	return 0;

fail:
	// https://docs.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhclosequery
	if(q) PdhCloseQuery(q);

	return 1;
}
