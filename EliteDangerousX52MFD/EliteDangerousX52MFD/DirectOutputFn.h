#pragma once
#include "DepInclude\DirectOutput.h"

class DirectOutputFn
{
	typedef std::vector<void*> DeviceList;
	DeviceList m_devices;
	HMODULE dll;
	HRESULT hr;
	int m_scrollpos;
	int currentPage;

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
	HRESULT registerSoftBtnCallback();
	HRESULT registerPageCallback();
	HRESULT unRegisterSoftBtnCallback();
	HRESULT unRegisterPageCallback();
	int getCurrentPage();
	void handlePageChange();

private:
	static void __stdcall OnEnumerateDevice(void* hDevice, void* pCtxt);
	static void __stdcall OnDeviceChanged(void* hDevice, bool bAdded, void* pCtxt);
	static void __stdcall OnPageChanged(void* hDevice, DWORD dwPage, bool bSetActive, void* pCtxt);
	static void __stdcall OnSoftButtonChanged(void* hDevice, DWORD dwButtons, void* pCtxt);
	void updatePageOnScroll(int oneUpZeroDown);
	void updatePage(int pageNumber);
};