#define _UNICODE 1
#define UNICODE 1
#include <Windows.h>
#include <WinDNS.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "dnsapi")

unsigned long winet_addr (WCHAR *ipv4str)
{
    unsigned long addr = 0L;
    WCHAR *p = NULL;
    WCHAR tok = L'.';
    int i = 0;
    p = wcstok(ipv4str, &tok);
    while (NULL != p)
    {
        addr += (_wtoi(p)) << (8*i);
        p = wcstok(NULL, &tok);
        i++;
    }
    p = NULL;
    return(addr);
}

// compile: cl /DUNICODE DnsLook.cpp && DnsLook www.baidu.com
int _tmain(int argc, TCHAR *argv[])
{
    if (argc < 2)
    {
        printf("Usage: DnsLook domain [dns server ip]. \nexp: DnsLook www.baidu.com 8.8.8.8");
        return 0;
    }

    WCHAR dnsServer[64] = L"8.8.8.8";
    if (argc > 2)
    {
        wcscpy_s(dnsServer, 64, argv[2]);
    }

    PDNS_RECORDW pdns_record = NULL;
    DNS_STATUS dns_status;
    IP4_ARRAY dns_server = {
        1,
        //33817604 // 4.4.4.2,
        //134744072 // 8.8.8.8
        winet_addr((WCHAR*)dnsServer)
    };
    PIP4_ARRAY pDnsServer = NULL;
    if (argc > 2)
    {
        pDnsServer = &dns_server;
    }

    while (true)
    {
        dns_status = DnsQuery_W(argv[1], DNS_TYPE_A, DNS_QUERY_STANDARD /*DNS_QUERY_BYPASS_CACHE | DNS_QUERY_NO_HOSTS_FILE*/, pDnsServer, (PDNS_RECORD *)&pdns_record, NULL);

        if (dns_status != 0)
        {
            printf("DnsQuery failed:%d\n", dns_status);
            continue;
        }

        PDNS_RECORDW pdns_iter = pdns_record;
        unsigned int idx = 0;

        while (pdns_iter)
        {
            in_addr iaddr = {0};
            char *paddr = NULL;
            iaddr.S_un.S_addr = pdns_iter->Data.A.IpAddress;
            paddr = inet_ntoa(iaddr);
            printf("domain = %ws \n", pdns_iter->pName);
            printf("%d: ", ++idx);
            printf("ip = %s \t", paddr);
            printf("data length = %d \t", pdns_iter->wDataLength);
            printf("TTL = %d \t", pdns_iter->dwTtl);

            pdns_iter = pdns_iter->pNext;
        }

        DnsRecordListFree(pdns_record, DnsFreeRecordList);
        ::Sleep(1000 * 20);
    }

    system("pause");
    return 0;
}