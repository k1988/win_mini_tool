#include <windows.h>
#include <stdio.h>
#include "psapi.h"

#pragma comment(lib,"psapi.lib")

void PrintMemoryInfo( DWORD processID )
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    // Print the process identifier.

    printf( "\nProcess ID: %u\n", processID );

    // Print information about the memory usage of the process.

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID );
    if (NULL == hProcess)
        return;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
        printf( "\tPageFaultCount: %d\n", pmc.PageFaultCount );
        printf( "\tPeakWorkingSetSize: %d\n", 
                  pmc.PeakWorkingSetSize / 1024 );
        printf( "\tWorkingSetSize: %d\n", pmc.WorkingSetSize  / 1024);
        printf( "\tQuotaPeakPagedPoolUsage: %d\n", 
                  pmc.QuotaPeakPagedPoolUsage  / 1024);
        printf( "\tQuotaPagedPoolUsage: %d\n", 
                  pmc.QuotaPagedPoolUsage  / 1024);
        printf( "\tQuotaPeakNonPagedPoolUsage: %d\n", 
                  pmc.QuotaPeakNonPagedPoolUsage  / 1024);
        printf( "\tQuotaNonPagedPoolUsage: %d\n", 
                  pmc.QuotaNonPagedPoolUsage  / 1024);
        printf( "\tPagefileUsage: %d\n", pmc.PagefileUsage  / 1024); 
        printf( "\tPeakPagefileUsage: %d\n", 
                  pmc.PeakPagefileUsage  / 1024);
    }

    CloseHandle( hProcess );
}

void main( )
{
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the memory usage for each process

    for ( i = 0; i < cProcesses; i++ )
        PrintMemoryInfo( aProcesses[i] );
}
