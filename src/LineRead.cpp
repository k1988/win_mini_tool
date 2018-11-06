#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <consoleapi.h>

void usage() {
	std::cout << "usage: " << std::endl
		<< "\tLineRead <txtpath>" << std::endl;
}

#define ONCE_LINE_COUNT  2

//------------------------------------------------------------------------------
// Clears the screen
//------------------------------------------------------------------------------
void clrscr()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	GetConsoleScreenBufferInfo(hConsole, &csbi);
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);	
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	SetConsoleCursorPosition(hConsole, coordScreen);
}

void clrline()
{
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	coordScreen = csbi.dwCursorPosition;
	coordScreen.Y -= ONCE_LINE_COUNT;
	coordScreen.X = 0;	
	FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
	GetConsoleScreenBufferInfo(hConsole, &csbi);	
	FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
	
	SetConsoleCursorPosition(hConsole, coordScreen);
}

bool g_hasEvent = false;
BOOL CtrlHandler( DWORD fdwCtrlType ) 
{ 
  switch( fdwCtrlType ) 
  { 
    // Handle the CTRL-C signal. 
    case CTRL_C_EVENT: 
	  g_hasEvent = true;
      return( TRUE );
	default:
		clrscr();
	  //g_hasEvent = true;
	  return FALSE;
  }
}

int main(int argc, char* argv[])
{
	if (argc < 2) {
		usage();
	}

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE );
	char* path = argv[1];
	std::ifstream inf(path);
	if (!inf.is_open()) std::cout << "open file failed!" << std::endl;

	std::string buffer;
	long n = 0;
	char c = 0;
	printf("%d", n);	
	while (std::getline(inf, buffer)) {
		if (buffer.empty()) continue;

		do
		{
			clrline();
			for (int i = 0; i < ONCE_LINE_COUNT; i++)
			{
				std::cout << buffer.substr(0,80) << std::endl;
				buffer.erase(0, 80);
			}
::Sleep(500);
			while (!g_hasEvent) {
				::Sleep(1);
			}
			g_hasEvent = false;
			if (c == 'n' || c == 0x72) {
				clrscr();
				usage();
				return 0;
			}
		} while (buffer.length() > 80);
	}
	clrscr();
	usage();
	return 0;
}
