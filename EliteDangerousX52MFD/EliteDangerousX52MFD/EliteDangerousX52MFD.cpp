// EliteDangerousX52MFD.cpp : Entry point

/*
	EliteDangerousX52MFD v 1.1.2
	Special Thanks to:
		Frontier Developments for Elite Dangerous
		Saitek for the use and development of the SDK to run this project
		Niels Lohmann for work on JSON for Modern C++ (https://github.com/nlohmann/json)
*/

#include "stdafx.h"
#include <ShlObj.h>
#include "DirectOutputFn.h"
#include "JSONDataStructure.h"

using namespace std;

// DirectOutput function object
// Creation of this object automatically loads in DirectOutput but it still needs to be initialized
DirectOutputFn fn;

// Accessor to JSON file
JSONDataStructure jsonDataClass;

// Text filepath holders
TCHAR profileFilepath[260];
TCHAR defaultDirectory[260];
TCHAR journalFolderpath[260];
TCHAR currentJournal[260];

// Instance checking
bool foundProfile = false;
bool closeOnWindowX = false;

// Internal Functions
void checkHR(HRESULT hr);
void txtFileCheck();
void getFilepaths();
void createTxtFile();
bool getFilePathName();
void determineJournalFilepath();
BOOL determineWriteTime();
void readJournal();
void waitForJournalUpdate();
void cleanupAndClose();
BOOL controlHandler(DWORD fdwCtrlType);

int main()
{
	// Alert version number
	cout << "\n---- EliteDangerousX52MFD  v 1.1.2 ----\n\n";

	// Setup control handling, if app is closed by other means. (Ctrl + C or hitting the 'X' button)
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)controlHandler, TRUE);

	// Create map to perform functions based on the event input
	jsonDataClass.createMap();

	// Initialize DirectOutput
	checkHR(fn.Initialize(L"EliteDangerousX52MFD"));

	// Registers and enumerates device
	fn.RegisterDevice();

	// Gets the enumerated device
	if (!fn.GetDeviceType())
	{
		cout << "\nPress ENTER to close this application.";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		return 0;
	}

	// .txt file check
	txtFileCheck();

	// Set the profile, if provided
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
	for (int i = 0; i < 3; i++)
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

	// Determine if journal folder path exists
	if (journalFolderpath[0] == _T('\0'))
	{
		cout << "\nCan't find the journal folder." << endl;
		cout << "Please delete EDX52Settings.txt and restart this application" << endl;
		cout << "Press enter to cleanup and quit..." << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cleanupAndClose();
		return 0;
	}

	// Determine initial file name of the Journal
	determineJournalFilepath();

	// Determine if the current journal found could be the latest
	if (!determineWriteTime())
	{
		cout << "File was created more than 10 minutes ago...Probably not the latest journal.\n";
		cout << "Waiting for the latest journal to be created\n";
		waitForJournalUpdate();
		cout << "Got latest Journal." << endl;
	}

	// Start first file reading from first line
	jsonDataClass.readLine = 0;

	// First file read, set continue action to false until another file is created
	jsonDataClass.continueEvent = false;

	// Set all line numbers to 0
	jsonDataClass.pg0.currentLine = 0;
	jsonDataClass.pg1.currentLine = 0;
	jsonDataClass.pg2.currentLine = 0;

	// Main loop
	do
	{
		// Read line by line of the journal to display new information
		readJournal();

		// When reading the journal and an event of 'Continue' is found, a new file is generated and it needs to be found.
		if (jsonDataClass.continueEvent)
		{
			waitForJournalUpdate();
			jsonDataClass.continueEvent = false;
			jsonDataClass.readLine = 0;
		}
		else
		{
			// Haven't found a continue event, wait until new information is shown
			waitForJournalUpdate();
		}

	} while (!closeOnWindowX);

	// No cleanup neccessary as it is done when the window closes.

	return 0;
}

/*
	PARAMETERS: HRESULT hr == some functions from DirectOutput return a HRESULT value, this just checks if it pass/fail and ouputs to the console
	RETURNS: none

	FUNCTION: Checks result of the function if it returns an HRESULT value

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

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: Looks for the existance of the text file in the project directory if it can't be found, prompt the user to select various file locations. Otherwise, load in the required filepaths
*/
void txtFileCheck()
{
	// Create .txt file for filepaths
	cout << "\nLooking for text file... ";
	char *filename = "EDX52Settings.txt";
	struct stat buffer;
	if (stat(filename, &buffer) == 0)
	{
		cout << "FOUND.\nLoading filepaths.\n";
		foundProfile = true;
		ifstream myFile("EDX52Settings.txt");
		string line;
		int lineNumber = 0;
		size_t strSize;
		// Load in each of the required filepaths per line
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
						defaultDirectory[i] = line[i];
					}
					lineNumber++;
					break;
				case 2:
					strSize = line.length();
					for (size_t i = 0; i < strSize; i++)
					{
						journalFolderpath[i] = line[i];
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

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: Prompts the user to select the filepaths corresponding to the profile location and folder of the Journals
*/
void getFilepaths()
{
	// Get the current directory so it can be restored after the files have been found
	GetCurrentDirectory(MAX_PATH, defaultDirectory);

	// Get filepaths for the profile to be used
	cout << "Couldn't find file. Creating file \"EDX52Settings.txt\"...\n\n";
	cout << "Please select your profile to use. This will allow use of pre-assigned keybindings, colors, settings, etc.\n";
	cout << "The default location of the profiles under Saitek is -> C:\\Users\\Public\\Public Documents\\SmartTechnology Profiles\n";
	cout << "The default location of the profiles under Logitech is -> C:\\Users\\Public\\Public Documents\\Logitech\\X52 Professional\n";
	if (getFilePathName())
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

	// Get folder location of the Journals
	cout << "\nPlease select the folder of the Journal files.\n";
	cout << "The default location is -> C:\\Users\\User Name\\Saved Games\\Frontier Developments\\Elite Dangerous\\\n";

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;
		// FileOpenDialog object
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileDialog, reinterpret_cast<void**>(&pFileOpen));
		if (SUCCEEDED(hr))
		{
			// Select folder only
			DWORD dwOptions;
			if (SUCCEEDED(pFileOpen->GetOptions(&dwOptions)))
			{
				pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
			}
			// Show Open dialog box
			hr = pFileOpen->Show(NULL);

			// Get the filename from the dialog box
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					pItem->Release();

					// Copy folder filepath to global variable so it can be saved to the text file for later retrieval
					_tcscpy_s(journalFolderpath, pszFilePath);
				}
			}
			else
			{
				cout << "Couldn't get the folder path or operation was cancelled.\n";
				cout << "Close the program, delete the text file, and repeat this opertaion while finding the folder path.\n";
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	SetCurrentDirectory(defaultDirectory);
}

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: Creates the text file with the provided filepaths. Each path is separated by a new line so it can easily be read in the next time the application is run.
*/
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
	myFile << defaultDirectory << "\n";
	myFile << journalFolderpath << "\n";
	myFile.close();
	cout << "Wrote to txt file.\n\n";
}


/*
	PARAMETERS: none
	RETURNS: bool value -> alerts the program if the filepaths are correct and can be opened successfully. Otherwise, the program will alert the user that the operation was cancelled or a filepath wasn't selected

	FUNCTION: Prompts the user to select the required filepath of the HOTAS profile
*/
bool getFilePathName()
{
	OPENFILENAME ofn;
	HWND hwnd = 0;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;

	ofn.lpstrFile = profileFilepath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(profileFilepath);
	ofn.lpstrFilter = _T("PR0 File (.pr0)\0*.pr0*\0\0");

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

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: Gets and sets the latest written/saved to journal. Will be called when needing to get a new journal file either at the beginning of playtime or when a new part is needed.
*/
void determineJournalFilepath()
{
	// Check directory for latest created files
	TCHAR tempCurrentFile[260];
	StringCchCopy(tempCurrentFile, MAX_PATH, journalFolderpath);
	StringCchCat(tempCurrentFile, MAX_PATH, TEXT("\\*log"));

	WIN32_FIND_DATA currentFileData;
	HANDLE file = FindFirstFile(tempCurrentFile, &currentFileData);

	struct FileInfo
	{
		HANDLE h;
		WIN32_FIND_DATA data;
	} newestFile;

	// Determine the latest modified file within the Journal folder
	if (file != INVALID_HANDLE_VALUE)
	{
		newestFile.h = file;
		newestFile.data = currentFileData;

		while (FindNextFile(file, &currentFileData))
		{
			if (CompareFileTime(&currentFileData.ftLastWriteTime, &newestFile.data.ftLastWriteTime) > 0)
			{
				newestFile.h = file;
				newestFile.data = currentFileData;
			}
		}
		StringCchCopy(currentJournal, MAX_PATH, journalFolderpath);
		StringCchCat(currentJournal, MAX_PATH, TEXT("\\"));
		StringCchCat(currentJournal, MAX_PATH, newestFile.data.cFileName);
		FindClose(file);
	}
}

/*
	PARAMETERS: none
	RETURNS: bool value -> true if the file was created in the last 10 minutes, false if not

	FUNCTION: This will be used to get a rough estimate of when the latest journal was created. I would think anything longer than 10 minutes ago would indicate a journal relating to a different play session.
*/
BOOL determineWriteTime()
{
	// Do time comparison
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	HANDLE hFile = CreateFile(currentJournal, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
		printf("\nGetFileTime failed.\n");
		cout << "No journals detected. Please run Elite Dangerous at least once and then restart this program." << endl;
		ExitProcess(GetLastError());
	}

	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	CloseHandle(hFile);

	// Determine if created within the last 10 minutes
	SYSTEMTIME currentLocal;
	union ftTime { FILETIME ft; ULARGE_INTEGER uli; };

	ftTime t1;
	ftTime t2;

	GetLocalTime(&currentLocal);

	SystemTimeToFileTime(&currentLocal, &t1.ft);
	SystemTimeToFileTime(&stLocal, &t2.ft);

	__int64 diff;
	diff = t1.uli.QuadPart - t2.uli.QuadPart;
	diff /= 600000000; // To minutes

	// Less than 10 minute check
	if (diff < 10)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: Reads the currently selected journal and updates passes the selected line to the JSON file to change to a JSON object where the event can be determined.
*/
void readJournal()
{
	// Read events from the journal line by line and pass to JSONDataStructure to process until end of file
	ifstream cmdrDataFile(currentJournal);
	if (cmdrDataFile.is_open())
	{
		string line;
		unsigned int tempLine = 0;
		while (getline(cmdrDataFile, line))
		{
			if (tempLine == jsonDataClass.readLine)
			{
				jsonDataClass.readStoreJSON(line);
				jsonDataClass.readLine++;
			}
			tempLine++;
		}
	}

	cmdrDataFile.close();

	// Update the page to the most relevant data
	fn.updatePage(fn.getCurrentPage());
}

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: This function will set a associated to when a file changes in the directory. This is useful so the program won't continually read the journal hogging resources. It will wait until there is a change in information.
*/
void waitForJournalUpdate()
{
	DWORD dwWaitStatus;
	HANDLE dwChangeHandle[2];

	// Watch for new Journal creations
	dwChangeHandle[0] = FindFirstChangeNotification(journalFolderpath, FALSE, FILE_NOTIFY_CHANGE_FILE_NAME);
	if (dwChangeHandle[0] == INVALID_HANDLE_VALUE)
	{
		printf("\nFindFirstChangeNotification failed.\n");
		FindCloseChangeNotification(dwChangeHandle[0]);
		ExitProcess(GetLastError());
	}

	// Validity check, make sure the handle notification was set correctly
	if (dwChangeHandle[0] == NULL || dwChangeHandle[1] == NULL)
	{
		printf("\nUnexpected NULL FindFirstChangeNotification\n");
		ExitProcess(GetLastError());
	}

	// Open up the journal in a stream for file size checking
	long prevPos;
	if (currentJournal[0] != _T('\0'))
	{
		ifstream journalFile(currentJournal, ios::binary | ios::ate);
		prevPos = journalFile.tellg();
		journalFile.close();
	}

	// Waiting and detecting changes
	bool changeDetected = false;
	while (!changeDetected)
	{
		dwWaitStatus = WaitForSingleObject(dwChangeHandle[0], 1000);
		switch (dwWaitStatus)
		{
		/*
			FILE_NOTIFY_CHANGE_FILE_NAME
			Runs when a file is added to the directory. I'll be using this if the player exits to the main menu and restarts the game.
			A new journal should be created.
		*/
		case WAIT_OBJECT_0:
			// Write to file was detected
			changeDetected = true;
			if (FindNextChangeNotification(dwChangeHandle[0]) == FALSE)
			{
				printf("\nFindFirstChangeNotification failed.\n");
				ExitProcess(GetLastError());
			}
			determineJournalFilepath();
			break;

		/*
			After doing some research on the forum on the journal this is the method I have to use to determine if the file has been modified.
			I'll have to check to see if the file size has changed once every second and if so there has been an update.

			Apparently they are writing to the file by flushing the stream and I have no idea how to detect that.
			Previously I would use a wait object above but for file writes and it wasn't being detected unless I manually back out and enter the journal folder.
		*/
		case WAIT_TIMEOUT:
			if (currentJournal[0] != _T('\0'))
			{
				ifstream journalFile(currentJournal, ios::binary | ios::ate);
				long newPos = journalFile.tellg();
				journalFile.close();
				if (newPos > prevPos)
				{
					changeDetected = true;
				}
			}
			break;

		default:
			printf("\nUnhandled dwWaitStatus\n");
			ExitProcess(GetLastError());
			break;
		}
	}
	CloseHandle(dwChangeHandle[0]);
}

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: Simplifies all cleanup into one function so the actions happen back to back allowing in easier exiting
*/
void cleanupAndClose() {
	//Unregister the callbacks
	checkHR(fn.unRegisterSoftBtnCallback());
	checkHR(fn.unRegisterPageCallback());

	// Deinitialize DirectOutput
	checkHR(fn.Deinitialize());
}

/*
	PARAMETERS: DWORD fdwCtrlType -> The type of action performed by the user on the console window or on system level
	RETURNS: bool condition -> Will alert the program if a certain close event has occured, so proper cleanup can be done

	FUNCTION: Case statements for determining the type of event invoked by the user on the console window
*/
BOOL controlHandler(DWORD fdwCtrlType) {
	switch (fdwCtrlType)
	{
		// Handle the CTRL-C signal. 
	case CTRL_C_EVENT:
		closeOnWindowX = true;
		cleanupAndClose();
		return(TRUE);

		// CTRL-CLOSE: confirm that the user wants to exit on 'X' button click on window. 
	case CTRL_CLOSE_EVENT:
		closeOnWindowX = true;
		cleanupAndClose();
		return(TRUE);

		// Pass other signals to the next handler. 
	case CTRL_BREAK_EVENT:
		return FALSE;

	case CTRL_LOGOFF_EVENT:
		return FALSE;

	case CTRL_SHUTDOWN_EVENT:
		return FALSE;

	default:
		return FALSE;
	}
}