/******************************************************************************
 *  版权所有（C）zhaohaiyang_1988@163.com                                                 *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：正则式替换文本文件
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <iostream>
#include <string>
#include "boost/regex.hpp"

void Usage()
{
	std::cout << "帮助：\n" << "命令行 file srcRegex destRegex" << std::endl;
}

int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		Usage();
		return 0;
	}

	//D:\\predev\\自动打包\\GMProductVersion.h -> (GM_VER_REVISION )([0-9]*) -> GM_VER_REVISION xxxx
	//D:\\predev\\自动打包\\GMProductVersion.h -> (VER_REVISION=)([0-9]*) -> VER_REVISION=xxxx
	FILE* inFile = fopen(argv[1], "r+");
	if (!inFile)
	{
		std::cerr << "打开文件失败" << std::endl;
		return 1;
	}
//boost::regex reg("(VER_REVISION=)([0-9]*)",

	boost::regex reg(argv[2], boost::regex::icase|boost::regex::perl);

	char buffer[4096];
	size_t count = fread(buffer, 1, 4096, inFile);
	if ((count ) > 0)
	{
		std::string s(buffer, count);
		s = boost::regex_replace(s,reg, argv[3]);
		int re = fseek(inFile, 0, SEEK_SET);
		size_t count_write = fwrite(s.c_str(), 1, s.length(), inFile);
		if (count_write != count)
		{
			std::cerr << "读写不一" << std::endl;
			return 3;
		}
		//std::cout << s;
	}	
	return 0;
}

