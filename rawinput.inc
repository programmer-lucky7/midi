#ifdef __RAWINPUT_H_
{
	DWORD dwWinVer = GetVersion();
	if (dwWinVer < 0x80000000U && (dwWinVer & 0xFF) >= 5) {
		HINSTANCE hLibrary = LoadLibrary(TEXT("user32"));
		if (hLibrary) {
			*((FARPROC *) &RegisterRawInputDevices) = GetProcAddress(hLibrary, "RegisterRawInputDevices");
			*((FARPROC *) &GetRegisteredRawInputDevices) = GetProcAddress(hLibrary, "GetRegisteredRawInputDevices");
			*((FARPROC *) &GetRawInputDeviceList) = GetProcAddress(hLibrary, "GetRawInputDeviceList");
			*((FARPROC *) &DefRawInputProc) = GetProcAddress(hLibrary, "DefRawInputProc");
#ifdef UNICODE
			*((FARPROC *) &GetRawInputDeviceInfo) = GetProcAddress(hLibrary, "GetRawInputDeviceInfoW");
#else
			*((FARPROC *) &GetRawInputDeviceInfo) = GetProcAddress(hLibrary, "GetRawInputDeviceInfoA");
#endif
			*((FARPROC *) &GetRawInputBuffer) = GetProcAddress(hLibrary, "GetRawInputBuffer");
			*((FARPROC *) &GetRawInputData) = GetProcAddress(hLibrary, "GetRawInputData");
			if (RegisterRawInputDevices && GetRegisteredRawInputDevices && GetRawInputDeviceList && DefRawInputProc && GetRawInputDeviceInfo && GetRawInputBuffer && GetRawInputData) {
			//	MessageBox(NULL, TEXT("Initialized function pointer!"), TEXT("INIT"), MB_ICONINFORMATION);
			} else {
				MessageBox(NULL, TEXT("Cannot initialize function pointer!"), TEXT("ERROR"), MB_ICONERROR);
				FreeLibrary(hLibrary);
				return 1;
			}
			FreeLibrary(hLibrary);
		}
	} else {
		MessageBox(NULL, TEXT("Windows XP or above required!"), TEXT("Unsupported Windows Version"), MB_ICONWARNING);
		return 1;
	}
}
#endif
