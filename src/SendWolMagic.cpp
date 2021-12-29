/*
   Copyright 2011-2012 Katsuya Iida
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")

HRESULT ParseMac(LPCTSTR lpsz, BYTE *pmac)
{
	HRESULT hr = S_OK;
	const TCHAR *pch = lpsz;
	BYTE *pb = pmac;
	TCHAR ch;

	for (int i = 0; i < 6; i++)
	{
		BYTE k = 0;
		for (int j = 0; j < 2; j++)
		{
			ch = *pch++;
			k *= 16;
			if (ch >= '0' && ch <= '9')
			{
				k += ch - '0';
			}
			else if (ch >= 'a' && ch <= 'f')
			{
				k += ch - 'a' + 10;
			}
			else if (ch >= 'A' && ch <= 'F')
			{
				k += ch - 'A' + 10;
			}
			else
			{
				hr = E_FAIL;
				break;
			}
		}
		if (SUCCEEDED(hr))
		{
			*pb++ = k;
			if (i < 5)
			{
				ch = *pch;
				if (ch == ':' || ch == '-')
				{
					pch++;
				}
				else if (ch != '\0')
				{
					hr = E_FAIL;
					break;
				}
			}
		}
	}
	if (SUCCEEDED(hr))
	{
		ch = *pch;
		if (ch != '\0')
		{
			hr = E_FAIL;
		}
	}

	return hr;
}


HRESULT SendMagic(BYTE *pmac, int interval, int count)
{
	HRESULT hr = S_OK;
	BYTE buf[6 * 17];
	SOCKET sock;

	memset(buf, 0xff, 6);
	for (int i = 0; i < 16; i++)
	{
		memcpy(buf + 6 * (i + 1), pmac, 6);
	}
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		hr = E_FAIL;
	}
	else
	{
		struct sockaddr_in addr;
		int yes = 1;

		memset(&addr, 0, sizeof addr);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(9);
		addr.sin_addr.s_addr = inet_addr("255.255.255.255");

		if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof yes) != 0)
		{
			hr = E_FAIL;
		}
		else
		{
			for (int i = 0; i < count; i++)
			{
				printf(_T("Sending magic packet...\n"));
				int len = ARRAYSIZE(buf);
				if (sendto(sock, (char *)buf, len, 0, (struct sockaddr *)&addr, sizeof addr) != len)
				{
					hr = E_FAIL;
					break;
				}
				else
				{
					if (i < count - 1)
					{
						Sleep(interval);
					}
					else
					{
						break;
					}
				}
			}
		}

		closesocket(sock);
	}

	return hr;
}

HRESULT ParseInt(LPCTSTR sz, INT *piResult)
{
	INT i = 0;
	WCHAR ch;
	while (*sz == '\0')
		return E_FAIL;
	while ((ch = *sz++) != '\0')
	{
		if (ch >= '0' && ch <= '9')
		{
			i = i * 10 + ch - '0';
		}
		else
		{
			return E_FAIL;
		}
	}
	*piResult = i;
	return S_OK;
}

void Usage()
{
	LPCTSTR szMsg = _T("Send Wakeup-On-LAN Magic Packet to the specified MAC address.\n")
		_T("\n")
		_T("SENDMAGIC macaddr\n")
		_T("\n");
	printf(szMsg);
}

int _tmain(int argc, TCHAR *argv[])
{
	HRESULT hr;
	WSADATA wsaData;
	BYTE mac[6];
	BOOL fHasMac = FALSE;
	BOOL fUsage = FALSE;
	BOOL fInvalidOptions = FALSE;
	int interval = 3000;
	int count = 5;
	int ret = 0;

	while (++argv,--argc)
	{
		if ((*argv)[0] == '/') // option
		{
			LPCTSTR lpsz = *argv + 1;
			if (_tcscmp(lpsz, _T("?")) == 0 || _tcscmp(lpsz, _T("help")) == 0)
			{
				fUsage = TRUE;
			}
			else if (_tcsncmp(lpsz, _T("interval:"), 9) == 0)
			{
				INT result;
				hr = ParseInt(lpsz + 6, &result);
				if (FAILED(hr))
				{
					fInvalidOptions = TRUE;
				}
				else
				{
					interval = result;
				}
			}
			else if (_tcsncmp(lpsz, _T("count:"), 6) == 0)
			{
				INT result;
				hr = ParseInt(lpsz + 6, &result);
				if (FAILED(hr))
				{
					fInvalidOptions = TRUE;
				}
				else
				{
					count = result;
				}
			}
			else
			{
				fInvalidOptions = TRUE;
			}
		}
		else
		{
			hr = ParseMac(*argv, mac);
			if (FAILED(hr))
			{
				fInvalidOptions = TRUE;
				hr = S_OK;
			}
			else
			{
				fHasMac = TRUE;
			}
		}
	}
	if (fUsage || !fHasMac)
	{
		Usage();
		ret = 2;
	}
	else if (fInvalidOptions)
	{
		printf(_T("Error: Invalid options.\n\n")); 
		Usage();
		ret = 3;
	}
	else
	{
		if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
		{
			hr = E_FAIL;
		}
		else
		{
			if (SUCCEEDED(hr))
			{
				hr = SendMagic(mac, interval, count);
			}

			WSACleanup();
		}

		if (FAILED(hr))
		{
			printf(_T("Failed"));
			ret = 1;
		}
	}

	return ret;
}