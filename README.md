PebblePasscodeWindow
====================

This is a passcode window I'm using in my two apps (<a href="http://appstore.com/notesforpebble">Notes for Pebble</a> and <a href="http://appstore.com/contactsforpebble">Contacts for Pebble</a>) to secure the access to the watchapp.

#Demo

Open the Demo folder. That project is a working watchapp that demonstrate the usage.

#Usage

Save a 4-digit string (aka passcode) to the persistent storage with the key 1.

    persistent_write_string(1, "1234");
    
If a passcode is found, the watchapp will open the Passcode Window to authenticate the user. If the passcode is correct, the app's window will be pushed otherwise an error message will appear.

You may need to adjust the functions names to correctly work with your windows if you plan to use it in your already exisitng app.

<img src="https://raw.githubusercontent.com/BalestraPatrick/PebblePasscodeWindow/master/screenshot1.png"><img src="https://raw.githubusercontent.com/BalestraPatrick/PebblePasscodeWindow/master/screenshot2.png">

#Credits

Created by <a href="http://www.patrickbalestra.com">Patrick Balestra</a>. If this project was useful to you and you're using it in any of your apps, let me know! :)

Thanks to <a href="https://github.com/edwinfinch">Edwin Finch</a> for the great pull requests.
