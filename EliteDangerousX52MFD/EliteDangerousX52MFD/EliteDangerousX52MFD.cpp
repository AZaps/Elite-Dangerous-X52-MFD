// EliteDangerousX52MFD.cpp : Entry point

#include "stdafx.h"
#include <chrono>
#include <sys/stat.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <string>
#include <sstream>
#include <thread>
#include "DirectOutputFn.h"

using namespace std;

// DirectOutput function object
// Creation of this object automatically loads in DirectOutput but it still needs to be initialized
DirectOutputFn fn;

TCHAR profileFilepath[260];
TCHAR edceScriptFilepath[260];
TCHAR edceJSONFilepath[260];
TCHAR defaultDirectory[260];
TCHAR edceDirectory[260];

bool foundProfile = false;

// Functions
void checkHR(HRESULT hr);
void txtFileCheck();
void getFilepaths();
void createTxtFile();
bool getFilePathName(bool isProfile);
void contactEDCE();


/*
	TODO:
	Get and save the current directory permanetly
	Get and save the directory of the edce script and save permanetly
*/


int main()
{
	// Initialize DirectOutput
	checkHR(fn.Initialize(L"EliteDangerousX52MFD"));

	// Registers and enumerates device
	fn.RegisterDevice();

	// Gets the enumerated device
	fn.GetDeviceType();

	// .txt file check
	txtFileCheck();

	// Set the profile
	if (foundProfile)
	{
		checkHR(fn.setDeviceProfile(profileFilepath));
	}
	else
	{
		cout << "Couldn't link a profile since one was not provided!\n";
	}
	
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

	// Main loop, run once aa minute
	bool isEliteRunning = true;
	LPCTSTR appName = TEXT("Elite Dangerous Launcher");
	do
	{
		contactEDCE();

		if (FindWindow(NULL, appName) == NULL)
		{
			isEliteRunning = false;
		}
		system("cls");
	} while (isEliteRunning);

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
void checkHR(HRESULT hr) 
{
	if (hr == S_OK)
	{
		cout << "DONE.\n";
	}
	else
	{
		cout << "FAILED/ hr = " << hr << endl;
	}
}

void txtFileCheck()
{
	// Create .txt file for filepaths to
	// Profile to use
	// edce_client.py
	cout << "\nLooking for txt file... ";
	char *filename = "EDX52Settings.txt";
	struct stat buffer;
	if (stat(filename, &buffer) == 0 )
	{
		cout << "FOUND.\nLoading filepaths for X52 profile and EDCE client.\n";
		foundProfile = true;
		ifstream myFile("EDX52Settings.txt");
		string line;
		int lineNumber = 0;
		size_t strSize;
		if (myFile.is_open())
		{
			while (getline(myFile, line))
			{
				cout << line << endl;
				switch (lineNumber)
				{
				case 0:
					strSize = line.length();
					for (size_t i = 0; i < strSize; i++)
					{
						profileFilepath[i] = line[i];
					}
					lineNumber++;
					break;
				case 1:
					strSize = line.length();
					for (size_t i = 0; i < strSize; i++)
					{
						edceScriptFilepath[i] = line[i];
					}
					lineNumber++;
					break;
				case 2:
					strSize = line.length();
					for (size_t i = 0; i < strSize; i++)
					{
						edceJSONFilepath[i] = line[i];
					}
					lineNumber++;
					break;
				case 3:
					strSize = line.length();
					for (size_t i = 0; i < strSize; i++)
					{
						defaultDirectory[i] = line[i];
					}
					lineNumber++;
					break;
				case 4:
					strSize = line.length();
					for (size_t i = 0; i < strSize; i++)
					{
						edceDirectory[i] = line[i];
					}
					break;
				default:
					break;
				}
			}
			myFile.close();
			cout << endl;
		}
	}
	else {
		// Couldn't find file. Need to create it
		// Get and save the filepaths
		getFilepaths();
		// Create the txt file with the filepaths
		createTxtFile();
		// Read in the newly created txt file
		txtFileCheck();
	}
}

void getFilepaths()
{
	// Get the current directory so it can be restored after the files have been found
	GetCurrentDirectory(MAX_PATH, defaultDirectory);

	// Get filepaths for the profile to be used, the location of the edce_client.py and create a filepath to last.json
	cout << "Couldn't find file. Creating file \"EDX52Settings.txt\"...\n\n";
	cout << "Please select your profile to use. This will allow use of pre-assigned keybindings, colors, settings, etc.\n";
	cout << "The default location is -> C:\\Users\\Public\\Public Documents\\SmartTechnology Profiles\n";
	if (getFilePathName(true))
	{
		cout << "Got profile filepath: ";
		wcout << profileFilepath << endl;
		foundProfile = true;
	}
	else
	{
		cout << "Couldn't get the profile or the operation was canceled.\n";
		foundProfile = false;
	}

	cout << "Please find the EDCE folder which contains edce_client.py.\n";
	if (getFilePathName(false))
	{
		cout << "Got EDCE script location: ";
		wcout << edceScriptFilepath << endl;

		// remove edce_client.py from previous selection and replace with last.json
		cout << "Creating filepath for last.json from the previous selection: ";
		// Copy to a temp char array
		char tempArray[sizeof(edceScriptFilepath)];
		for (size_t i = 0; i < sizeof(edceScriptFilepath); i++)
		{
			tempArray[i] = edceScriptFilepath[i];
		}
		PathRemoveFileSpecA(tempArray);
		size_t tempArraySize = strlen(tempArray) + 1;
		wchar_t *wcString = new wchar_t[tempArraySize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcString, tempArraySize, tempArray, _TRUNCATE);
		wcscpy_s(edceDirectory, wcString);
		delete[] wcString;

		strcat_s(tempArray, "\\last.json\0");
		tempArraySize = strlen(tempArray) + 1;
		wchar_t *wcStr = new wchar_t[tempArraySize];
		convertedChars = 0;
		mbstowcs_s(&convertedChars, wcStr, tempArraySize, tempArray, _TRUNCATE);
		wcscpy_s(edceJSONFilepath, wcStr);
		delete[] wcStr;
		wcout << edceJSONFilepath << endl;

		cout << "Got needed filepaths. Creating txt file to save these paths.\n\n";
	}
	else
	{
		cout << "Couldn't get the script location or the operation was canceled.\n\n";
	}
	// Restore the starting directory
	SetCurrentDirectory(defaultDirectory);
}

void createTxtFile() {
	// Create the txt file
	cout << "Creating txt file...";
	TCHAR currentDirectoy[MAX_PATH];
	TCHAR txtFile[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDirectoy);
	swprintf_s(txtFile, _T("%s\\EDX52Settings.txt"), currentDirectoy);
	HANDLE hFile = CreateFile(txtFile, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Add the filepaths
	cout << "DONE.\nWriting to txt file selected locations.\n";
	wofstream myFile;
	myFile.open("EDX52Settings.txt");
	myFile << profileFilepath << "\n";
	myFile << edceScriptFilepath << "\n";
	myFile << edceJSONFilepath << "\n";
	myFile << defaultDirectory << "\n";
	myFile << edceDirectory << "\n";
	myFile.close();
	cout << "Wrote to txt file.\n\n";
}


bool getFilePathName(bool isProfile)
{
	OPENFILENAME ofn;
	HWND hwnd = 0;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	if (isProfile)
	{
		ofn.lpstrFile = profileFilepath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(profileFilepath);
		ofn.lpstrFilter = _T("PR0 File (.pr0)\0*.pr0*\0\0");
	}
	else
	{
		ofn.lpstrFile = edceScriptFilepath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(edceScriptFilepath);
		ofn.lpstrFilter = _T("Python File (.py)\0*.py*\0\0");
	}

	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"C:\\";
	ofn.Flags = OFN_PATHMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == TRUE) {
		return true;
	}
	else
	{
		return false;
	}
}


void contactEDCE()
{
	// Do this once a minute and then update pages with relevant data
	// Need to set the directory from the script location
	SetCurrentDirectory(edceDirectory);
	
	// Run the script
	_wsystem(edceScriptFilepath);
	cout << "Cooldown: 2.5 minutes\n";
	// Wait for 2.5 minutes
	using namespace std::this_thread;
	using namespace std::chrono;
	int timer = 150;
	while (timer != 0)
	{
		sleep_until(system_clock::now() + 1s);
		timer--;
		cout << timer << "..";
	}
	cout << endl;

	// Restore working directory
	SetCurrentDirectory(defaultDirectory);
}
