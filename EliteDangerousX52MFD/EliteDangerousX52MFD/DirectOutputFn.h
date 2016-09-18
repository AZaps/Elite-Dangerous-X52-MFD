#pragma once
#include "../Include/DirectOutput.h"

class DirectOutputFn
{
	typedef std::vector<void*> DeviceList;
	DeviceList m_devices;
	HMODULE dll;
	HRESULT hr;

public:
	DirectOutputFn();
	~DirectOutputFn();

	HRESULT Initialize(const wchar_t* wszPluginName);
	HRESULT Deinitialize();
	void RegisterDevice();
	void GetDeviceType();
	HRESULT setDeviceProfile(wchar_t* filepath);
	HRESULT setPage(int pageNumber, const DWORD flag);
	HRESULT setString(int pageNumber, int stringLineID, wchar_t* stringToOutput);
	
private:
	static void __stdcall OnEnumerateDevice(void* hDevice, void* pCtxt);
	static void __stdcall OnDeviceChanged(void* hDevice, bool bAdded, void* pCtxt);
	
};

