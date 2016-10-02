#pragma once
#include "DirectOutputFn.h"

class JSONDataStructure
{
	typedef std::vector<std::wstring> cmdrShipList;
public:

	bool isFirstTime;

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
		cmdrShipList cmdrPage3Info;
	}; mfdPage3 pg3;

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
	
	void readStoreJSON(TCHAR *jsonDirectory, TCHAR *defaultDirectory, TCHAR *jsonFilepath);
	void updateCurrentPage(TCHAR *jsonDirectory, TCHAR *defaultDirectory, TCHAR *jsonFilepath, DirectOutputFn& fnJSON);

private:
	void setPage0(TCHAR *jsonFilepath);
	void setPage1(TCHAR *jsonFilepath);
	void setPage2(TCHAR *jsonFilePath);
	void setPage3(TCHAR *jsonFilePath);
	std::wstring strToWStr(std::string str);
};

extern JSONDataStructure jsonDataClass;