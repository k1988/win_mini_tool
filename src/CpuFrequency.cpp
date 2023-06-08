#include <iostream>
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strsafe.h>
#include <process.h>
#include <powerbase.h>

#pragma comment(lib, "PowrProf.lib")

float ProcSpeedCalc()
{
#define RdTSC __asm _emit 0x0f __asm _emit 0x31

    // variables for the clock-cycles:
    __int64 cyclesStart = 0, cyclesStop = 0;
    // variables for the High-Res Preformance Counter:
    unsigned __int64 nCtr = 0, nFreq = 0, nCtrStop = 0;

    // retrieve performance-counter frequency per second:
    if (!QueryPerformanceFrequency((LARGE_INTEGER *)&nFreq))
        return 0;

    // retrieve the current value of the performance counter:
    QueryPerformanceCounter((LARGE_INTEGER *)&nCtrStop);

    // add the frequency to the counter-value:
    nCtrStop += nFreq;

    _asm
    { // retrieve the clock-cycles for the start value:
        RdTSC
        mov DWORD PTR cyclesStart, eax
        mov DWORD PTR [cyclesStart + 4], edx
    }

    do
    {
        // retrieve the value of the performance counter
        // until 1 sec has gone by:
        QueryPerformanceCounter((LARGE_INTEGER *)&nCtr);
    }
    while (nCtr < nCtrStop);

    _asm
    { // retrieve again the clock-cycles after 1 sec. has gone by:
        RdTSC
        mov DWORD PTR cyclesStop, eax
        mov DWORD PTR [cyclesStop + 4], edx
    }

    // stop-start is speed in Hz divided by 1,000,000 is speed in MHz
    return ((float)cyclesStop - (float)cyclesStart) / 1000000;
}

typedef struct _PROCESSOR_POWER_INFORMATION
{
    ULONG Number;
    ULONG MaxMhz;
    ULONG CurrentMhz;
    ULONG MhzLimit;
    ULONG MaxIdleState;
    ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, *PPROCESSOR_POWER_INFORMATION;

// from :
float psutil_cpu_freq()
{
    PROCESSOR_POWER_INFORMATION *ppi;
    NTSTATUS ret;
    size_t size;
    LPBYTE pBuffer = NULL;
    ULONG current;
    ULONG max;
    unsigned int num_cpus;
    SYSTEM_INFO system_info;
    system_info.dwNumberOfProcessors = 0;

    // Get the number of CPUs.
    GetSystemInfo(&system_info);
    if (system_info.dwNumberOfProcessors == 0)
        num_cpus = 1;
    else
        num_cpus = system_info.dwNumberOfProcessors;

    // Allocate size.
    size = num_cpus * sizeof(PROCESSOR_POWER_INFORMATION);
    pBuffer = (BYTE *)LocalAlloc(LPTR, size);
    if (!pBuffer)
    {
        return NULL;
    }

    // Syscall.
    ret = CallNtPowerInformation(
        ProcessorInformation, NULL, 0, pBuffer, size);
    if (ret != 0)
    {
        printf("CallNtPowerInformation syscall failed");
        goto error;
    }

    // Results.
    ppi = (PROCESSOR_POWER_INFORMATION *)pBuffer;
    max = ppi->MaxMhz;
    current = ppi->CurrentMhz;
    LocalFree(pBuffer);

    return printf("cpu info max:%d current:%d", current, max);

error:
    if (pBuffer != NULL)
        LocalFree(pBuffer);
    return NULL;
}

void main()
{
    printf("CPU frequency = %f\n", ProcSpeedCalc());
    float f_rdtsc = psutil_cpu_freq();
    // printf("processor frequency is %.4f", f_rdtsc);
}