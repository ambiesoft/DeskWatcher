#include "stdafx.h"

#include "../../lsMisc/IsDirectory.h"

using namespace Ambiesoft;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	TCHAR szDesktopDir[MAX_PATH] = { 0 };
	if (!SHGetSpecialFolderPath(HWND_DESKTOP, szDesktopDir, CSIDL_DESKTOP, FALSE))
	{
		return 1;
	}
	if (szDesktopDir[0] == 0 || !IsDirectory(szDesktopDir))
	{
		return 1;
	}

	HANDLE hChangeHandle = FindFirstChangeNotification(
		szDesktopDir,                         // directory to watch 
		FALSE,                         // do not watch subtree 
		FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file name changes 

	while (TRUE)
	{
		DWORD dwWaitStatus = WaitForSingleObject(hChangeHandle, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:

			// A file was created, renamed, or deleted in the directory.
			// Refresh this directory and restart the notification.

			if (!FindNextChangeNotification(hChangeHandle))
			{
				// printf("\n ERROR: FindNextChangeNotification function failed.\n");
				ExitProcess(GetLastError());
			}
			break;

		case WAIT_TIMEOUT:

			// A timeout occurred, this would happen if some value other 
			// than INFINITE is used in the Wait call and no changes occur.
			// In a single-threaded environment you might not want an
			// INFINITE wait.

			// printf("\nNo changes in the timeout period.\n");
			break;

		default:
			// printf("\n ERROR: Unhandled dwWaitStatus.\n");
			ExitProcess(GetLastError());
			break;
		}
	}




	return 0;
}
