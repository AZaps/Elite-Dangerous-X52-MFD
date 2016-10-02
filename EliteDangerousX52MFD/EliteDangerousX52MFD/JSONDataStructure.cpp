// JSONDataStructure.cpp
#include "stdafx.h"
#include "JSONDataStructure.h"
#include "DepInclude\json.hpp"
#include <fstream>

using json = nlohmann::json;
using namespace std;

/*
	PARAMETERS: TCHAR *jsonDirectory -> Filepath location of the folder where the json file is located
				TCHAR *defaultDirectory -> Folder location of this project
				TCHAR *jsonFilepath -> Exact file location of the json file
	RETURNS: none

	FUNCTION: Entry point for this file to handle page updates
*/
void JSONDataStructure::readStoreJSON(TCHAR *jsonDirectory, TCHAR *defaultDirectory, TCHAR *jsonFilepath) {
	SetCurrentDirectory(jsonDirectory);
	setPage0(jsonFilepath);
	setPage1(jsonFilepath);
	setPage2(jsonFilepath);
	setPage3(jsonFilepath);
	SetCurrentDirectory(defaultDirectory);
}

/*
	PARAMETERS:	TCHAR *jsonDirectory -> Filepath location of the folder where the json file is located
				TCHAR *defaultDirectory -> Folder location of this project
				TCHAR *jsonFilepath -> Exact file location of the json file
	RETURNS: none

	FUNCTION: Will determine the current page by the handle to the DirectOutputFn object and update that page on the MFD. This is so the page doesn't have to be manually refreshed by the user every polling update
*/

void JSONDataStructure::updateCurrentPage(TCHAR *jsonDirectory, TCHAR *defaultDirectory, TCHAR *jsonFilepath, DirectOutputFn& fnJSON)
{
	SetCurrentDirectory(jsonDirectory);

	ifstream cmdrDataFile(jsonFilepath);
	json cmdrData(cmdrDataFile);
	int length = 32;
	int currentPage = fnJSON.getCurrentPage();

	// Get current page
	switch (currentPage)
	{
	case 0:
		fnJSON.setString(0, 0, pg0.cmdrPage0Info[pg0.currentLine]);
		fnJSON.setString(0, 1, pg0.cmdrPage0Info[pg0.currentLine + 1]);
		fnJSON.setString(0, 2, pg0.cmdrPage0Info[pg0.currentLine + 2]);
		break;
	case 1:
		fnJSON.setString(1, 0, pg1.cmdrPage1Info[pg1.currentLine]);
		fnJSON.setString(1, 1, pg1.cmdrPage1Info[pg1.currentLine + 1]);
		fnJSON.setString(1, 2, pg1.cmdrPage1Info[pg1.currentLine + 2]);
		break;
	case 2:
		fnJSON.setString(2, 0, pg2.cmdrPage2Info[0]);
		fnJSON.setString(2, 1, pg2.cmdrPage2Info[1]);
		fnJSON.setString(2, 2, pg2.cmdrPage2Info[2]);
		break;
	case 3:
		wchar_t str0[32];
		wchar_t str1[32];
		wchar_t str2[32];
		wcsncpy_s(str0, pg3.cmdrPage3Info.at(pg3.currentLine).c_str(), 32);
		wcsncpy_s(str1, pg3.cmdrPage3Info.at(pg3.currentLine + 1).c_str(), 32);
		wcsncpy_s(str2, pg3.cmdrPage3Info.at(pg3.currentLine + 2).c_str(), 32);
		fnJSON.setString(3, 0, str0);
		fnJSON.setString(3, 1, str1);
		fnJSON.setString(3, 2, str2);
		break;
	default:
		break;
	}

	SetCurrentDirectory(defaultDirectory);
}

/*
	PARAMETERS: TCHAR *jsonFilepath -> location of the json file so the data can be read from it and stored locally to update on the controller
	RETURNS: none

	FUNCTION: Reads the json file and updates the relevant page information listed below. Uses null checking on the json file so if the current key value doesn't exist it won't crash upon trying to read a non existant key value. 
*/
/*
	Page 0 consists of:
	Greetings CMDR
	(CMDR NAME)
	CR: (CREDIT BALANCE)
	(CURRENT SHIP)
	Combat: (COMBAT_RANK)
	Trade: (TRADE_RANK)
	Exploration: (EXPLORATION_RANK)
	CQC: (CQC_RANK)
	Federation: (FEDERATION_RANK)
	Empire: (EMPIRE_RANK)
*/
void JSONDataStructure::setPage0(TCHAR *jsonFilepath)
{
	ifstream cmdrDataFile(jsonFilepath);
	json cmdrData(cmdrDataFile);
	int length = 32;

	if (isFirstTime) {
		pg0.currentLine = 0;
	}
	wcsncpy_s(pg0.cmdrPage0Info[0], L"Greetings CMDR", length);

	// Key value null checking. Returns true if it doesn't exist
	if (cmdrData["commander"]["name"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[1], strToWStr(cmdrData["commander"]["name"]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[1], strToWStr("No Name").c_str(), length);
	}

	if (cmdrData["commander"]["credits"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[2], strToWStr("CR: " + to_string((long)cmdrData["commander"]["credits"])).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[2], strToWStr("No Credits").c_str(), length);
	}
	
	if (cmdrData["ships"][0]["name"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[3], strToWStr(cmdrData["ships"][0]["name"]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[3], strToWStr("No Ship").c_str(), length);
	}
	
	if (cmdrData["commander"]["rank"]["combat"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[4], strToWStr("Combat: " + combatRank[cmdrData["commander"]["rank"]["combat"]]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[4], strToWStr("No Combat").c_str(), length);
	}

	if (cmdrData["commander"]["rank"]["trade"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[5], strToWStr("Trade: " + tradeRank[cmdrData["commander"]["rank"]["trade"]]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[5], strToWStr("No Trade").c_str(), length);
	}

	if (cmdrData["commander"]["rank"]["explore"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[6], strToWStr("Exploration: " + explorerRank[cmdrData["commander"]["rank"]["explore"]]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[6], strToWStr("No Exploration").c_str(), length);
	}

	if (cmdrData["commander"]["rank"]["cqc"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[7], strToWStr("CQC: " + cqcRank[cmdrData["commander"]["rank"]["cqc"]]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[7], strToWStr("CQC: No Rank").c_str(), length);
	}

	if (cmdrData["commander"]["rank"]["federation"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[8], strToWStr("Federation: " + federationRank[cmdrData["commander"]["rank"]["federation"]]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[8], strToWStr("No Federation").c_str(), length);
	}

	if (cmdrData["commander"]["rank"]["empire"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[9], strToWStr("Empire: " + empireRank[cmdrData["commander"]["rank"]["empire"]]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[9], strToWStr("No Empire").c_str(), length);
	}
}

/*
	PARAMETERS: TCHAR *jsonFilepath -> location of the json file so the data can be read from it and stored locally to update on the controller
	RETURNS: none

	FUNCTION: Reads the json file and updates the relevant page information listed below. Uses null checking on the json file so if the current key value doesn't exist it won't crash upon trying to read a non existant key value.
*/
/*
	Page 1 consists of:
	Ship Details
	(CURRENT_SHIP_NAME)
	Cargo: (CURRENT / TOTAL_CARGO_CAPACITY)
	MainTank: (CURRENT / TOTAL_MAIN_FUEL_LEVEL)
	ResTank: (CURRENT / TOTAL_RESERVE_TANK)
	Oxygen: (TIME_REMAINING MINUTES:SECONDS)
*/
void JSONDataStructure::setPage1(TCHAR * jsonFilepath)
{
	ifstream cmdrDataFile(jsonFilepath);
	json cmdrData(cmdrDataFile);
	int length = 32;

	if (isFirstTime) {
		pg1.currentLine = 0;
	}
	wcsncpy_s(pg1.cmdrPage1Info[0], L"Ship Details", length);

	if (cmdrData["ships"][0]["name"].is_null() != true)
	{
		wcsncpy_s(pg1.cmdrPage1Info[1], strToWStr(cmdrData["ships"][0]["name"]).c_str(), length);
	}

	if (cmdrData["ship"]["cargo"]["capacity"].is_null() != true)
	{
		wcsncpy_s(pg1.cmdrPage1Info[2], strToWStr("Cargo: " + to_string((int)cmdrData["ship"]["cargo"]["qty"]) + "/" + to_string((int)cmdrData["ship"]["cargo"]["capacity"])).c_str(), length);

	}
	else
	{
		wcsncpy_s(pg1.cmdrPage1Info[2], strToWStr("Cargo: No Module").c_str(), length);
	}
	
	// Main fuel two decimal place precision conversion
	if (cmdrData["ship"]["fuel"]["main"]["level"].is_null() != true)
	{
		double currentMainLevel = cmdrData["ship"]["fuel"]["main"]["level"];
		cout.flush();
		stringstream mainlevelStream;
		mainlevelStream << fixed << setprecision(2) << currentMainLevel;
		wcsncpy_s(pg1.cmdrPage1Info[3], strToWStr("MainT: " + mainlevelStream.str() + "/" + to_string((int)cmdrData["ship"]["fuel"]["main"]["capacity"])).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg1.cmdrPage1Info[3], strToWStr("No Fuel").c_str(), length);
	}

	// Reserve fuel two decimal place precision conversion
	if (cmdrData["ship"]["fuel"]["reserve"]["capacity"].is_null() != true)
	{
		double currentResLevel = cmdrData["ship"]["fuel"]["reserve"]["level"];
		double totalResLevel = cmdrData["ship"]["fuel"]["reserve"]["capacity"];
		cout.flush();
		stringstream currentResStream;
		stringstream totalResStream;
		currentResStream << fixed << setprecision(2) << currentResLevel;
		totalResStream << fixed << setprecision(2) << totalResLevel;
		wcsncpy_s(pg1.cmdrPage1Info[4], strToWStr("ResT: " + currentResStream.str() + "/" + totalResStream.str()).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg1.cmdrPage1Info[4], strToWStr("No Reserve").c_str(), length);
	}

	// Oxygen remaining minutes:seconds
	if (cmdrData["ship"]["oxygenRemaining"].is_null() != true)
	{
		long milliOxygenRemaining = cmdrData["ship"]["oxygenRemaining"];
		long seconds = (milliOxygenRemaining / 1000) % 60;
		long minutes = (milliOxygenRemaining / 60000) % 60;
		wcsncpy_s(pg1.cmdrPage1Info[5], strToWStr("Oxygen: " + to_string(minutes) + ":" + to_string(seconds)).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg1.cmdrPage1Info[5], strToWStr("No Oxygen").c_str(), length);
	}
}

/*
	PARAMETERS: TCHAR *jsonFilepath -> location of the json file so the data can be read from it and stored locally to update on the controller
	RETURNS: none

	FUNCTION: Reads the json file and updates the relevant page information listed below. Uses null checking on the json file so if the current key value doesn't exist it won't crash upon trying to read a non existant key value.
*/
/*
	Page 2 consists of:
	(CURRENT_SYSTEM)
	(CURRENT_STATION) if possible
	(CURRENT_PLANET) if landed on one and not on station
*/
void JSONDataStructure::setPage2(TCHAR * jsonFilePath)
{
	ifstream cmdrDataFile(jsonFilePath);
	json cmdrData(cmdrDataFile);
	int length = 32;
	
	// Get currentShipID to determine current location of CMDR
	if (cmdrData["commander"]["currentShipId"].is_null() != true)
	{
		int shipID = cmdrData["commander"]["currentShipId"];
		if (cmdrData["ships"].is_null() != true)
		{
			int amountOfShips = cmdrData["ships"].size();
			int selectedShipID;
			for (size_t i = 0; i < amountOfShips; i++)
			{
				selectedShipID = cmdrData["ships"][i]["id"];
				if (selectedShipID == shipID)
				{
					// System
					if (cmdrData["ships"][i]["starsystem"]["name"].is_null() != true) 
					{
						wcsncpy_s(pg2.cmdrPage2Info[0], strToWStr(cmdrData["ships"][i]["starsystem"]["name"]).c_str(), length);
					}
					else
					{
						// Get the last visited system name because the planet name can't be retrieved if on the surface
						wcsncpy_s(pg2.cmdrPage2Info[0], strToWStr(cmdrData["lastSystem"]["name"]).c_str(), length);
					}
					// Station
					if (cmdrData["ships"][i]["station"].is_null() != true)
					{
						wcsncpy_s(pg2.cmdrPage2Info[1], strToWStr(cmdrData["ships"][i]["station"]["name"]).c_str(), length);
						wcsncpy_s(pg2.cmdrPage2Info[2], strToWStr("").c_str(), length);
					}
					else
					{
						wcsncpy_s(pg2.cmdrPage2Info[1], strToWStr("On surface or").c_str(), length);
						wcsncpy_s(pg2.cmdrPage2Info[2], strToWStr("in space").c_str(), length);
					}
				}
			}
		}
	}
}

/*
	PARAMETERS: TCHAR *jsonFilepath -> location of the json file so the data can be read from it and stored locally to update on the controller
	RETURNS: none

	FUNCTION: Reads the json file and updates the relevant page information listed below. Uses null checking on the json file so if the current key value doesn't exist it won't crash upon trying to read a non existant key value.
*/
/*
	Page 3 consists of: FOR TOTAL AMOUNT OF SHIPS STATIONED
	All Ships
	(SHIP_NAME)
	(STARSYSTEM)
	(STATION)
*/
void JSONDataStructure::setPage3(TCHAR * jsonFilePath)
{
	ifstream cmdrDataFile(jsonFilePath);
	json cmdrData(cmdrDataFile);
	int length = 32;
	pg3.cmdrPage3Info.clear();

	if (isFirstTime) {
		pg3.currentLine = 0;
	}

	// Get currentShipID to determine current location of CMDR
	if (cmdrData["commander"]["currentShipId"].is_null() != true)
	{
		int shipID = cmdrData["commander"]["currentShipId"];
		if (cmdrData["ships"].is_null() != true)
		{
			pg3.amountOfShips = cmdrData["ships"].size();

			// Change array size to equal amount of ships * 3, since it'll be (SHIP, STARSYSTEM, STATION)
			pg3.cmdrPage3Info.resize(pg3.amountOfShips * 3);
			int i = 0;
			while (i != pg3.amountOfShips)
			{
				if (cmdrData["ships"][i]["name"].is_null() != true)
				{
					pg3.cmdrPage3Info.insert(pg3.cmdrPage3Info.begin() + (i * 3), strToWStr(cmdrData["ships"][i]["name"]).c_str());
				}

				if (cmdrData["ships"][i]["starsystem"]["name"].is_null() != true)
				{
					pg3.cmdrPage3Info.insert(pg3.cmdrPage3Info.begin() + (i * 3 + 1), strToWStr(cmdrData["ships"][i]["starsystem"]["name"]).c_str());
				}
				else
				{
					pg3.cmdrPage3Info.insert(pg3.cmdrPage3Info.begin() + (i * 3 + 1), strToWStr(cmdrData["lastSystem"]["name"]).c_str());
				}

				if (cmdrData["ships"][i]["station"]["name"].is_null() != true)
				{
					pg3.cmdrPage3Info.insert(pg3.cmdrPage3Info.begin() + (i * 3 + 2), strToWStr(cmdrData["ships"][i]["station"]["name"]).c_str());
				}
				else
				{
					pg3.cmdrPage3Info.insert(pg3.cmdrPage3Info.begin() + (i * 3 + 2), strToWStr("Surface or Space").c_str());
				}
				
				i++;
			}
		}
	}
}

/*
	PARAMETERS: string str -> string value
	RETURNS: wstring

	FUNCTION: Takes a string value(from the json file) and converts it to a wstring. Upon returning from this function the wstring value will be copied into the relevant page array. Usually as a wchar_t since the printing function to the controller requires a wchar_t type.
*/
std::wstring JSONDataStructure::strToWStr(std::string str)
{
	wstring wStr = wstring(str.begin(), str.end());
	return wStr;
}