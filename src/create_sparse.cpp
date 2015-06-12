#include <stdio.h>
#include <windows.h>
#include <time.h>
#define FSCTL_SET_SPARSE                CTL_CODE(FILE_DEVICE_FILE_SYSTEM, 49, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
bool clear_sparse(HANDLE src_file)
{
	DWORD temp;
	typedef struct _FILE_SET_SPARSE_BUFFER {
	    BOOLEAN SetSparse;
} FILE_SET_SPARSE_BUFFER, *PFILE_SET_SPARSE_BUFFER;
	FILE_SET_SPARSE_BUFFER b;
	b.SetSparse = FALSE;
	if(FALSE == ::DeviceIoControl(src_file, FSCTL_SET_SPARSE, &b, sizeof(b)
		, 0, 0, &temp, 0))
	{
		DWORD re = GetLastError();
		printf("clear_sparse 失败 error code:%d\n", re);
		if (re != ERROR_INVALID_FUNCTION)
		{
		   return false;
		}
	}
	else
	{
		return true;
	}
	return true;
}

bool HasSparseAttribute(HANDLE file)
{
	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(file, &fileSize))
	{
		return false;
	}

	BY_HANDLE_FILE_INFORMATION fileInfo;
	GetFileInformationByHandle(file, &fileInfo);
	if (fileInfo.dwFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
	{
		return true;
	}

	return false;
}


bool is_sparse(HANDLE file)
{
	LARGE_INTEGER file_size;
	if (!GetFileSizeEx(file, &file_size))
		return false;

	typedef struct _FILE_ALLOCATED_RANGE_BUFFER {
		LARGE_INTEGER FileOffset;
		LARGE_INTEGER Length;
	} FILE_ALLOCATED_RANGE_BUFFER, *PFILE_ALLOCATED_RANGE_BUFFER;
#define FSCTL_QUERY_ALLOCATED_RANGES ((0x9 << 16) | (1 << 14) | (51 << 2) | 3)

	FILE_ALLOCATED_RANGE_BUFFER in;
	in.FileOffset.QuadPart = 0;
	in.Length.QuadPart = file_size.QuadPart;

	FILE_ALLOCATED_RANGE_BUFFER out[2];

	DWORD returned_bytes = 0;
	BOOL ret = DeviceIoControl(file, FSCTL_QUERY_ALLOCATED_RANGES, (void*)&in, sizeof(in)
		, out, sizeof(out), &returned_bytes, NULL);

	if (ret == FALSE)
	{
		int error = GetLastError();
		return true;
	}

	// if we only have a single range in the file, we're not sparse
	return returned_bytes != sizeof(FILE_ALLOCATED_RANGE_BUFFER);
}

void main(int argc, char* argv[])
{
	if (argc < 3)
	{
	    printf("Usage: %s sourcefile destfile\n", argv[0]);
		fflush(stdin);
		printf("任意健退出...");
		getchar();
		return ;
	}
	
   	HANDLE src_file = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (src_file  == INVALID_HANDLE_VALUE)
	{
		printf("打开源文件失败!!%d\n", GetLastError());
		return;
	}
	
	HANDLE dest_file = CreateFile(argv[2], GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (dest_file  == INVALID_HANDLE_VALUE)
	{
		printf("创建目标文件失败!!%d\n", GetLastError());
		return;
	}
	printf("设置sparse标志!!\n");
	DWORD temp;
	if (FALSE == ::DeviceIoControl(dest_file, FSCTL_SET_SPARSE, 0, 0, 0, 0, &temp, 0))
	{
	   printf("设置sparse标志失败:%d!!\n", GetLastError());
	}
	else
	{
		printf("设置sparse标志成功!!\n");
	}
		
	LARGE_INTEGER fileSize;
	if (!GetFileSizeEx(src_file, &fileSize))
	{
		return;
	}

	LONGLONG src_size = fileSize.QuadPart;
	LONGLONG dest_pos = 0;
	printf("source file size:%I64u\n", src_size);
	srand(time(NULL));
	
	#define block_size 512*1024
	char* buffer = new char[block_size];
	while (dest_pos < src_size)
	{
	   DWORD numberOfBytesRead = 0;
	   //printf("read offset:%I64u,size:%I64u\n", dest_pos, min(block_size, src_size - dest_pos));
	   if (FALSE == ReadFile(src_file, buffer, min(block_size, src_size - dest_pos), &numberOfBytesRead, NULL))
	   {
	       printf("源文件读取失败(%I64u,%I64u)\n", dest_pos, min(block_size, src_size - dest_pos));
		   return;
	   }
	   
	   // 在100范围内随机，随机到13时跳过该块的写
	   // 在最后4块中，跳过前3块，写入最后一块
	   if ((rand() % 100) == 13 || (src_size - dest_pos < block_size * 4 && src_size - dest_pos > block_size))
	   {
	        printf("保持文件sparse区域(%I64u,%I64u)\n", dest_pos, numberOfBytesRead);
	        SetFilePointer(dest_file, numberOfBytesRead, 0, FILE_CURRENT);
	   }   
	   else 
	   {
		   if (FALSE == WriteFile(dest_file, buffer, numberOfBytesRead, &numberOfBytesRead, NULL))
		   {
			   printf("写入文件失败(%I64u,%I64u)\n", dest_pos, min(block_size, src_size - dest_pos));
			   return;
		   }
		}	   
	   dest_pos += numberOfBytesRead;
	}
	
	printf("选择操作: 2-清除稀疏(vista系统前无效) 其它键退出");
	char c = getchar();	
	if (c == '2')
	{
		if (!clear_sparse(dest_file))
		{
		   printf("清除sparse标志失败!!");
		}
		else
		{
		   printf("清除sparse标志成功!!");
		}
	}
	fflush(stdin);
	printf("任意健退出...");
	getchar();
}