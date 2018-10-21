#include <windows.h>
#include <mmsystem.h>

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

TCHAR szClsName[] = TEXT("LUCKY7_MIDI");
TCHAR szAppName[] = TEXT("MIDI Keyboard by Lucky7");
HINSTANCE g_hInst;

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hWnd;
	MSG msg;

	g_hInst = hInst;
	wc.cbSize = sizeof (WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor (NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szClsName;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);
	hWnd = CreateWindowEx (0, szClsName, szAppName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
	ShowWindow (hWnd, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

void noteOn(HMIDIOUT hmo, unsigned char note) {
	midiOutShortMsg(hmo, 0x007F0090 | (note << 8));
}

void noteOff(HMIDIOUT hmo, unsigned char note) {
	midiOutShortMsg(hmo, 0x007F0080 | (note << 8));
}

int matrix[128] = { 0, };
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	static HMIDIOUT hmo;

	switch(uMsg) {
	case WM_CREATE:
		midiOutOpen(&hmo, 0xFFFFFFFFU, 0, 0, CALLBACK_NULL);
		matrix['Z'] = 60;
		matrix['X'] = 62;
		matrix['C'] = 64;
		matrix['V'] = 65;
		matrix['B'] = 67;
		matrix['N'] = 69;
		matrix['M'] = 71;
		return 0;
	case WM_KEYDOWN:
		if ((lParam >> 30) & 1) return 0;
		if (wParam >= 128) return 0;
		if (matrix[wParam]) noteOn(hmo, matrix[wParam]);
		return 0;
	case WM_KEYUP:
		if (wParam >= 128) return 0;
		if (matrix[wParam]) noteOff(hmo, matrix[wParam]);
		return 0;
	case WM_PAINT:
		hDC = GetDC(hWnd);
		TextOut(hDC, 0, 0, "MIDI Keyboard by Lucky7", 23);
		ReleaseDC(hWnd, hDC);
		return 0;
	case WM_DESTROY:
		midiOutClose(hmo);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
