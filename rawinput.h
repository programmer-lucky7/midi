#ifndef __RAWINPUT_H_
#define __RAWINPUT_H_
#ifndef WM_INPUT
  #define WM_INPUT 0x00FF
  #define RID_INPUT 0x10000003
  #define RID_HEADER 0x10000005
  #define RIM_TYPEKEYBOARD 1
  #define RIM_INPUT 0
  #define RIM_INPUTSINK 1
  #define RIDI_DEVICENAME 0x20000007
  #define RIDEV_REMOVE 0x00000001
  #define RIDEV_INPUTSINK 0x00000100
  typedef BOOL WINBOOL;
  typedef HANDLE HRAWINPUT;
  typedef struct tagRAWINPUTHEADER {
    DWORD dwType;
    DWORD dwSize;
    HANDLE hDevice;
    WPARAM wParam;
  } RAWINPUTHEADER,*PRAWINPUTHEADER,*LPRAWINPUTHEADER;
  typedef struct tagRAWHID {
    DWORD dwSizeHid;
    DWORD dwCount;
    BYTE bRawData[1];
  } RAWHID,*PRAWHID,*LPRAWHID;
  typedef struct tagRAWMOUSE {
    USHORT usFlags;
    union {
      ULONG ulButtons;
      struct {
	USHORT usButtonFlags;
	USHORT usButtonData;
      };
    };
    ULONG ulRawButtons;
    LONG lLastX;
    LONG lLastY;
    ULONG ulExtraInformation;
  } RAWMOUSE,*PRAWMOUSE,*LPRAWMOUSE;
  typedef struct tagRAWKEYBOARD {
    USHORT MakeCode;
    USHORT Flags;
    USHORT Reserved;
    USHORT VKey;
    UINT Message;
    ULONG ExtraInformation;
  } RAWKEYBOARD,*PRAWKEYBOARD,*LPRAWKEYBOARD;
  typedef struct tagRAWINPUT {
    RAWINPUTHEADER header;
    union {
      RAWMOUSE mouse;
      RAWKEYBOARD keyboard;
      RAWHID hid;
    } data;
  } RAWINPUT,*PRAWINPUT,*LPRAWINPUT;
  typedef struct tagRAWINPUTDEVICELIST {
    HANDLE hDevice;
    DWORD dwType;
  } RAWINPUTDEVICELIST,*PRAWINPUTDEVICELIST;
  typedef struct tagRAWINPUTDEVICE {
    USHORT usUsagePage;
    USHORT usUsage;
    DWORD dwFlags;
    HWND hwndTarget;
  } RAWINPUTDEVICE,*PRAWINPUTDEVICE,*LPRAWINPUTDEVICE;
  typedef CONST RAWINPUTDEVICE *PCRAWINPUTDEVICE;

  WINBOOL (WINAPI *RegisterRawInputDevices) (PCRAWINPUTDEVICE pRawInputDevices, UINT uiNumDevices, UINT cbSize);
  UINT (WINAPI *GetRegisteredRawInputDevices) (PRAWINPUTDEVICE pRawInputDevices, PUINT puiNumDevices, UINT cbSize);
  UINT (WINAPI *GetRawInputDeviceList) (PRAWINPUTDEVICELIST pRawInputDeviceList, PUINT puiNumDevices, UINT cbSize);
  LRESULT (WINAPI *DefRawInputProc) (PRAWINPUT *paRawInput, INT nInput, UINT cbSizeHeader);
  UINT (WINAPI *GetRawInputDeviceInfo) (HANDLE hDevice, UINT uiCommand, LPVOID pData, PUINT pcbSize);
  UINT (WINAPI *GetRawInputBuffer) (PRAWINPUT pData, PUINT pcbSize, UINT cbSizeHeader);
  UINT (WINAPI *GetRawInputData) (HRAWINPUT hRawInput, UINT uiCommand, LPVOID pData, PUINT pcbSize, UINT cbSizeHeader);
#endif
#endif
