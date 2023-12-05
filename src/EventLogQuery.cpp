#include <windows.h>
#include <sddl.h>
#include <stdio.h>
#include <winevt.h>

#pragma comment(lib, "wevtapi.lib")

#define ARRAY_SIZE 10
#define TIMEOUT 1000  // 1 second; Set and use in place of INFINITE in EvtNext call

// The structured XML query.
#define QUERY \
    L"<QueryList>" \
    L"  <Query Path='Application'>" \
    L"    <Select>Event/Application[EventID=4]</Select>" \
    L"  </Query>" \
    L"</QueryList>"

DWORD PrintQueryStatuses(EVT_HANDLE hResults);
DWORD GetQueryStatusProperty(EVT_QUERY_PROPERTY_ID Id, EVT_HANDLE hResults, PEVT_VARIANT& pProperty);
DWORD PrintResults(EVT_HANDLE hResults);
DWORD PrintEvent(EVT_HANDLE hEvent);  // Shown in the Rendering Events topic

void main(void)
{
    DWORD status = ERROR_SUCCESS;
    EVT_HANDLE hResults = NULL;

    hResults = EvtQuery(NULL, NULL, QUERY, EvtQueryChannelPath | EvtQueryTolerateQueryErrors);
    if (NULL == hResults)
    {
        // Handle error.
        goto cleanup;
    }

    // Print the status of each query. If all the queries succeeded,
    // print the events in the result set. The status can be
    // ERROR_EVT_CHANNEL_NOT_FOUND or ERROR_EVT_INVALID_QUERY among others.
    if (ERROR_SUCCESS == PrintQueryStatuses(hResults))
        PrintResults(hResults);

cleanup:

    if (hResults)
        EvtClose(hResults);

}

// Get the list of paths in the query and the status for each path. Return
// the sum of the statuses, so the caller can decide whether to enumerate 
// the results.
DWORD PrintQueryStatuses(EVT_HANDLE hResults)
{
    DWORD status = ERROR_SUCCESS;
    PEVT_VARIANT pPaths = NULL;
    PEVT_VARIANT pStatuses = NULL;

    wprintf(L"List of channels/logs that were queried and their status\n\n");

    if (status = GetQueryStatusProperty(EvtQueryNames, hResults, pPaths))
        goto cleanup;

    if (status = GetQueryStatusProperty(EvtQueryStatuses, hResults, pStatuses))
        goto cleanup;

    for (DWORD i = 0; i < pPaths->Count; i++)
    {
        wprintf(L"%s (%lu)\n", pPaths->StringArr[i], pStatuses->UInt32Arr[i]);
        status += pStatuses->UInt32Arr[i];
    }

cleanup:

    if (pPaths)
        free(pPaths);

    if (pStatuses)
        free(pStatuses);

    return status;
}


// Get the list of paths specified in the query or the list of status values 
// for each path.
DWORD GetQueryStatusProperty(EVT_QUERY_PROPERTY_ID Id, EVT_HANDLE hResults, PEVT_VARIANT& pProperty)
{
    DWORD status = ERROR_SUCCESS;
    DWORD dwBufferSize = 0;
    DWORD dwBufferUsed = 0;

    if  (!EvtGetQueryInfo(hResults, Id, dwBufferSize, pProperty, &dwBufferUsed))
    {
        status = GetLastError();
        if (ERROR_INSUFFICIENT_BUFFER == status)
        {
            dwBufferSize = dwBufferUsed;
            pProperty = (PEVT_VARIANT)malloc(dwBufferSize);
            if (pProperty)
            {
                EvtGetQueryInfo(hResults, Id, dwBufferSize, pProperty, &dwBufferUsed);
            }
            else
            {
                wprintf(L"realloc failed\n");
                status = ERROR_OUTOFMEMORY;
                goto cleanup;
            }
        }

        if (ERROR_SUCCESS != (status = GetLastError()))
        {
            wprintf(L"EvtGetQueryInfo failed with %d\n", GetLastError());
            goto cleanup;
        }
    }

cleanup:

    return status;
}


// Enumerate all the events in the result set. 
DWORD PrintResults(EVT_HANDLE hResults)
{
    DWORD status = ERROR_SUCCESS;
    EVT_HANDLE hEvents[ARRAY_SIZE];
    DWORD dwReturned = 0;

    while (true)
    {
        // Get a block of events from the result set.
        if (!EvtNext(hResults, ARRAY_SIZE, hEvents, INFINITE, 0, &dwReturned))
        {
            if (ERROR_NO_MORE_ITEMS != (status = GetLastError()))
            {
                wprintf(L"EvtNext failed with %lu\n", status);
            }

            goto cleanup;
        }

        // For each event, call the PrintEvent function which renders the
        // event for display. PrintEvent is shown in RenderingEvents.
        for (DWORD i = 0; i < dwReturned; i++)
        {
            if (ERROR_SUCCESS == (status = PrintEvent(hEvents[i])))
            {
                EvtClose(hEvents[i]);
                hEvents[i] = NULL;
            }
            else
            {
                goto cleanup;
            }
        }
    }

cleanup:

    for (DWORD i = 0; i < dwReturned; i++)
    {
        if (NULL != hEvents[i])
            EvtClose(hEvents[i]);
    }

    return status;
}


DWORD PrintEvent(EVT_HANDLE hEvent)
{
	return 0;
}