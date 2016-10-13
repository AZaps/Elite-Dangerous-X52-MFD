#pragma once
#include "DirectOutputFn.h"
#include <unordered_map>
#include "DepInclude\json.hpp"

class JSONDataStructure
{
	typedef std::vector<std::wstring> v_strList;

public:

	JSONDataStructure();
	~JSONDataStructure();
	void readStoreJSON(TCHAR *jsonDirectory, TCHAR *defaultDirectory, TCHAR *jsonFilepath);
	void updateCurrentPage(TCHAR *jsonDirectory, TCHAR *defaultDirectory, TCHAR *jsonFilepath, DirectOutputFn& fnJSON);

	bool isFirstTime;
	bool continueEvent;

	struct mfdPage0
	{
		int currentLine;
		wchar_t cmdrPage0Info[10][32];
	}; mfdPage0 pg0;

	struct mfdPage1
	{
		int currentLine;
		wchar_t cmdrPage1Info[6][32];
	}; mfdPage1 pg1;

	struct mfdPage2
	{
		wchar_t cmdrPage2Info[3][32];
	}; mfdPage2 pg2;

	struct mfdPage3
	{
		int currentLine;
		int amountOfShips;
		v_strList cmdrPage3Info;
	}; mfdPage3 pg3;

	// Ranks are listed on 11.1 Ranks of Journal Manual
	std::string combatRank[9] = {
		"Harmless",
		"Mostly Harmless",
		"Novice",
		"Competent",
		"Expert",
		"Master",
		"Dangerous",
		"Deadly",
		"Elite"
	};
	std::string tradeRank[9] = {
		"Penniless",
		"Mostly Penniless",
		"Peddler",
		"Dealer",
		"Merchant",
		"Broker",
		"Entrepreneur",
		"Tycoon",
		"Elite"
	};
	std::string explorerRank[9] = {
		"Aimless",
		"Mostly Aimless",
		"Scout",
		"Surveyor",
		"Trailblazer",
		"Pathfinder",
		"Ranger",
		"Pioneer",
		"Elite"
	};
	std::string cqcRank[9] = {
		"Helpless",
		"Mostly Helpless",
		"Amateur",
		"Semi Professional",
		"Professional",
		"Champion",
		"Hero",
		"Legend",
		"Elite"
	};
	std::string federationRank[15] = {
		"None",
		"Recruit",
		"Cadet",
		"Midshipman",
		"Petty Officer",
		"Chief Petty Officer",
		"Warrant Officer",
		"Ensign",
		"Lieutenant",
		"Lieutenant Commander",
		"Post Commander",
		"Post Captain",
		"Rear Admiral",
		"Vice Admiral",
		"Admiral"
	};
	std::string empireRank[15] = {
		"None",
		"Outsider",
		"Serf",
		"Master",
		"Squire",
		"Knight",
		"Lord",
		"Baron",
		"Viscount",
		"Count",
		"Earl",
		"Marquis",
		"Duke",
		"Prince",
		"King"
	};

	// Star Descriptions -> 11.2

	// Planet Classes -> 11.3

	// Atmosphere Classes -> 11.4

	// Volcanism Classes -> 11.5

	// BodyType values -> 11.7

private:
	void setPage0(nlohmann::json::object_t obj);
	void setPage1(TCHAR *jsonFilepath);
	void setPage2(TCHAR *jsonFilePath);
	void setPage3(TCHAR *jsonFilePath);
	std::wstring strToWStr(std::string str);

	// Journal functions
	void e_LoadGame(nlohmann::json::object_t obj);
	void e_Progress(nlohmann::json::object_t obj);
	void e_Rank(nlohmann::json::object_t obj);
	void e_Docked(nlohmann::json::object_t obj);
	void e_DockingGranted(nlohmann::json::object_t obj);
	void e_FSDJump(nlohmann::json::object_t obj);
	void e_Location(nlohmann::json::object_t obj);
	void e_SupercruiseEntry(nlohmann::json::object_t obj);
	void e_SupercruiseExit(nlohmann::json::object_t obj);
	void e_Touchdown(nlohmann::json::object_t obj);
	void e_Undocked(nlohmann::json::object_t obj);
	void e_Scan(nlohmann::json::object_t obj);
	void e_ShipyardSwap(nlohmann::json::object_t obj);
	void e_Continued(nlohmann::json::object_t obj);
	void e_DockFighter(nlohmann::json::object_t obj);
	void e_DockSRV(nlohmann::json::object_t obj);
	void e_LaunchFighter(nlohmann::json::object_t obj);
	void e_LaunchSRV(nlohmann::json::object_t obj);
	void e_Promotion(nlohmann::json::object_t obj);
	void e_VehicleSwitch(nlohmann::json::object_t obj);
	void createMap();
	
};

extern JSONDataStructure jsonDataClass;