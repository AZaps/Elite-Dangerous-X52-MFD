#pragma once
#include "DirectOutputFn.h"
#include <unordered_map>
#include "DepInclude\json.hpp"

class JSONDataStructure
{
	typedef std::vector<std::wstring> v_strList;

public:
	void readStoreJSON(std::string eventString);
	void createMap();

	bool continueEvent;
	unsigned int readLine;

	/*
		--- Commander Data ---
		Name
		Ship
		Credits
		Ranks
	*/
	struct mfdPage0
	{
		int currentLine;
		long long creditBalance;
		wchar_t mothership[64];
		bool playerControlFighter;
		wchar_t cmdrPage0Info[10][64];
	}; mfdPage0 pg0;

	/*
		--- Location Based Data ---
		StarSystem
		StationName <-> CurrentBody <-> Body <-> Supercruise Status
		^StationType <-> BodyType (if null, then BodyType is a barycenter of a binary starsystem)
		^LandingPad (if docking) <-> Docked (if docked) <-> Latitude (if landed on planet)
		Longitude (if landed on planet)

		(^ = if current data doesn't exist push up and replace)
	*/
	struct mfdPage1
	{
		int currentLine;
		std::string lastKnownBody = "";
		v_strList cmdrPage1Info;
	}; mfdPage1 pg1;

	/*
		--- Scan Data ---
		BodyName
		StarType <-> PlanetClass
		Landable
		SurfaceGravity
		Atmosphere
		^TerraformState
		^Volcanism
	*/
	struct mfdPage2
	{
		int currentLine;
		v_strList cmdrPage2Info;
	}; mfdPage2 pg2;

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

private:
	int length = 64;

	std::wstring strToWStr(std::string str);
	void copyCreditBalance();
	std::string formmatedShipName(std::string ship);

	// Journal functions										Location
	void e_LoadGame(nlohmann::json::object_t obj);				// 3.3
	void e_Rank(nlohmann::json::object_t obj);					// 3.5
	void e_Docked(nlohmann::json::object_t obj);				// 4.1
	void e_DockingGranted(nlohmann::json::object_t obj);		// 4.4
	void e_FSDJump(nlohmann::json::object_t obj);				// 4.7
	void e_Liftoff(nlohmann::json::object_t obj);				// 4.8
	void e_Location(nlohmann::json::object_t obj);				// 4.9
	void e_SupercruiseEntry(nlohmann::json::object_t obj);		// 4.10
	void e_SupercruiseExit(nlohmann::json::object_t obj);		// 4.11
	void e_Touchdown(nlohmann::json::object_t obj);				// 4.12
	void e_Undocked(nlohmann::json::object_t obj);				// 4.13
	void e_Scan(nlohmann::json::object_t obj);					// 6.1
	void e_BuyExplorationData(nlohmann::json::object_t obj);	// 6.5
	void e_SellExplorationData(nlohmann::json::object_t obj);	// 6.6
	void e_BuyTradeData(nlohmann::json::object_t obj);			// 7.1
	void e_MarketBuy(nlohmann::json::object_t obj);				// 7.4
	void e_MarketSell(nlohmann::json::object_t obj);			// 7.5
	void e_BuyAmmo(nlohmann::json::object_t obj);				// 8.1
	void e_BuyDrones(nlohmann::json::object_t obj);				// 8.2
	void e_CommunityGoalReward(nlohmann::json::object_t obj);	// 8.4
	void e_CrewHire(nlohmann::json::object_t obj);				// 8.8
	void e_FetchRemoteModule(nlohmann::json::object_t obj);		// 8.12
	void e_MissionCompleted(nlohmann::json::object_t obj);		// 8.16
	void e_ModuleBuy(nlohmann::json::object_t obj);				// 8.18
	void e_ModuleSell(nlohmann::json::object_t obj);			// 8.20
	void e_ModuleSellRemote(nlohmann::json::object_t obj);		// 8.21
	void e_PayFines(nlohmann::json::object_t obj);				// 8.24
	void e_PayLegacyFines(nlohmann::json::object_t obj);		// 8.25
	void e_RedeemVoucher(nlohmann::json::object_t obj);			// 8.26
	void e_RefuelAll(nlohmann::json::object_t obj);				// 8.27
	void e_RefuelPartial(nlohmann::json::object_t obj);			// 8.28
	void e_Repair(nlohmann::json::object_t obj);				// 8.29
	void e_RepairAll(nlohmann::json::object_t obj);				// 8.30
	void e_RestockVehicle(nlohmann::json::object_t obj);		// 8.31
	void e_SellDrones(nlohmann::json::object_t obj);			// 8.33
	void e_ShipyardBuy(nlohmann::json::object_t obj);			// 8.34
	void e_ShipyardSell(nlohmann::json::object_t obj);			// 8.36
	void e_ShipyardTransfer(nlohmann::json::object_t obj);		// 8.37
	void e_ShipyardSwap(nlohmann::json::object_t obj);			// 8.38
	void e_PowerplayFastTrack(nlohmann::json::object_t obj);	// 9.4
	void e_PowerplaySalary(nlohmann::json::object_t obj);		// 9.7
	void e_Continued(nlohmann::json::object_t obj);				// 10.4
	void e_DockFighter(nlohmann::json::object_t obj);			// 10.8
	void e_DockSRV(nlohmann::json::object_t obj);				// 10.9
	void e_LaunchFighter(nlohmann::json::object_t obj);			// 10.13
	void e_LaunchSRV(nlohmann::json::object_t obj);				// 10.14
	void e_Promotion(nlohmann::json::object_t obj);				// 10.15
	void e_VehicleSwitch(nlohmann::json::object_t obj);			// 10.23	
};

extern JSONDataStructure jsonDataClass;