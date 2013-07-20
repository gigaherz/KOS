#include "PEFile.h"

void LoadPE(void* data)
{
	PIMAGE_DOS_HEADER pdosheader = (PIMAGE_DOS_HEADER)data;
	PIMAGE_NT_HEADERS pntheaders = (PIMAGE_NT_HEADERS)((DWORD)data + pdosheader->e_lfanew);
	PIMAGE_SECTION_HEADER psectionheader = (PIMAGE_SECTION_HEADER)(pntheaders + 1);
	PIMAGE_IMPORT_DESCRIPTOR pimportdescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)data + pntheaders->OptionalHeader.DataDirectory[15].VirtualAddress);
	PIMAGE_THUNK_DATA pthunkdatain, pthunkdataout;
	PIMAGE_IMPORT_BY_NAME pimportbyname;
	PIMAGE_BASE_RELOCATION pbaserelocation;
	DWORD dw;
	PWORD pw;
	PDWORD pdw;
	PCHAR ptr;
	TCHAR buff[1024];
	HANDLE handle;

	psectionheader->


	while ( pimportdescriptor->TimeDateStamp != 0 ||pimportdescriptor->Name != 0)
	{
		ptr = (PCHAR)((DWORD)data+ pimportdescriptor->Name);
		handle = GetModuleHandle(ptr);
		if (handle == NULL)
			handle = LoadLibrary(ptr);
		if (handle == NULL)
		{
			wsprintf(buff, "A required .DLL file, %hs, was not found.", ptr);
			MessageBox(HWND_DESKTOP, buff, "Error Starting Program", MB_OK|MB_ICONEXCLAMATION);
			ExitProcess(0);
		}

		if (pimportdescriptor->TimeDateStamp != -1)
		{
			pimportdescriptor->ForwarderChain  = (DWORD)handle;
			pimportdescriptor->TimeDateStamp = 0xCDC31337;

			pthunkdataout = (PIMAGE_THUNK_DATA)((DWORD)hInstance + (DWORD)pimportdescriptor->FirstThunk);
			if (pimportdescriptor->Characteristics == 0)
				pthunkdatain = pthunkdataout;
			else 
				pthunkdatain = (PIMAGE_THUNK_DATA)((DWORD)hInstance + (DWORD)pimportdescriptor->Characteristics);

			while ( pthunkdatain->u1.AddressOfData != NULL)
			{
				if ((DWORD)pthunkdatain->u1.Ordinal & IMAGE_ORDINAL_FLAG) // no name, just ordinal
				{
					dw = (DWORD)GetProcAddress(handle, MAKEINTRESOURCE(LOWORD(pthunkdatain->u1.Ordinal)));
				} else {
					pimportbyname = (PIMAGE_IMPORT_BY_NAME)((DWORD)pthunkdatain->u1.AddressOfData + (DWORD)hInstance);
					dw = (DWORD)GetProcAddress(handle, pimportbyname->Name);
				}
				if (dw == 0)
				{
					if ((DWORD)pthunkdatain->u1.AddressOfData & IMAGE_ORDINAL_FLAG)
					{
						wsprintf(buff, "The %hs file is \nlinked to missing export %hs:0x%04x.", exename, ptr, LOWORD(pthunkdatain->u1.AddressOfData));
					} else {
						wsprintf(buff, "The %hs file is \nlinked to missing export %hs:%hs.", exename, ptr, pimportbyname->Name);
					}
					MessageBox(HWND_DESKTOP, buff, "Error Starting Program", MB_OK|MB_ICONEXCLAMATION);
					ExitProcess(0);

				}

				pthunkdataout->u1.Function = (PDWORD)dw;

				pthunkdatain++;
				pthunkdataout++;
			}

		} else { // -1 = new style bound import
			// don't know how to handle these yet...
			dw = 0;
			wsprintf(buff, "New style bound import: %hs", ptr);
			MessageBox(HWND_DESKTOP, buff, "Error:", MB_OK );
		}
		pimportdescriptor++;                
	}


	if ((DWORD)hInstance != pntheaders->OptionalHeader.ImageBase && pntheaders->OptionalHeader.DataDirectory[15].Size)
	{
		pbaserelocation = (PIMAGE_BASE_RELOCATION)((DWORD)hInstance + pntheaders->OptionalHeader.DataDirectory[15].Size);
		while (pbaserelocation->VirtualAddress != 0)
		{
			pw = (PWORD)((DWORD)pbaserelocation + sizeof(IMAGE_BASE_RELOCATION));
			for (x = 0; x < (pbaserelocation->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))/2; x++)
			{
				pdw = (PDWORD)(pbaserelocation->VirtualAddress + (DWORD)hInstance + (*pw & 0xFFF));
				switch(*pw >> 12)
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					dw = *pdw;
					dw = dw - pntheaders->OptionalHeader.ImageBase + (DWORD)hInstance;
					if (dw < (DWORD)hInstance || dw > (DWORD)hInstance + pntheaders->OptionalHeader.SizeOfImage)
					{
						wsprintf(buff, "*pdw = 0x%08x", *pdw);
						MessageBox(HWND_DESKTOP, buff, "Bad pointer:", MB_OK );
					} else {
						*pdw = dw;
					}
					break;
				default:
				case IMAGE_REL_BASED_HIGH:
				case IMAGE_REL_BASED_LOW:
					// these are not used in Win32 exe's
					wsprintf(buff, "*pw = 0x%04x  *pdw = 0x%08x", *pw, *pdw);
					MessageBox(HWND_DESKTOP, buff, "Unexpected relocation type:", MB_OK);
					break;
				}

				pw++;
			}
			pbaserelocation = (PIMAGE_BASE_RELOCATION)((DWORD)pbaserelocation + pbaserelocation->SizeOfBlock);
		}                
	}

}