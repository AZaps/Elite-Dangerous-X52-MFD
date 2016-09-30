# Elite-Dangerous-X52-MFD
X52 MFD interaction with Elite Dangerous

This program will pull your CMDR data from the Frontier Elite Dangerous servers as a JSON file. I use this file to display relevant data to the MFD screen on the X52. So far the page data consists of...

#### Page 0
* Greetings CMDR
* (CMDR NAME)
* CR: (CREDIT BALANCE)
* (CURRENT SHIP)
* Combat: (COMBAT_RANK)
* Trade: (TRADE_RANK)
* Exploration: (EXPLORATION_RANK)
* CQC: (CQC_RANK)
* Federation: (FEDERATION_RANK)
* Empire: (EMPIRE_RANK)

#### Page 1
* Ship Details
* (CURRENT_SHIP_NAME)
* Cargo: (CURRENT / TOTAL_CARGO_CAPACITY)
* MainTank: (CURRENT / TOTAL_MAIN_FUEL_LEVEL)
* ResTank: (CURRENT / TOTAL_RESERVE_TANK)
* Oxygen: (TIME_REMAINING MINUTES:SECONDS)

#### Page 2 *In-Progress*
* (CURRENT_SYSTEM)
* (CURRENT_STATION) if possible
* (CURRENT_PLANET) if landed on one and not on station

#### Page 3 *In-Progress*
* Docked Ships
* (SHIP_NAME)
* (STARSYSTEM)
* (STATION)

## Thanks
Frontier Developments for Elite Dangerous

Saitek for the use and development of the SDK to run this project

Andargor for work on [Elite Dangerous Companion Emulator](https://github.com/Andargor/edce-client)

Niels Lohmann for work on [JSON for Modern C++](https://github.com/nlohmann/json)
