#include <direct.h>
#include <windows.h>
#include "islmem.h"
#include "islfile.h"
#include "Main.h"

#define PSX_BASEDIR	"X:\\TeamSpirit\\psxversion\\cd\\"

void fileInitialise(char *fileSystem)
{
}

unsigned char *fileLoad(const char *filename, int *bytesRead)
{
	void *buffer;
	DWORD size, read;
	HANDLE h;

	SetCurrentDirectory(PSX_BASEDIR);
	h = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h == INVALID_HANDLE_VALUE) return NULL;
	size = GetFileSize(h, NULL);
	buffer = MALLOC0(size);
	ReadFile(h, buffer, size, &read, NULL);
	CloseHandle(h);
	
	if (bytesRead) *bytesRead = read;
	return (unsigned char*)buffer;
}
