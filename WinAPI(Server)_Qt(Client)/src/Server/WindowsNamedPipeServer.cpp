//
// Title               : Native Messaging using Named Pipe to  Qt C++ Client
// Author              : Mandar Shinde
// Interface           : Console
// IDE                 : Visual Studio 2013
// Operating System    : Windows 10
//
//--------------------------------------------------------
//Windows named pipe demo running
//  > Pipe name is[\\.\pipe\my_pipe_name]...
//  --------------------------------------------------------
//  Data received on pipe...[\\.\pipe\my_pipe_name].
//  Going to read data from pipe
//  > Total Message Size[37]
//  > Message Size[32]
//  -------------------- - Message Received------------------ -
//  [Client Hello from Qt Application]
//------------------------------------------------------------ -

#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <winsock.h>
#include <tchar.h>

#define PIPE_NAME	_T("\\\\.\\pipe\\my_pipe_name")
#define BUFSIZE 4096

int main()
{
	BOOL fConnected;
	HANDLE hPipe, instance_handle;
	TCHAR pipe_name[512] = { 0 };

	printf("\n--------------------------------------------------------\n Windows named pipe demo running\n  > Pipe name is [%S]... \n--------------------------------------------------------\n", PIPE_NAME);
	wcscpy((wchar_t *)pipe_name, (wchar_t *)PIPE_NAME);

	//Make sure every can access it.
	SECURITY_DESCRIPTOR sd;
	if (!(InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION) && SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE)))
	{
		printf("InitializeSecurityDescriptor failed.\n");
	}

	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = (LPVOID)&sd;
	sa.bInheritHandle = FALSE;

	for (;;)
	{
		hPipe = CreateNamedPipe(pipe_name,PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			BUFSIZE,BUFSIZE,
			NMPWAIT_USE_DEFAULT_WAIT,
			&sa);
		
		if (hPipe == INVALID_HANDLE_VALUE)
		{
			printf("CRITICAL ERROR: Failed to create the pipe. hPIpe:[0x%x] \n",hPipe);
			return 0;
		}

		if (0 == ConnectNamedPipe(hPipe, NULL))
		{
			int last_error = GetLastError();
			printf("ConnectNamedPipe failed. hPIpe:[0x%x] Error:[%d]\n", hPipe, last_error);

			if (ERROR_PIPE_CONNECTED == last_error)
			{
				fConnected = TRUE;
			}
			else if (ERROR_NO_DATA == last_error)
			{
				DisconnectNamedPipe(hPipe);
				printf("Calling ConnectNamedPipe again after it failed.hPIpe :[0x%x] \n", hPipe);
				fConnected = ConnectNamedPipe(hPipe, NULL);
			}
		}
		else
		{
			fConnected = TRUE;
			printf(" Data received on pipe... [%S].  \n",PIPE_NAME);
		}
		
		if (fConnected)
		{
			printf("  Going to read data from pipe  \n");
			DWORD cbBytesRead = 0;
			char  data[4096] = { 0 };
			ReadFile(hPipe, data, 4096, &cbBytesRead, NULL);
			printf("  > Total Message Size [%d]  \n",cbBytesRead);
			printf("  > Message Size [%d]  \n", cbBytesRead-5); // -4 head size -1 string zero
			char destination[4096];
			memcpy(destination, data + 4, cbBytesRead);
			FlushFileBuffers(hPipe);
			printf("--------------------- Message Received -------------------\n [%s] \n-------------------------------------------------------------\n\n", destination);

			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
			hPipe = INVALID_HANDLE_VALUE;
		}
		else
		{
			printf("ConnectNamedPipe failed. closing handle hPIpe:[0x%x] \n",hPipe);
			CloseHandle(hPipe);
			hPipe = INVALID_HANDLE_VALUE;
		}
	}

	if (hPipe != INVALID_HANDLE_VALUE)
		CloseHandle(hPipe);

	CloseHandle(instance_handle);
	ExitThread(1);
	return 1;
}