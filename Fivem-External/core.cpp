
#include <Windows.h>
#include <thread>
#include <future>
#include <Cheat/Cheat.hpp>
#include <FrameWork/FrameWork.hpp>
#include <tchar.h>

static BOOL CheckForUIAccess(DWORD* pdwErr, BOOL* pfUIAccess) 
{
    BOOL result = FALSE;
    HANDLE hToken;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
    {
        DWORD dwRetLen;

        if (GetTokenInformation(hToken, TokenUIAccess, pfUIAccess, sizeof(*pfUIAccess), &dwRetLen))
        {
            result = TRUE;
        }
        else
        {
            *pdwErr = GetLastError(); 
        }
        CloseHandle(hToken); 
    }
    else
    {
        *pdwErr = GetLastError(); 
    }

    return result;
}

DWORD WINAPI Unload()
{
    SafeCall(ExitProcess)(0); 
    return TRUE;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    Cheat::Initialize(); 
}