// EliteDangerousX52MFD.cpp : Entry point

#include "stdafx.h"
#include "DirectOutputFn.h"
#undef max

using namespace std;

// DirectOutput function object
// Creation of this object automatically loads in DirectOutput but it still needs to be initialized
DirectOutputFn fn;
// Filepath of the profile to use
wchar_t* profileFilepath = TEXT("C:\\Users\\Public\\Documents\\SmartTechnology Profiles\\Elite Dangerous.pr0\0");

void checkHR(HRESULT hr);

int main()
{
	// TODO: Get user to input profile filepath

	// Initialize DirectOutput
	checkHR(fn.Initialize(L"EliteDangerousX52MFD"));

	// Registers and enumerates device
	fn.RegisterDevice();

	// Gets the enumerated device
	fn.GetDeviceType();

	// Set the profile
	checkHR(fn.setDeviceProfile(profileFilepath));

	// Register right soft button clicks and scrolls
	checkHR(fn.registerSoftBtnCallback());

	// Register page change callback
	checkHR(fn.registerPageCallback());
	cout << "Setup Complete.\n\n";

	// Add 5 pages
	for (int i = 0; i < 6; i++)
	{
		if (i == 0)
		{
			checkHR(fn.setPage(0, FLAG_SET_AS_ACTIVE));
		}
		else
		{
			checkHR(fn.setPage(i, 0));
		}
	}

	// Set default greeting
	checkHR(fn.setString(0, 0, TEXT("Greetings CMDR")));
	checkHR(fn.setString(0, 1, TEXT("Page 0")));

	// Pause to check outputs
	cout << "\nPress enter to deinitialize.\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	//Unregister the callbacks
	checkHR(fn.unRegisterSoftBtnCallback());
	checkHR(fn.unRegisterPageCallback());

	// Deinitialize DirectOutput
	checkHR(fn.Deinitialize());
 
	cout << "Press enter to free DirectOutput.dll and quit.\n";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return 0;
}

/*
	PARAMETERS: HRESULT hr == some functions from DirectOutput return a HRESULT value, this just checks if it pass/fail and ouputs to the console
	RETURNS: none

	FUNCTION: Checks resulkt of the function if it returns an HRESULT value

	Read more about HRESULT return types at
	Microsoft (MSDN) -> https://msdn.microsoft.com/en-us/library/windows/desktop/aa378137(v=vs.85).aspx
	Wikipedia -> https://en.wikipedia.org/wiki/HRESULT
*/
void checkHR(HRESULT hr) {
	if (hr == S_OK)
	{
		cout << "DONE.\n";
	}
	else
	{
		cout << "FAILED/ hr = " << hr << endl;
	}
}
