#include <iostream>
#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

void usage()
{
     std::cout << "usage: IpUtils Opt ip"
        << "\n opt:"
        << "\n\t -i integer ip: exp 1361305146"
        << "\n\t -s string ip: exp 127.0.0.1"
        << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        usage();
        return 0;
    }


    if (stricmp(argv[1], "-i") == 0)
    {
        std::cout << argv[1] << std::endl;
        int ip = atoi(argv[2]);
        in_addr addr;
        addr.S_un.S_addr = ip;
        std::cout << "inet_ntoa(" << ip << ") => " << inet_ntoa(addr) << std::endl;
    }
    else if (stricmp(argv[1], "-s") == 0)
    {
        std::cout << argv[1] << std::endl;
        char* ip = argv[2];
        std::cout << "inet_addr(" << ip << ") => " << inet_addr(ip) << std::endl;
    }
    else
    {
        usage();
        return -1;
    }
}