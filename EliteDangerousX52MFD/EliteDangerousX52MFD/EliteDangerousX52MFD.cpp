// EliteDangerousX52MFD.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "DirectOutputFn.h"
#undef max

using namespace std;
HRESULT hr;


int main()
{
	// Creating an object of the class automatically loads in the DirectOutput dll
	DirectOutputFn fn;

	hr = fn.Initialize(L"EliteDangerousX52MFD");
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (hr == S_OK)
	{
		cout << "DONE.\n";
	}
	else
	{
		cout << "FAILED.\n";
	}

	hr = fn.Deinitialize();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if (hr == S_OK)
	{
		cout << "DONE.\n";
	}
	else
	{
		cout << "FAILED.\n";
	}

	
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return 0;
}

