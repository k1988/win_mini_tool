/*
@author zhaohaiyang_1988@163.com
@detail 此程序的作用是用来检查部分用户机器上出现的情况，即winsock2可用，但winInet不可用的情况
*/
#include <winsock2.h>
#include <wininet.h>
#include <iostream>
#include <string>

#pragma  comment(lib, "ws2_32.lib")
#pragma  comment(lib, "wininet.lib")

bool TestWinSock(std::string ip, short int port)
{
WORD wVersionRequested;
	WSADATA wsaData;
	int result;

	wVersionRequested = MAKEWORD(2, 2);
	result = WSAStartup(wVersionRequested, &wsaData);
	if (result)
	{
        std::cout << "WSAStartup error :" << ::GetLastError() << std::endl;
		return false;
	}

    SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
     addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);
    addr.sin_addr.S_un.S_addr = ::inet_addr(ip.c_str());
    if (SOCKET_ERROR == ::connect(socket, (sockaddr*)&addr, sizeof(addr)))
    {
          std::cout << "connect error :" << ::GetLastError() << std::endl;
		return false;
    }

	return true;
}

bool TestWinSockListen(std::string ip, short int port)
{
WORD wVersionRequested;
	WSADATA wsaData;
	int result;

	wVersionRequested = MAKEWORD(2, 2);
	result = WSAStartup(wVersionRequested, &wsaData);
	if (result)
	{
        std::cout << "WSAStartup error :" << ::GetLastError() << std::endl;
		return false;
	}

    SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
     addr.sin_family = AF_INET;
    addr.sin_port = ::htons(port);
    addr.sin_addr.S_un.S_addr = ::inet_addr(ip.c_str());
    if (SOCKET_ERROR == bind(socket, (struct sockaddr *) &addr, sizeof(addr))) 
    {
        std::cout << "\nError occurred while binding." << std::endl;
        return false;
    }
    
    if (SOCKET_ERROR == listen(socket, 0x7fffffff))
    {
        closesocket(socket);
        socket = NULL;
        std::cout <<  ("\nError occurred while listening.") << std::endl;
        return false;
    }
	return true;
}

bool TestWininet(std::string url){
    HINTERNET hNet = InternetOpenA(NULL,
                                   INTERNET_OPEN_TYPE_PRECONFIG,
                                   NULL,
                                   NULL,
                                   0);
    if (hNet == NULL)
    {
        std::cout << "InternetOpenA error :" << ::GetLastError() << std::endl;
        return false;
    }

	// 默认TIMEOUT时间2秒
    static DWORD dwTimeout = 2000;
	static DWORD dwRetries = 3;
    InternetSetOptionA(hNet, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimeout, sizeof(DWORD));
    InternetSetOptionA(hNet, INTERNET_OPTION_CONNECT_RETRIES, &dwRetries, sizeof(DWORD));

    HINTERNET hUrlFile = InternetOpenUrlA(hNet, 
                                          url.c_str(), 
                                          NULL,
                                          0,
                                          INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD,
                                          0);
    if (hUrlFile == NULL)
    {        
        std::cout << url << "InternetOpenUrlA error :" << ::GetLastError() << std::endl;
 		InternetCloseHandle(hNet);
        return false;
    }
    
 	InternetCloseHandle(hUrlFile);
    InternetCloseHandle(hNet) ;
    return true;
}

int main(int argc, char** argv)
{
    std::string ip;
    unsigned short port = 80;
    if (argc == 1) {
        std::cout << "usage: nettest <ip> <port:default = 80>" << std::endl;        
    } else{
        ip = argv[1];
        if (argc  > 2) {
            port = atoi(argv[2]);
        }
    }

    if (TestWinSock(ip, port)){
        std::cout << "winsock2 connect successed!" << std::endl;
    }
    
    if (TestWinSockListen(ip, port)){
        std::cout << "TestWinSockListen connect successed!" << std::endl;
    }

    char url[1024]={0};
    sprintf(url, "http://%s:%d", ip.c_str(), port);
    if (TestWininet(url)){
        std::cout << "Wininet connect successed!" << std::endl;
    }
}