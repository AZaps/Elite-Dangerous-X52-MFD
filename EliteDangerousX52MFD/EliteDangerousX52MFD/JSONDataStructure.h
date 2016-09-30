#pragma once

class JSONDataStructure
{
	
public:

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

	// Will need one page as empty blank dummy since the controller defaults to having one on and it duplicates the last page


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

private:
	void setPage0(TCHAR *jsonFilepath);
	void setPage1(TCHAR *jsonFilepath);
	void setPage2(TCHAR *jsonFilePath);
	void setPage3(TCHAR *jsonFilePath);
	std::wstring strToWStr(std::string str);
};

extern JSONDataStructure jsonDataClass;