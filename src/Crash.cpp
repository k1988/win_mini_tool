#include <windows.h>

void main()
{
	int a = 100;
    int* b = (int*)0x1ffff;    
    int c = a / *b ;
    return;
}