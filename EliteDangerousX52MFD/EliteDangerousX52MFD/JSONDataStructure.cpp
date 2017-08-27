// JSONDataStructure.cpp
#include "stdafx.h"
#include "JSONDataStructure.h"

using namespace std;
using json = nlohmann::json;

typedef void(JSONDataStructure::*EventFunction)(json::object_t obj);
std::unordered_map<std::string, EventFunction> eventsMap;

/*
	PARAMETERS: std::string eventString -> determined event title from the journal
	RETURNS: none

	FUNCTION: Determines the correct function to execute based on the event string
*/
void JSONDataStructure::readStoreJSON(std::string eventString) 
{
	// String to JSON object and compare event
	json::object_t tempObj = json::parse(eventString.begin(), eventString.end());
	if (tempObj["event"].is_null() != true)
	{
		string event = tempObj["event"];
		auto it = eventsMap.find(event);
		if (it == eventsMap.end())
		{
			//cout << "Event: " << tempObj["event"] << " not in list.\n";
		}
		else
		{
			// If the event matches, call the associated function within the map
			(*this.*it->second)(tempObj);
		}
	}
}

/*
	PARAMETERS: none
	RETURNS: none

	FUNCTION: Adds entries to the unordered map on startup. By doing this, comparison of the string from the log, if true, will call the associated function and update the saved data.
*/
void JSONDataStructure::createMap()
{																						
	eventsMap.emplace("LoadGame", &JSONDataStructure::e_LoadGame);						
	eventsMap.emplace("Rank", &JSONDataStructure::e_Rank);								
	eventsMap.emplace("Docked", &JSONDataStructure::e_Docked);							
	eventsMap.emplace("DockingGranted", &JSONDataStructure::e_DockingGranted);			
	eventsMap.emplace("FSDJump", &JSONDataStructure::e_FSDJump);
	eventsMap.emplace("Liftoff", &JSONDataStructure::e_Liftoff);
	eventsMap.emplace("Location", &JSONDataStructure::e_Location);						
	eventsMap.emplace("SupercruiseEntry", &JSONDataStructure::e_SupercruiseEntry);		
	eventsMap.emplace("SupercruiseExit", &JSONDataStructure::e_SupercruiseExit);		
	eventsMap.emplace("Touchdown", &JSONDataStructure::e_Touchdown);					
	eventsMap.emplace("Undocked", &JSONDataStructure::e_Undocked);						
	eventsMap.emplace("Scan", &JSONDataStructure::e_Scan);								
	eventsMap.emplace("BuyExplorationData", &JSONDataStructure::e_BuyExplorationData);	
	eventsMap.emplace("SellExplorationData", &JSONDataStructure::e_SellExplorationData);
	eventsMap.emplace("BuyTradeData", &JSONDataStructure::e_BuyTradeData);				
	eventsMap.emplace("MarketBuy", &JSONDataStructure::e_MarketBuy);					
	eventsMap.emplace("MarketSell", &JSONDataStructure::e_MarketSell);					
	eventsMap.emplace("BuyAmmo", &JSONDataStructure::e_BuyAmmo);						
	eventsMap.emplace("BuyDrones", &JSONDataStructure::e_BuyDrones);					
	eventsMap.emplace("CommunityGoalReward", &JSONDataStructure::e_CommunityGoalReward);
	eventsMap.emplace("CrewHire", &JSONDataStructure::e_CrewHire);						
	eventsMap.emplace("FetchRemoteModule", &JSONDataStructure::e_FetchRemoteModule);	
	eventsMap.emplace("MissionCompleted", &JSONDataStructure::e_MissionCompleted);		
	eventsMap.emplace("ModuleBuy", &JSONDataStructure::e_ModuleBuy);				
	eventsMap.emplace("ModuleSell", &JSONDataStructure::e_ModuleSell);			
	eventsMap.emplace("ModuleSellRemote", &JSONDataStructure::e_ModuleSellRemote);		
	eventsMap.emplace("PayFines", &JSONDataStructure::e_PayFines);						
	eventsMap.emplace("PayLegacyFines", &JSONDataStructure::e_PayLegacyFines);			
	eventsMap.emplace("RedeemVoucher", &JSONDataStructure::e_RedeemVoucher);			
	eventsMap.emplace("RefuelAll", &JSONDataStructure::e_RefuelAll);				
	eventsMap.emplace("RefuelPartial", &JSONDataStructure::e_RefuelPartial);		
	eventsMap.emplace("Repair", &JSONDataStructure::e_Repair);			
	eventsMap.emplace("RepairAll", &JSONDataStructure::e_RepairAll);
	eventsMap.emplace("RestockVehicle", &JSONDataStructure::e_RestockVehicle);			
	eventsMap.emplace("SellDrones", &JSONDataStructure::e_SellDrones);					
	eventsMap.emplace("ShipyardBuy", &JSONDataStructure::e_ShipyardBuy);				
	eventsMap.emplace("ShipyardSell", &JSONDataStructure::e_ShipyardSell);			
	eventsMap.emplace("ShipyardTransfer", &JSONDataStructure::e_ShipyardTransfer);		
	eventsMap.emplace("ShipyardSwap", &JSONDataStructure::e_ShipyardSwap);				
	eventsMap.emplace("PowerplayFastTrack", &JSONDataStructure::e_PowerplayFastTrack);	
	eventsMap.emplace("PowerplaySalary", &JSONDataStructure::e_PowerplaySalary);	
	eventsMap.emplace("Continued", &JSONDataStructure::e_Continued);				
	eventsMap.emplace("DockFighter", &JSONDataStructure::e_DockFighter);				
	eventsMap.emplace("DockSRV", &JSONDataStructure::e_DockSRV);						
	eventsMap.emplace("LaunchFighter", &JSONDataStructure::e_LaunchFighter);			
	eventsMap.emplace("LaunchSRV", &JSONDataStructure::e_LaunchSRV);					
	eventsMap.emplace("Promotion", &JSONDataStructure::e_Promotion);					
	eventsMap.emplace("VehicleSwitch", &JSONDataStructure::e_VehicleSwitch);			
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

void JSONDataStructure::copyCreditBalance()
{
	wcsncpy_s(pg0.cmdrPage0Info[3], strToWStr("CR: " + to_string(pg0.creditBalance)).c_str(), length);
}

std::string JSONDataStructure::formmatedShipName(std::string ship)
{
	for (int i = 0; i < ship.length(); i++)
	{
		ship[i] = toupper(ship[i]);
	}
	if (ship == "SIDEWINDER")
	{
		return "Sidewinder";
	}
	else if (ship == "EAGLE")
	{
		return "Eagle";
	}
	else if (ship == "HAULER")
	{
		return "Hauler";
	}
	else if (ship == "ADDER")
	{
		return "Adder";
	}
	else if (ship == "IMPERIAL EAGLE")
	{
		return "Imperial Eagle";
	}
	else if (ship == "VIPER MKIII")
	{
		return "Viper MkIII";
	}
	else if (ship == "COBRA MKIII")
	{
		return "Cobra MkIII";
	}
	else if (ship == "VIPER MKIV")
	{
		return "Viper MkIV";
	}
	else if (ship == "DIAMONDBACK SCOUT")
	{
		return "Diamondback Scout";
	}
	else if (ship == "COBRA MKIV")
	{
		return "Cobra MkIV";
	}
	else if (ship == "TYPE-6")
	{
		return "Type-6";
	}
	else if (ship == "DIAMONDBACK EXPLORER")
	{
		return "Diamondback Explorer";
	}
	else if (ship == "IMPERIAL COURIER")
	{
		return "Imperial Courier";
	}
	else if (ship == "KEELBACK")
	{
		return "Keelback";
	}
	else if (ship == "ASP SCOUT")
	{
		return "Asp Scout";
	}
	else if (ship == "VULTURE")
	{
		return "Vulture";
	}
	else if (ship == "ASP EXPLORER")
	{
		return "Asp Explorer";
	}
	else if (ship == "FEDERAL DROPSHIP")
	{
		return "Federal Dropship";
	}
	else if (ship == "TYPE-7")
	{
		return "Type-7";
	}
	else if (ship == "FEDERAL ASSAULT SHIP")
	{
		return "Federal Assault Ship";
	}
	else if (ship == "IMPERIAL CLIPPER")
	{
		return "Imperial Clipper";
	}
	else if (ship == "FEDERAL GUNSHIP")
	{
		return "Federal Gunship";
	}
	else if (ship == "ORCA")
	{
		return "Orca";
	}
	else if (ship == "BELUGA LINER")
	{
		return "Beluga Liner";
	}
	else if (ship == "FER-DE-LANCE")
	{
		return "Fer-de-Lance";
	}
	else if (ship == "PYTHON")
	{
		return "Python";
	}
	else if (ship == "TYPE-9")
	{
		return "Type-9";
	}
	else if (ship == "ANACONDA")
	{
		return "Anaconda";
	}
	else if (ship == "FEDERAL CORVETTE")
	{
		return "Federal Corvette";
	}
	else if (ship == "IMPERIAL CUTTER")
	{
		return "Imperial Cutter";
	}
	else if (ship == "EMPIRE_COURIER")
	{
		return "Empire Courier";
	}
	else if (ship == "ASP")
	{
		return "Asp";
	}
}

/*
	Each function will take the full JSON object event associated to the correct event key value. Hopefully each of the associated key values are self explanatory and can be easily determined.
	For easy lookup, each event function is prefixed with 'e_' and the duplicated function name from the journal manual is listed after.

	Not all parts of the function I found relevant yet so some functions may be short and only some of the associated key values are used.

	Each function does key value checks to see if it even exists. Most of the time this might be redundant but I'd rather have value checking saftey instead of the program crashing. I haven't noticed a performance impact either.
	Once the key value is determined safe and exists, the value is taken from it and stored in the correct location.
*/

void JSONDataStructure::e_LoadGame(json::object_t obj)
{
	wcsncpy_s(pg0.cmdrPage0Info[0], L"Greetings CMDR", length);

	// Key value checking if it exists otherwise, will crash
	if (obj["Commander"].is_null() != true) {
		wcsncpy_s(pg0.cmdrPage0Info[1], strToWStr(obj["Commander"]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[1], L"No Name", length);
	}

	if (obj["Ship"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[2], strToWStr(obj["Ship"]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[2], L"No Ship", length);
	}

	if (obj["Credits"].is_null() != true)
	{
		pg0.creditBalance = (long)obj["Credits"];
		wcsncpy_s(pg0.cmdrPage0Info[3], strToWStr("CR: " + to_string((long)obj["Credits"])).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[3], L"No Credits", length);
	}
}

void JSONDataStructure::e_Rank(json::object_t obj)
{
	string combat = "Combat: ";
	string trade = "Trade: ";
	string exploration = "Exploration: ";
	string empire = "Empire: ";
	string federation = "Federation: ";
	string cqc = "CQC: ";

	if (obj["Combat"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (combat.length() + combatRank[obj["Combat"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[4], strToWStr(combat + combatRank[obj["Combat"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[4], strToWStr("C: " + combatRank[obj["Combat"]]).c_str(), length);
		}
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[4], L"No Combat Rank", length);
	}

	if (obj["Trade"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (trade.length() + tradeRank[obj["Trade"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[5], strToWStr(trade + tradeRank[obj["Trade"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[5], strToWStr("T: " + tradeRank[obj["Trade"]]).c_str(), length);
		}
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[5], L"No Trade Rank", length);
	}

	if (obj["Explore"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (exploration.length() + explorerRank[obj["Explore"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[6], strToWStr(exploration + explorerRank[obj["Explore"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[6], strToWStr("EX: " + explorerRank[obj["Explore"]]).c_str(), length);
		}
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[6], L"No Explore Rank", length);
	}

	if (obj["Empire"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (empire.length() + empireRank[obj["Empire"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[7], strToWStr(empire + empireRank[obj["Empire"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[7], strToWStr("EM: " + empireRank[obj["Empire"]]).c_str(), length);
		}
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[7], L"No Empire Rank", length);
	}

	if (obj["Federation"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (federation.length() + federationRank[obj["Federation"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[8], strToWStr(federation + federationRank[obj["Federation"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[8], strToWStr("F: " + federationRank[obj["Federation"]]).c_str(), length);
		}
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[8], L"No Fed Rank", length);
	}

	if (obj["CQC"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[9], strToWStr(cqc + cqcRank[obj["CQC"]]).c_str(), length);
	}
	else
	{
		wcsncpy_s(pg0.cmdrPage0Info[9], L"No CQC Rank", length);
	}

}

void JSONDataStructure::e_Docked(json::object_t obj)
{
	// Clear out the vector
	pg1.cmdrPage1Info.clear();

	// Reset currentLine
	pg1.currentLine = 0;

	if (obj["StarSystem"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StarSystem"]).c_str());
	}

	if (obj["StationName"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StationName"]).c_str());
	}

	if (obj["StationType"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StationType"]).c_str());
	}

	pg1.cmdrPage1Info.push_back(strToWStr("Docked").c_str());

}

void JSONDataStructure::e_DockingGranted(json::object_t obj)
{
	// Store the starsystem as it will be needed after clearing
	wstring starsystem = pg1.cmdrPage1Info[0];

	// Clear vector and reset the current line
	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	// Push back the starsystem
	pg1.cmdrPage1Info.push_back(starsystem);

	// Get new info
	if (obj["StationName"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StationName"]).c_str());
	}

	if (obj["LandingPad"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr("LP -> " + to_string((int)obj["LandingPad"])).c_str());
	}
}

void JSONDataStructure::e_FSDJump(json::object_t obj)
{
	// Clear the vector and reset the currentline
	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	// Get new info
	if (obj["StarSystem"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StarSystem"]).c_str());
	}

	pg1.cmdrPage1Info.push_back(strToWStr("In Supercruise").c_str());

}

void JSONDataStructure::e_Liftoff(nlohmann::json::object_t obj)
{
	wstring starsystem = pg1.cmdrPage1Info[0];
	wstring body = pg1.cmdrPage1Info[1];

	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	pg1.cmdrPage1Info.push_back(starsystem);
	pg1.cmdrPage1Info.push_back(body);
}

void JSONDataStructure::e_Location(json::object_t obj)
{
	// Clear vector and reset the currentline
	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	// Get new info
	if (obj["StarSystem"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StarSystem"]).c_str());
	}

	// Determine docked status
	if (obj["Docked"].is_null() != true)
	{
		if (obj["Docked"])
		{
			// Get StationName and StationType since player is docked
			if (obj["StationName"].is_null() != true)
			{
				pg1.cmdrPage1Info.push_back(strToWStr(obj["StationName"]).c_str());
			}
			if (obj["StationType"].is_null() != true)
			{
				pg1.cmdrPage1Info.push_back(strToWStr(obj["StationType"]).c_str());
			}
			pg1.cmdrPage1Info.push_back(strToWStr("Docked").c_str());
		}
		else
		{
			// Not docked. Get Body and BodyType
			if (obj["Body"].is_null() != true)
			{
				pg1.cmdrPage1Info.push_back(strToWStr(obj["Body"]).c_str());
				std::string tempStr = obj["Body"];
				pg1.lastKnownBody = tempStr;
			}
			if (obj["BodyType"].is_null() != true)
			{
				// BodyType can be null which could be a barycenter
				string bodyType = obj["BodyType"];
				if (bodyType.empty())
				{
					pg1.cmdrPage1Info.push_back(strToWStr("Barycenter").c_str());
				}
				else
				{
					pg1.cmdrPage1Info.push_back(strToWStr(bodyType).c_str());
				}
			}
		}
	}
}

void JSONDataStructure::e_SupercruiseEntry(json::object_t obj)
{
	// Clear vector and reset currentline
	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	// Get new data
	if (obj["StarSystem"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StarSystem"]).c_str());
		pg1.cmdrPage1Info.push_back(strToWStr("In Supercruise").c_str());
	}
}

void JSONDataStructure::e_SupercruiseExit(json::object_t obj)
{
	// Clear the vector and reset currentline
	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	// Get new data
	if (obj["StarSystem"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StarSystem"]).c_str());
	}

	if (obj["Body"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["Body"]).c_str());
		std::string tempStr = obj["Body"];
		pg1.lastKnownBody = tempStr;
	}

	if (obj["BodyType"].is_null() != true)
	{
		// BodyType can be null which could be a barycenter. Might need testing if it is always a barycenter or it could just be space...
		string bodyType = obj["BodyType"];
		if (bodyType.empty())
		{
			pg1.cmdrPage1Info.push_back(strToWStr("Barycenter").c_str());
		}
		else
		{
			pg1.cmdrPage1Info.push_back(strToWStr(bodyType).c_str());
		}
	}
}

void JSONDataStructure::e_Touchdown(json::object_t obj)
{
	// Get the starsystem
	wstring starsystem = pg1.cmdrPage1Info[0];

	// Clear vector and reset currentline
	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	// Write back starsystem and body
	pg1.cmdrPage1Info.push_back(starsystem);
	if (!pg1.lastKnownBody.empty())
	{
		pg1.cmdrPage1Info.push_back(strToWStr(pg1.lastKnownBody).c_str());
	}

	// Get new latitude and longitude data
	if (obj["Latitude"].is_null() != true)
	{
		stringstream ss;
		ss << fixed << setprecision(5) << (float)obj["Latitude"];
		if ((float)obj["Latitude"] < 0)
		{
			pg1.cmdrPage1Info.push_back(strToWStr("Lat:" + ss.str()).c_str());
		}
		else
		{
			pg1.cmdrPage1Info.push_back(strToWStr("Lat: " + ss.str()).c_str());
		}
	}

	if (obj["Longitude"].is_null() != true)
	{
		stringstream ss;
		ss << fixed << setprecision(5) << (float)obj["Longitude"];
		if ((float)obj["Longitude"] < 0)
		{
			pg1.cmdrPage1Info.push_back(strToWStr("Lon:" + ss.str()).c_str());
		}
		else
		{
			pg1.cmdrPage1Info.push_back(strToWStr("Lon: " + ss.str()).c_str());
		}
	}
}

void JSONDataStructure::e_Undocked(json::object_t obj)
{
	// Get the starsystem
	wstring starsystem = pg1.cmdrPage1Info[0];

	// Clear the vector and reset currentline
	pg1.cmdrPage1Info.clear();
	pg1.currentLine = 0;

	// Write back starsystem
	pg1.cmdrPage1Info.push_back(starsystem);

	// Get new data
	if (obj["StationName"].is_null() != true)
	{
		pg1.cmdrPage1Info.push_back(strToWStr(obj["StationName"]).c_str());
	}
}

void JSONDataStructure::e_Scan(json::object_t obj)
{
	// Clear the vector and reset the currentline
	pg2.cmdrPage2Info.clear();
	pg2.currentLine = 0;

	// Get new data
	if (obj["BodyName"].is_null() != true)
	{
		pg2.cmdrPage2Info.push_back(strToWStr(obj["BodyName"]).c_str());
	}

	if (obj["StarType"].is_null() != true)
	{
		string temp0 = obj["StarType"];
		string temp1 = "Class: ";
		temp1 = temp1 + temp0;
		pg2.cmdrPage2Info.push_back(strToWStr(temp1).c_str());
	}

	if (obj["PlanetClass"].is_null() != true)
	{
		pg2.cmdrPage2Info.push_back(strToWStr(obj["PlanetClass"]).c_str());
	}

	if (obj["Landable"].is_null() != true)
	{
		bool isLandable = (bool)obj["Landable"];
		if (isLandable)
		{
			pg2.cmdrPage2Info.push_back(strToWStr("Landable").c_str());
		}
		else
		{
			pg2.cmdrPage2Info.push_back(strToWStr("Not Landable").c_str());
		}
	}

	if (obj["SurfaceGravity"].is_null() != true)
	{
		double gravity = (double)obj["SurfaceGravity"];
		gravity = gravity / 9.81;
		stringstream ss;
		ss << fixed << setprecision(2) << gravity;
		pg2.cmdrPage2Info.push_back(strToWStr(ss.str() + " G").c_str());
	}

	if (obj["Atmosphere"].is_null() != true)
	{
		string atmoStr = obj["Atmosphere"];
		if (!atmoStr.empty())
		{
			pg2.cmdrPage2Info.push_back(strToWStr(obj["Atmosphere"]).c_str());
		}
	}

	if (obj["TerraformState"].is_null() != true)
	{
		string terraStr = obj["TerraformState"];
		if (!terraStr.empty())
		{
			pg2.cmdrPage2Info.push_back(strToWStr(terraStr).c_str());
		}
	}

	if (obj["Volcanism"].is_null() != true)
	{
		string volStr = obj["TerraformState"];
		if (!volStr.empty())
		{
			pg2.cmdrPage2Info.push_back(strToWStr(obj["Volcanism"]).c_str());
		}
	}
}

void JSONDataStructure::e_BuyExplorationData(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_SellExplorationData(nlohmann::json::object_t obj)
{
	signed long int tempBalance = 0;
	signed long int tempBaseValue = 0;
	signed long int tempBonus = 0;
	if (obj["BaseValue"].is_null() != true)
	{
		tempBaseValue = obj["BaseValue"];
		pg0.creditBalance += obj["BaseValue"];
	}

	if (obj["Bonus"].is_null() != true)
	{
		tempBonus = obj["Bonus"];
		pg0.creditBalance += obj["Bonus"];
	}

	tempBalance = tempBaseValue + tempBonus;
	pg0.creditBalance += tempBalance;
	copyCreditBalance();
}

void JSONDataStructure::e_BuyTradeData(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_MarketBuy(nlohmann::json::object_t obj)
{
	if (obj["TotalCost"].is_null() != true)
	{
		signed long int tempBalance = obj["TotalCost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_MarketSell(nlohmann::json::object_t obj)
{
	if (obj["TotalSale"].is_null() != true)
	{
		signed long int tempBalance = obj["TotalSale"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_BuyAmmo(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_BuyDrones(nlohmann::json::object_t obj)
{
	if (obj["TotalCost"].is_null() != true)
	{
		signed long int tempBalance = obj["TotalCost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_CommunityGoalReward(nlohmann::json::object_t obj)
{
	if (obj["Reward"].is_null() != true)
	{
		signed long int tempBalance = obj["Reward"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_CrewHire(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_FetchRemoteModule(nlohmann::json::object_t obj)
{
	if (obj["TransferCost"].is_null() != true)
	{
		signed long int tempBalance = obj["TransferCost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_MissionCompleted(nlohmann::json::object_t obj)
{
	signed long int tempBalance = 0;
	signed long int tempReward = 0;
	signed long int tempDonation = 0;
	if (obj["Reward"].is_null() != true)
	{
		tempReward = obj["Reward"];
	}

	if (obj["Donation"].is_null() != true)
	{
		tempDonation = obj["Donation"];
		tempDonation = tempDonation * -1;
	}

	tempBalance = tempReward + tempDonation;
	if (tempBalance < 0)
	{
		pg0.creditBalance -= abs(tempBalance);
		copyCreditBalance();
	}
	else
	{
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_ModuleBuy(nlohmann::json::object_t obj)
{
	signed long int tempBuy = 0;
	signed long int tempSell = 0;
	signed long int tempBalance = 0;

	if (obj["BuyPrice"].is_null() != true)
	{
		tempBuy = obj["BuyPrice"];
		tempBuy = tempBuy * -1;
	}
	if (obj["SellPrice"].is_null() != true)
	{
		tempSell = obj["SellPrice"];
	}

	// If the balance is less than zero, player lost money on the module buy
	tempBalance = tempBuy + tempSell;
	if (tempBalance < 0)
	{
		pg0.creditBalance -= abs(tempBalance);
		copyCreditBalance();
	}
	else
	{
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_ModuleSell(nlohmann::json::object_t obj)
{
	if (obj["SellPrice"].is_null() != true)
	{
		signed long int tempBalance = obj["SellPrice"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_ModuleSellRemote(nlohmann::json::object_t obj)
{
	if (obj["SellPrice"].is_null() != true)
	{
		signed long int tempBalance = obj["SellPrice"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_PayFines(nlohmann::json::object_t obj)
{
	if (obj["Amount"].is_null() != true)
	{
		signed long int tempBalance = obj["Amount"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_PayLegacyFines(nlohmann::json::object_t obj)
{
	if (obj["Amount"].is_null() != true)
	{
		signed long int tempBalance = obj["Amount"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_RedeemVoucher(nlohmann::json::object_t obj)
{
	if (obj["Amount"].is_null() != true)
	{
		signed long int tempBalance = obj["Amount"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_RefuelAll(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_RefuelPartial(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_Repair(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_RepairAll(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_RestockVehicle(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_SellDrones(nlohmann::json::object_t obj)
{
	if (obj["TotalSale"].is_null() != true)
	{
		signed long int tempBalance = obj["TotalSale"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_ShipyardBuy(nlohmann::json::object_t obj)
{
	signed long int tempBalance = 0;
	signed long int tempBuy = 0;
	signed long int tempSell = 0;
	if (obj["ShipPrice"].is_null() != true)
	{
		tempBuy = obj["ShipPrice"];
		tempBuy = tempBuy * -1;
	}
	if (obj["SellPrice"].is_null() != true)
	{
		tempSell = obj["SellPrice"];
	}
	if (obj["StoreOldShip"].is_null() != true)
	{
		string temp = obj["StoreOldShip"];
		if (temp != "")
		{
			if (obj["ShipType"].is_null() != true)
			{
				string temp = obj["ShipType"];
				wcsncpy_s(pg0.cmdrPage0Info[2], strToWStr(formmatedShipName(temp)).c_str(), length);
			}
		}
	}

	tempBalance = tempBuy + tempSell;
	if (tempBalance < 0)
	{
		pg0.creditBalance -= abs(tempBalance);
		copyCreditBalance();
	}
	else
	{
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_ShipyardSell(nlohmann::json::object_t obj)
{
	if (obj["ShipPrice"].is_null() != true)
	{
		signed long int tempBalance = obj["ShipPrice"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_ShipyardTransfer(nlohmann::json::object_t obj)
{
	if (obj["TransferPrice"].is_null() != true)
	{
		signed long int tempBalance = obj["TransferPrice"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_ShipyardSwap(json::object_t obj)
{
	// Changing the name of the currently selected ship
	if (obj["ShipType"].is_null() != true)
	{
		string temp = obj["ShipType"];
		wcsncpy_s(pg0.cmdrPage0Info[2], strToWStr(formmatedShipName(temp)).c_str(), length);
	}
}

void JSONDataStructure::e_PowerplayFastTrack(nlohmann::json::object_t obj)
{
	if (obj["Cost"].is_null() != true)
	{
		signed long int tempBalance = obj["Cost"];
		pg0.creditBalance -= tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_PowerplaySalary(nlohmann::json::object_t obj)
{
	if (obj["Amount"].is_null() != true)
	{
		signed long int tempBalance = obj["Amount"];
		pg0.creditBalance += tempBalance;
		copyCreditBalance();
	}
}

void JSONDataStructure::e_Continued(json::object_t obj)
{
	// Notify new file needs to be read
	this->continueEvent = true;
}

void JSONDataStructure::e_DockFighter(json::object_t obj)
{
	// Restore mothership name if fighter was docked by player control
	if (pg0.playerControlFighter)
	{
		wcsncpy_s(pg0.cmdrPage0Info[2], pg0.mothership, length);
	}
}

void JSONDataStructure::e_DockSRV(json::object_t obj)
{
	// Restore mothership name
	wcsncpy_s(pg0.cmdrPage0Info[2], pg0.mothership, length);
}

void JSONDataStructure::e_LaunchFighter(json::object_t obj)
{
	// Determine if player controlled
	if (obj["PlayerControlled"].is_null() != true)
	{
		if ((bool)obj["PlayerControlled"])
		{
			// Save the current Mothership name
			wcsncpy_s(pg0.mothership, pg0.cmdrPage0Info[2], length);

			// Change ship name to Fighter
			wcsncpy_s(pg0.cmdrPage0Info[2], L"Fighter", length);

			pg0.playerControlFighter = true;
		}
	}
}

void JSONDataStructure::e_LaunchSRV(json::object_t obj)
{
	// Save current mothership name
	wcsncpy_s(pg0.mothership, pg0.cmdrPage0Info[2], length);

	// Change ship name to SRV
	wcsncpy_s(pg0.cmdrPage0Info[2], L"SRV", length);
}

void JSONDataStructure::e_Promotion(json::object_t obj)
{
	string combat = "Combat: ";
	string trade = "Trade: ";
	string exploration = "Exploration: ";
	string cqc = "CQC: ";
	
	if (obj["Combat"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (combat.length() + combatRank[obj["Combat"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[4], strToWStr(combat + combatRank[obj["Combat"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[4], strToWStr("C: " + combatRank[obj["Combat"]]).c_str(), length);
		}
	}

	if (obj["Trade"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (trade.length() + tradeRank[obj["Trade"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[5], strToWStr(trade + tradeRank[obj["Trade"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[5], strToWStr("T: " + tradeRank[obj["Trade"]]).c_str(), length);
		}
	}

	if (obj["Explore"].is_null() != true)
	{
		// Determine lengths if shorthand is needed
		if (exploration.length() + explorerRank[obj["Explore"]].length() <= 16)
		{
			wcsncpy_s(pg0.cmdrPage0Info[6], strToWStr(exploration + explorerRank[obj["Explore"]]).c_str(), length);
		}
		else
		{
			wcsncpy_s(pg0.cmdrPage0Info[6], strToWStr("EX: " + explorerRank[obj["Explore"]]).c_str(), length);
		}
	}

	if (obj["CQC"].is_null() != true)
	{
		wcsncpy_s(pg0.cmdrPage0Info[9], strToWStr(cqc + cqcRank[obj["CQC"]]).c_str(), length);
	}
}

void JSONDataStructure::e_VehicleSwitch(json::object_t obj)
{
	if (obj["VehicleSwitch"].is_null() != true)
	{
		// Determine switch
		string shipSwitch = obj["To"];
		if (shipSwitch == "Fighter")
		{
			// Save mothership name
			wcsncpy_s(pg0.mothership, pg0.cmdrPage0Info[2], length);

			// Ship name to fighter
			wcsncpy_s(pg0.cmdrPage0Info[2], L"Fighter", length);
		}
		else if (shipSwitch == "Mothership")
		{
			// Restore mothership name
			wcsncpy_s(pg0.cmdrPage0Info[2], pg0.mothership, length);
		}
	}
}