# Elite-Dangerous-X52-MFD
X52 MFD interaction with Elite Dangerous

Using the new Journal functionality of Elite Dangerous with v2.2 The Guardians, extra information can now be displayed on the screen of the X52 controller.

Information on what is to be displayed on the controller will be provided as a text file separate from the download in the releases section.

## Important Information
Grab the latest release version of this app. Also unzip and put in a 'permanent' location. Creating a separate folder and putting this app in it would be a good idea and also make for easy deletion if needed.

The reason I state 'permanent' is due to how this application finds and reads various files. It'll ask for two file locations...
* The X52 profile you want to use so your keybindings can carry over. I named my profile Elite Dangerous so it'll be easy to find. The default location of the profiles are C:\\Users\\Public\\Public Documents\\SmartTechnology Profiles.
* The folder location of the journals is C:\\Users\\User Name\\Saved Games\\Frontier Developments\\Elite Dangerous.

So if you do change any of the locations of those files, just delete the EDX52Settings.txt file that is created in this applications folder and the information will just need to be entered again.

If the console window is clicked it will pause itself because it is waiting for input so, for it to continue functioning, hit the Esc key. To quit hit the 'X' button on the window or using 'Ctrl + C' in the console window

### Thanks
Frontier Developments for Elite Dangerous

Saitek for the use and development of the SDK to run this project

Niels Lohmann for work on [JSON for Modern C++](https://github.com/nlohmann/json)
