/******************************************************************************
 *  版权所有（C）terry.zhao                                                   *
 *  保留所有权利。                                                            *
*******************************************************************************
 *  来源 : https://github.com/k1988/win_mini_tool
 *  版本 : 1.0
 *  功能说明：添加权限
******************************************************************************/
/*  修改记录: 
      日期       版本    修改人             修改内容
    --------------------------------------------------------------------------
******************************************************************************/

#include <windows.h>
#include <Aclapi.h>
#include <cstdio>

#pragma comment(lib, "Advapi32.lib")

//
//bool SetDACL(string filePath, string userName)
//{
//	//获取帐户信息	
//	char sid[100];
//	DWORD sidBufferSize = 100;
//	char domainName[MAX_PATH] = {0};
//	DWORD domainBufferSize = MAX_PATH;
//	SID_NAME_USE sidType = SidTypeUser;
//	bool result = LookupAccountName(null, userName.c_str(), sid, &sidBufferSize, domainName, &domainBufferSize, sidType);
//
//	if (!result)
//	{
//		return false;
//	}
//
//	//获取文件描述符
//	ManagementPath path = new ManagementPath();
//	path.Server = ".";
//	path.NamespacePath = @"\root\cimv2";
//	path.RelativePath = @"Win32_LogicalFileSecuritySetting.Path='" + filePath + "'";
//	ManagementObject dir = new ManagementObject(path);
//	ManagementBaseObject outParams = dir.InvokeMethod("GetSecurityDescriptor", null, null);
//
//	if (((uint)(outParams.Properties["ReturnValue"].Value)) != 0)
//	{
//		throw new Exception("获取文件描述符失败");
//	}
//	ManagementBaseObject Descriptor = ((ManagementBaseObject)(outParams.Properties["Descriptor"].Value));
//
//	//获取访问控制列表
//	ManagementBaseObject[] DaclObject = ((ManagementBaseObject[])(Descriptor.Properties["Dacl"].Value));
//
//	//复制一个访问控制项
//	ManagementBaseObject ace = (ManagementBaseObject)DaclObject[0].Clone();
//
//	//设置访问控制项属性
//	ManagementBaseObject trustee = (ManagementBaseObject)ace.Properties["Trustee"].Value;
//	//   return ace.Properties["AccessMask"].Value.ToString();
//	trustee.Properties["Domain"].Value = domainName.ToString();
//	trustee.Properties["Name"].Value = userName;
//	trustee.Properties["SID"].Value = userSid;
//	trustee.Properties["SidLength"].Value = 28;//trustee.Properties["SIDString"].Value="S-1-5-21-602162358-708899826-854245398-1005";
//
//	ace.Properties["Trustee"].Value = trustee;
//	ace.Properties["AccessMask"].Value = 1245631;//2032127完全控制//1245631修改权限
//	ace.Properties["AceFlags"].Value = 3;//允许AccessMask指定的访问级别
//	ace.Properties["AceType"].Value = 0;//允许AccessMask指定的访问级别
//
//	//复制一份访问控制列表，并将以上生成的访问控制项添加到其后。
//	ManagementBaseObject[] newDacl = new ManagementBaseObject[DaclObject.Length + 1];
//	for (int i = 0; i < DaclObject.Length; i++)
//	{
//		newDacl[i] = DaclObject[i];
//	}
//
//	newDacl[DaclObject.Length] = ace;
//	//将安全描述符的DACL属性设为新生成的访问控制列表
//	Descriptor.Properties["Dacl"].Value = newDacl;
//
//	//设置安全描述符
//	dir.Scope.Options.EnablePrivileges = true;
//
//	ManagementBaseObject inProperties = dir.GetMethodParameters("SetSecurityDescriptor");
//	inProperties["Descriptor"] = Descriptor;
//	outParams = dir.InvokeMethod("SetSecurityDescriptor", inProperties, null);
//}

int step = 0;

//
// 启用某个账户对某个文件(夹)的所有操作权限
// pszPath: 文件(夹)路径
// pszAccount: 账户名称
//
BOOL EnableFileAccountPrivilege(PCTSTR pszPath, PCTSTR pszAccount)
{
	BOOL bSuccess = TRUE;
	PACL pNewDacl = NULL, pOldDacl = NULL;
	EXPLICIT_ACCESS ea;
	do
	{
		// 获取文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != ::GetNamedSecurityInfo ((LPTSTR)pszPath, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, NULL))
		{
			step = 1;
			bSuccess = FALSE;
			break;
		}
		// 此处不可直接用AddAccessAllowedAce函数,因为已有的DACL长度是固定,必须重新创建一个DACL对象
		// 生成指定用户帐户的访问控制信息(这里指定赋予全部的访问权限)
		::BuildExplicitAccessWithName (&ea, (LPTSTR)pszAccount, GENERIC_ALL, GRANT_ACCESS, 
SUB_CONTAINERS_AND_OBJECTS_INHERIT);
		step = 2;
		// 创建新的ACL对象(合并已有的ACL对象和刚生成的用户帐户访问控制信息)
		if (ERROR_SUCCESS != ::SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl))
		{
			step = 3;
			bSuccess = FALSE;
			break;
		}

		// 设置文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != ::SetNamedSecurityInfo ((LPTSTR)pszPath, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL))
		{
			step = 4;
			printf("错误:%d", GetLastError());
			bSuccess = FALSE;
		}
	} while (FALSE);

	// 释放资源
	if (pNewDacl != NULL)
	{
		::LocalFree(pNewDacl);
	}
	return bSuccess;
}

/*
//ATL封装了安全操作函数,用ATL来写就简单多了: //
// 启用某个账户对某个文件(夹)的所有操作权限(ATL版本)
// pszPath: 文件(夹)路径
// pszAccount: 账户名称
//
BOOL AtlEnableFileAccountPrivilege (PCTSTR pszPath, PCTSTR pszAccount)
{
	CDacl dacl;
	CSid sid;
	// 获取用户帐户标志符
	if (!sid.LoadAccount (pszAccount))
	{
		return FALSE;
	}
	// 获取文件(夹)的DACL
	if (!AtlGetDacl (pszPath, SE_FILE_OBJECT, &dacl))
	{
		return FALSE;
	}
	// 在DACL中添加新的ACE项
	dacl.AddAllowedAce (sid, GENERIC_ALL);
	// 设置文件(夹)的DACL
	return AtlSetDacl (pszPath, SE_FILE_OBJECT, dacl) ? TRUE : FALSE;
}
*/

void main()
{
	//FIXME TODO

	// Get and display the user name. 
	DWORD size = MAX_PATH;
	char username[MAX_PATH] = {0};
	if(!GetUserName(username, &size))
	{
		printf("错误:%d", GetLastError());
	}

	printf( "\nUser name:%s\n", username);
	if (FALSE == EnableFileAccountPrivilege("C:\\windows\\winsxs\\Manifests", username))
	//if (FALSE == EnableFileAccountPrivilege("D:\\test\\cl\\testdir\\", username))
	{
		printf("step:%d, 错误:%d", step, GetLastError());
	}
	getchar();
}
