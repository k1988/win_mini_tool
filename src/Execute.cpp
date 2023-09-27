#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include <tchar.h>

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"User32.lib")


bool WinShellExecuteEx(LPCTSTR lpOperation,
                       LPCTSTR lpFile,
                       LPCTSTR lpParameters,
                       LPCTSTR lpDirectory,
                       HANDLE *hProcess,
                       DWORD *errorCode,
                       ULONG fMask,
                       HWND hWnd,
                       INT nShowCmd)
{
    SHELLEXECUTEINFO execInfo = {0};
    execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    execInfo.hwnd = (hWnd != NULL) ? hWnd : ::GetActiveWindow();
    execInfo.lpVerb = lpOperation;
    execInfo.lpFile = lpFile;
    execInfo.lpParameters = lpParameters;
    execInfo.lpDirectory = lpDirectory;
    execInfo.nShow = nShowCmd;

    if (!hProcess)
    {
        execInfo.hProcess = NULL;
        execInfo.fMask = fMask;
    }
    else
    {
        *hProcess = NULL;
        execInfo.fMask = fMask | SEE_MASK_NOCLOSEPROCESS;
    }

    if (::ShellExecuteEx(&execInfo))
    {
        if (hProcess)
        {
            *hProcess = execInfo.hProcess;
        }
        return (execInfo.hInstApp > (HINSTANCE)32);
    }
    else
    {
        if (errorCode)
        {
            *errorCode = ::GetLastError();
        }
        return false;
    }
}

int main(int argc, TCHAR *argv[])
{
    if (WinShellExecuteEx(_T("open"), argv[1], nullptr, _T(R"(C:\Program Files (x86)\应用程序)"), 0, 0, 0, 0, SW_SHOWDEFAULT))
    {
        //MessageBoxA(0, "test","启动成功", MB_OK);
    }
}

// int test()
// {
//     LPITEMIDLIST pidlWinFiles = NULL;
//     LPITEMIDLIST pidlItems = NULL;
//     IShellFolder *psfWinFiles = NULL;
//     IShellFolder *psfDeskTop = NULL;
//     LPENUMIDLIST ppenum = NULL;
//     STRRET strDispName;
//     TCHAR pszParseName[MAX_PATH];
//     ULONG celtFetched;
//     SHELLEXECUTEINFO ShExecInfo;
//     HRESULT hr;
//     BOOL fBitmap = FALSE;

//     hr = SHGetFolderLocation(NULL, CSIDL_WINDOWS, NULL, NULL, &pidlWinFiles);
//     hr = SHGetDesktopFolder(&psfDeskTop);
//     hr = psfDeskTop->BindToObject(pidlWinFiles, NULL, IID_IShellFolder, (LPVOID *)&psfWinFiles);
//     hr = psfDeskTop->Release();
//     hr = psfWinFiles->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &ppenum);
//     while (hr = ppenum->Next(1, &pidlItems, &celtFetched) == S_OK && (celtFetched) == 1)
//     {
//         psfWinFiles->GetDisplayNameOf(pidlItems, SHGDN_FORPARSING, &strDispName);
//         StrRetToBuf(&strDispName, pidlItems, pszParseName, MAX_PATH);
//         CoTaskMemFree(pidlItems);
//         if (StrCmpI(PathFindExtension(pszParseName), TEXT(".bmp")) == 0)
//         {
//             fBitmap = TRUE;
//             break;
//         }
//     }

//     ppenum->Release();

//     if (fBitmap)
//     {
//         ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
//         ShExecInfo.fMask = NULL;
//         ShExecInfo.hwnd = NULL;
//         ShExecInfo.lpVerb = NULL;
//         ShExecInfo.lpFile = pszParseName;
//         ShExecInfo.lpParameters = NULL;
//         ShExecInfo.lpDirectory = NULL;
//         ShExecInfo.nShow = SW_MAXIMIZE;
//         ShExecInfo.hInstApp = NULL;

//         ShellExecuteEx(&ShExecInfo);
//     }

//     CoTaskMemFree(pidlWinFiles);
//     psfWinFiles->Release();

//     return 0;
// }