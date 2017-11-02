#include <stdio.h>
#include <TChar.h>
#include <windows.h>
#include <locale.h>

int main(int argc, char* argv[]){
    setlocale(LC_ALL, "");//设置本地语言,这样可以再控制台完美显现宽字符    
    printf("test printf：\n");
    printf("%.32hs","这是一个%%.32hs格式的字符串12345678912345678912345678945613123\n");
    printf("\n");
    printf("%.32Ls",L"12321一个%%.32ls格式的字符串");//result: 1
    printf("%.32LS",L"12321一个%%.32ls格式的字符串");//<如果没有setlocale，则这里只打印出12321


    return 0;
}