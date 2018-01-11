#include <iostream> 
/*包含time头文件*/
#include <time.h>
 
 
using namespace std;
 
int main()
{
    //time_t是long类型，精确到秒，是当前时间和1970年1月1日零点时间的差
    const time_t t = time(NULL);
 
    cout<<"current time is "<<t<<endl;
 
    /*本地时间：日期，时间 年月日，星期，时分秒*/
    struct tm* current_time = localtime(&t);
    printf("current year is %d;current month is %d;current date of month is %d\r\n",
        1900 + current_time->tm_year,
        1 + current_time->tm_mon/*此month的范围为0-11*/,
        current_time->tm_mday);
 
    printf("current day of year is %d;current day in week is %d\r\n",
        current_time->tm_yday,/*当前日期是今年的第多少天[0,365] */
        current_time->tm_wday/*days since Sunday - [0,6] */);
 
    printf("time part %d:%d:%d \r\n",
        current_time->tm_hour,
        current_time->tm_min,
        current_time->tm_sec);
 
    printf("\t本地时间：%d-%d-%d %d:%d:%d\r\n",
        current_time->tm_year + 1900,
        current_time->tm_mon + 1,
        current_time->tm_mday,
        current_time->tm_hour,
        current_time->tm_min,
        current_time->tm_sec);
 
    /*格林威治时间*/
    struct tm* current_gmtime = gmtime(&t);
 
    printf("格林威治时间：%d-%d-%d %d:%d:%d\r\n",
        current_gmtime->tm_year + 1900,
        current_gmtime->tm_mon + 1,
        current_gmtime->tm_mday,
        current_gmtime->tm_hour,
        current_gmtime->tm_min,
        current_gmtime->tm_sec);
 
 
    system("pause");
    return 0;
}