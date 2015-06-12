#include <cstdio>
#include <string.h>

void main(int argc, char** argv)
{
	if (argc > 1) printf("%sµÄ³¤¶È£º\r\n%d", argv[1], strlen(argv[1]));
}