// JSONDataStructure.cpp
#include "stdafx.h"
#include "JSONDataStructure.h"
#include "../json.hpp"
#include <fstream>

using json = nlohmann::json;
using namespace std;

void JSONDataStructure::readStoreJSON(TCHAR *jsonDirectory, TCHAR *defaultDirectory, TCHAR *jsonFilepath) {
	// If the station name hasn't changed, the player most likely hasn't left yet so no reason to update
	SetCurrentDirectory(jsonDirectory);
	setPage0(jsonFilepath);
	setPage1(jsonFilepath);
	SetCurrentDirectory(defaultDirectory);
}

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
	std::ifstream cmdrDataFile(jsonFilepath);
	json cmdrData(cmdrDataFile);
	int length = 32;

	pg0.currentLine = 0;
	wcsncpy_s(pg0.cmdrPage0Info[0], L"Greetings CMDR", length);
	wcsncpy_s(pg0.cmdrPage0Info[1], strToWStr(cmdrData["commander"]["name"]).c_str(), length);
	wcsncpy_s(pg0.cmdrPage0Info[2], strToWStr("CR: " + to_string((long)cmdrData["commander"]["credits"])).c_str(), length);
	wcsncpy_s(pg0.cmdrPage0Info[3], strToWStr(cmdrData["ships"][0]["name"]).c_str(), length);
	wcsncpy_s(pg0.cmdrPage0Info[4], strToWStr("Combat: " + combatRank[cmdrData["commander"]["rank"]["combat"]]).c_str() , length);
	wcsncpy_s(pg0.cmdrPage0Info[5], strToWStr("Trade: " + tradeRank[cmdrData["commander"]["rank"]["trade"]]).c_str(), length);
	wcsncpy_s(pg0.cmdrPage0Info[6], strToWStr("Exploration: " + explorerRank[cmdrData["commander"]["rank"]["explore"]]).c_str(), length);
	wcsncpy_s(pg0.cmdrPage0Info[7], strToWStr("CQC: " + cqcRank[cmdrData["commander"]["rank"]["cqc"]]).c_str(), length);
	wcsncpy_s(pg0.cmdrPage0Info[8], strToWStr("Federation: " + federationRank[cmdrData["commander"]["rank"]["federation"]]).c_str(), length);
	wcsncpy_s(pg0.cmdrPage0Info[9], strToWStr("Empire: " + empireRank[cmdrData["commander"]["rank"]["empire"]]).c_str(), length);
}

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
	std::ifstream cmdrDataFile(jsonFilepath);
	json cmdrData(cmdrDataFile);
	int length = 32;

	pg1.currentLine = 0;
	wcsncpy_s(pg1.cmdrPage1Info[0], L"Ship Details", length);
	wcsncpy_s(pg1.cmdrPage1Info[1], strToWStr(cmdrData["ships"][0]["name"]).c_str(), length);
	wcsncpy_s(pg1.cmdrPage1Info[2], strToWStr("Cargo: " + to_string((int)cmdrData["ship"]["cargo"]["qty"]) + "/" + to_string((int)cmdrData["ship"]["cargo"]["capacity"])).c_str(), length);
	
	// Main fuel two decimal place precision conversion
	double currentMainLevel = cmdrData["ship"]["fuel"]["main"]["level"];
	cout.flush();
	stringstream mainlevelStream;
	mainlevelStream << fixed << setprecision(2) << currentMainLevel;
	wcsncpy_s(pg1.cmdrPage1Info[3], strToWStr("MainT: " + mainlevelStream.str() + "/" + to_string((int)cmdrData["ship"]["fuel"]["main"]["capacity"])).c_str(), length);
	
	// Reserve fuel two decimal place precision conversion
	double currentResLevel = cmdrData["ship"]["fuel"]["reserve"]["level"];
	double totalResLevel = cmdrData["ship"]["fuel"]["reserve"]["capacity"];
	cout.flush();
	stringstream currentResStream;
	stringstream totalResStream;
	currentResStream << fixed << setprecision(2) << currentResLevel;
	totalResStream << fixed << setprecision(2) << totalResLevel;
	wcsncpy_s(pg1.cmdrPage1Info[4], strToWStr("ResT: " + currentResStream.str() + "/" + totalResStream.str()).c_str(), length);

	// Oxygen remaining minutes:seconds
	long milliOxygenRemaining = cmdrData["ship"]["oxygenRemaining"];
	long seconds = (milliOxygenRemaining / 1000) % 60;
	long minutes = (milliOxygenRemaining / 60000) % 60;
	wcsncpy_s(pg1.cmdrPage1Info[5], strToWStr("Oxygen: " + to_string(minutes) + ":" + to_string(seconds)).c_str(), length);
}

/*
	Page 2 consists of:
	(CURRENT_SYSTEM)
	(CURRENT_STATION) if possible
	(CURRENT_PLANET) if landed on one and not on station
*/
void JSONDataStructure::setPage2(TCHAR * jsonFilePath)
{
}

/*
	Page 3 consists of: FOR TOTAL AMOUNT OF SHIPS STATIONED
	Docked Ships
	(SHIP_NAME)
	(STARSYSTEM)
	(STATION)
*/
void JSONDataStructure::setPage3(TCHAR * jsonFilePath)
{
}


std::wstring JSONDataStructure::strToWStr(std::string str)
{
	wstring wStr = wstring(str.begin(), str.end());
	return wStr;
}
