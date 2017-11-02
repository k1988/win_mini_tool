#include <iostream>
#include <fstream>
#include <consoleapi.h>
#include <string>

void usage() {
	std::cout << "usage: " << std::endl
		<< "\tLineRead <txtpath>" << std::endl;
}

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

int main(int argc, char* argv[])
{
	if (argc < 2) {
		usage();
	}

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
			clrscr();
			std::cout << buffer.substr(0,80) << std::endl;
			buffer.erase(0, 80);

			std::cin >> std::noskipws >> c;
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
