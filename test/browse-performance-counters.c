// https://docs.microsoft.com/en-us/windows/win32/perfctrs/browsing-performance-counters

#define UNICODE

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <pdh.h>
#include <pdhmsg.h>

// Performance Data Helper
#pragma comment(lib, "pdh.lib")

CONST ULONG SAMPLE_INTERVAL_MS    = 1000;
CONST PWSTR BROWSE_DIALOG_CAPTION = L"Select a counter to monitor.";

void wmain(void) {
	PDH_STATUS status;
	HQUERY query = NULL;
	HCOUNTER counter;
	PDH_FMT_COUNTERVALUE dv;
	DWORD ct;
	SYSTEMTIME stime;
	PDH_BROWSE_DLG_CONFIG dlg;
	WCHAR path[PDH_MAX_COUNTER_PATH];

	// Create a query.

	// https://docs.microsoft.com/en-us/windows/win32/api/pdh/nf-pdh-pdhopenqueryw
	status = PdhOpenQuery(NULL, 0, &query);

	if(status) {
		wprintf(L"\nPdhOpenQuery failed with status 0x%x.", status);

		goto fail;
	}

	// Initialize the browser dialog window settings.

	ZeroMemory(&path, sizeof(path));
	ZeroMemory(&dlg, sizeof(PDH_BROWSE_DLG_CONFIG));

	dlg.bIncludeInstanceIndex = FALSE;
	dlg.bSingleCounterPerAdd = TRUE;
	dlg.bSingleCounterPerDialog = TRUE;
	dlg.bLocalCountersOnly = FALSE;
	dlg.bWildCardInstances = TRUE;
	dlg.bHideDetailBox = TRUE;
	dlg.bInitializePath = FALSE;
	dlg.bDisableMachineSelection = FALSE;
	dlg.bIncludeCostlyObjects = FALSE;
	dlg.bShowObjectBrowser = FALSE;
	dlg.hWndOwner = NULL;
	dlg.szReturnPathBuffer = path;
	dlg.cchReturnPathLength = PDH_MAX_COUNTER_PATH;
	dlg.pCallBack = NULL;
	dlg.dwCallBackArg = 0;
	dlg.CallBackStatus = ERROR_SUCCESS;
	dlg.dwDefaultDetailLevel = PERF_DETAIL_WIZARD;
	dlg.szDialogBoxCaption = BROWSE_DIALOG_CAPTION;

	// Display the counter browser window. The dialog is configured
	// to return a single selection from the counter list.

	status = PdhBrowseCounters(&dlg);

	if(status) {
		if(status == PDH_DIALOG_CANCELLED) {
			wprintf(L"Dialog canceled by user.\n");
		}

		else {
			wprintf(L"PdhBrowseCounters failed with status 0x%x.\n", status);
		}

		goto fail;
	}

	else if(!wcslen(path)) {
		wprintf(L"User did not select any counter...\n");

		goto fail;
	}

	else {
		wprintf(L"Counter selected: %s\n", path);
	}

	// Add the selected counter to the query.

	status = PdhAddCounter(query, path, 0, &counter);

	if(status) {
		wprintf(L"PdhAddCounter failed with status 0x%x.\n", status);

		goto fail;
	}

	// Most counters require two sample values to display a formatted value.
	// PDH stores the current sample value and the previously collected
	// sample value. This call retrieves the first value that will be used
	// by PdhGetFormattedCounterValue in the first iteration of the loop
	// Note that this value is lost if the counter does not require two
	// values to compute a displayable value.

	status = PdhCollectQueryData(query);

	if(status) {
		wprintf(L"PdhCollectQueryData failed with 0x%x.\n", status);

		goto fail;
	}

	// Print counter values until a key is pressed.

	while(!_kbhit()) {
		Sleep(SAMPLE_INTERVAL_MS);

		GetLocalTime(&stime);

		status = PdhCollectQueryData(query);

		if(status) wprintf(L"PdhCollectQueryData failed with status 0x%x.\n", status);

		wprintf(L"\"%2.2d/%2.2d/%4.4d %2.2d:%2.2d:%2.2d.%3.3d\"",
			stime.wMonth,
			stime.wDay,
			stime.wYear,
			stime.wHour,
			stime.wMinute,
			stime.wSecond,
			stime.wMilliseconds
		);

		// Compute a displayable value for the counter.

		status = PdhGetFormattedCounterValue(
			counter,
			PDH_FMT_DOUBLE,
			&ct,
			&dv
		);

		if(status) {
			wprintf(L"PdhGetFormattedCounterValue failed with status 0x%x.", status);

			goto fail;
		}

		wprintf(L" => \"%.20g\"\n", dv.doubleValue);
	}

fail:
	// Close the query.

	if(query) PdhCloseQuery(query);
}
