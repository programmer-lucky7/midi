#include <windows.h>
#include <mmsystem.h>

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

TCHAR szClsName[] = TEXT("LUCKY7_MIDI");
TCHAR szAppName[] = TEXT("MIDI Keyboard by Lucky7");
HINSTANCE g_hInst;
int g_nNoteOffset = 0;
unsigned char matrix[256] = {
	 0,  0,  0,  0,  0,  0,  0,  0, 84, 52,  0,  0,  0, 83,  0,  0,
	 0,  0,  0,  0, 48,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0, 89, 88, 86, 87,  0,  0,  0,  0,  0,  0,  0,  0, 85, 84,  0,
	 0, 52, 54, 56, 58,  0, 61, 71, 73, 75,  0,  0,  0,  0,  0,  0,
	 0, 59, 67, 64, 63, 57,  0, 66, 68, 74, 70,  0, 73, 71, 69, 76,
	77, 53, 59, 61, 60, 72, 65, 55, 62, 62, 60,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0, 89, 91, 93, 94, 95,  0, 96,  0, 92,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	90,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 75, 80, 72, 78, 74, 76,
	50,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 79, 82, 81, 77,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
unsigned char drummap[12] = {
//	kick	snare1	snare2	c-hat	h-tom	m-tom	l-tom	floor	cymbal	ride	o-hat	f-hat
	36,	40,	38,	42,	47,	45,	43,	41,	49,	51,	46,	44
};

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
	if ((int) note + g_nNoteOffset >= 0 && (int) note + g_nNoteOffset < 128) {
		note += g_nNoteOffset;
		midiOutShortMsg(hmo, 0x007F0090 | (note << 8));
	}
}

void noteOff(HMIDIOUT hmo, unsigned char note) {
	if ((int) note + g_nNoteOffset >= 0 && (int) note + g_nNoteOffset < 128) {
		note += g_nNoteOffset;
		midiOutShortMsg(hmo, 0x007F0080 | (note << 8));
	}
}

void inline drumOn(HMIDIOUT hmo, unsigned char note) {
	midiOutShortMsg(hmo, 0x007F0099 | (note << 8));
}

void inline drumOff(HMIDIOUT hmo, unsigned char note) {
	midiOutShortMsg(hmo, 0x007F0089 | (note << 8));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	PAINTSTRUCT ps;
	static HMIDIOUT hmo;

	switch(uMsg) {
	case WM_CREATE:
		midiOutOpen(&hmo, 0xFFFFFFFFU, 0, 0, CALLBACK_NULL);
		return 0;
	case WM_SYSKEYDOWN:
		switch(LOWORD(wParam)) {
		case VK_F10:
			drumOn(hmo, drummap[9]);
			return 0;
		}
		break;
	case WM_SYSKEYUP:
		switch(LOWORD(wParam)) {
		case VK_F10:
			drumOff(hmo, drummap[9]);
			return 0;
		}
		break;
	case WM_KEYDOWN:
		if ((lParam >> 30) & 1) return 0; // Prevent auto-repeat
		switch(LOWORD(wParam)) {
		case VK_LEFT: g_nNoteOffset -= 12; break; // 1-octave down
		case VK_RIGHT: g_nNoteOffset += 12; break; // 1-octave up
		case VK_DOWN: g_nNoteOffset -= 1; break; // half-note down
		case VK_UP: g_nNoteOffset += 1; break; // half-note up
		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F11:
		case VK_F12:
			drumOn(hmo, drummap[LOWORD(wParam) - VK_F1]);
			break;
		default:
			wParam = wParam & 0xFF;
			if (matrix[wParam]) noteOn(hmo, matrix[wParam]);
		}
		return 0;
	case WM_KEYUP:
		switch(LOWORD(wParam)) {
		case VK_LEFT:
		case VK_RIGHT:
		case VK_DOWN:
		case VK_UP: break; // ignore left, right, down, up arrow key up event
		case VK_F1:
		case VK_F2:
		case VK_F3:
		case VK_F4:
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F11:
		case VK_F12:
			drumOff(hmo, drummap[LOWORD(wParam) - VK_F1]);
			break;
		default:
			wParam = wParam & 0xFF;
			if (matrix[wParam]) noteOff(hmo, matrix[wParam]);
		}
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		TextOut(hDC, 0, 0, "MIDI Keyboard by Lucky7", 23);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		midiOutClose(hmo);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
