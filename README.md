With this project, I no longer control my own AC manually; it's auto-controlled by an ESP32! Or by you, if you want, of course.

The logic is quite simple. There are two modes: auto mode and self mode.

If auto mode is on, it works like this:

The DHT11 reads the temperature and returns it.
If the temperature is 29 degrees Celsius or higher and one of these conditions is met:
The AC has been off for at least 10 minutes.
No "power on" command has been sent before.
It will turn on the AC.
After the AC is on, your room will get cooler. There are two important room temperatures that trigger the "power off" command:

If the room temperature drops below 24 degrees Celsius, the AC will shut down no matter how long it has been working, so you won't freeze.
If the temperature is between 24-27 degrees Celsius, and the AC has been working for 10 minutes, it will turn off.
You can change these values in the source code to adapt the settings for your AC. Currently, this project works with the Mitsubishi112 protocol. Check the IRemoteESP8266 by crankyoldgit on github for that!

If self mode is on, you can open and close your AC using your phone, regardless of whether it has an IR LED.

You can easily set your AC's swing, mode, fan speed, and temperature using MQTT. You can find each topic by checking the code and adapt it as needed. (Came with version 2)

And I tried to split code as much as possible so I hope the code doesn't seem chaotic.

THIS IS NOT A COMPLETE PROJECT. I am sharing it so I won't lose my code in the future, and someone might get inspired by it.
