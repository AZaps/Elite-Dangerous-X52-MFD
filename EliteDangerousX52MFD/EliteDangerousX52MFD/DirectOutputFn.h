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

private:

};

