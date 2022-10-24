#include "Error.h"

void SysError(LPCWSTR message) {
	DWORD error_id = GetLastError();
	wchar_t* buffer = nullptr;
	DWORD message_len = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		error_id,
		0,
		buffer,
		0,
		NULL
	);
	wprintf(L"Get error %u (%s): %s", error_id, buffer, message != NULL ? message : L"");

	MessageBoxW(NULL, message != NULL ? message : buffer, L"Error", MB_OK);

	LocalFree(buffer);

	ExitProcess(error_id);
}