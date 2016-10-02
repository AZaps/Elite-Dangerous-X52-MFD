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

#### Page 2
* (CURRENT_SYSTEM)
* (CURRENT_STATION) if possible
* (CURRENT_PLANET) if landed on one and not on station

#### Page 3
* Docked Ships
* (SHIP_NAME)
* (STARSYSTEM)
* (STATION)

## Important Information
For this to function correctly please download the latest verison of [EDCE](https://github.com/Andargor/edce-client), download zip is fine. Once downloaded you'll need to unzip and put the folder in a 'permanent' location.

Grab the latest release version* of this app. Also unzip and put in a 'permanent' location. Creating a separate folder and putting this app and EDCE in it would be a good idea.

The reason I state 'permanent' is due to how this application finds and reads various files. It'll ask for two file locations...
* The X52 profile you want to use so your keybindings can carry over. I named my profile Elite Dangerous so it'll be easy to find. The default location of the profiles are C:\\Users\\Public\\Public Documents\\SmartTechnology Profiles.
* The location of the edce_client.py file. That'll be in the main folder of EDCE. From this file I also create a path to the last.json file that gets created everytime the script runs.

So if you do change any of the locations of those files, just delete the EDX52Settings.txt file that is created in this applications folder and the information will just need to be entered again.

As a request from Frontier, and also built into EDCE, there needs to be a delay in accessing your data from their servers. I have set a timer of 2 1/2 minutes in the app between fetching. There is also a chance that when EDCE goes out to pull fresh info, the Elite servers might have not updated yet so there is a chance for a longer delay.

This app will continue to run as long as the Elite Dangerous launcher is open. It can still be closed normally by tapping the X or by clicking in the console window and pressing 'Ctrl + C'. Also, if the console window is clicked it will pause itself because it is waiting for input so, for it to continue functioning, either click the top banner of the window or just hit the Esc key.

### Thanks
Frontier Developments for Elite Dangerous

Saitek for the use and development of the SDK to run this project

Andargor for work on [Elite Dangerous Companion Emulator](https://github.com/Andargor/edce-client)

Niels Lohmann for work on [JSON for Modern C++](https://github.com/nlohmann/json)
