### Version 3.2

Updates on this version:

- OLED changes:
    - Disabled auto-display when draw to screen to improve performance. To draw to screen you must invoke "oled.display()"
    -   Add new functions:
        - oled.width(string), get the string width with current font
        - oled.align(align), set the text alignment on screen:
            - 0, TEXT_ALIGN_LEFT
            - 1, TEXT_ALIGN_RIGHT
            - 2, TEXT_ALIGN_CENTER
            - 3, TEXT_ALIGN_CENTER_BOTH
        - oled.flip(), flip screen vertically
        - oled.display(), draw content to screen
```
oled.align(2) 'Centered
oled.print("Hello", 10, 10)
oled.display()
```
- IO changes:
    - Add PIP input mode for pin with internal PULLUP
```
io(pip, D7)
```
- Add SOUND object to play music async using Nokia RTTTL. Buzzer needed on pin. 
    - sound.pin(pin), set the speaker pin
    - sound.play(rtttl), play the RTTTL song
    - sound.stop(), stop current playing song
    - soundBranch, callback when music stop playing. Optional
```
soundBranch [EndSong]
song = "Star Trek:d=4,o=5,b=63:8f.,16a#,d#.6,8d6,16a#.,16g.,16c.6,f6"
sound.pin(16)
sound.play(song)
...
[EndSong]
print "Song stopped"
wait
```

### Version 3.1

Updates on this version:

- ESP8266 core update to version 2.7.4
- PROGMEM variables reading optimization to avoid crashes
- Fix blank line added at end of script when saved
- Set UTF-8 codepage in editor
- Basic MQTT implementation. Sample:
```
server = "test.mosquitto.org"
port = 1883 'optional
user = "" 'optional
pass = "" 'optional
topic = "test_123"
payload = "ESP Test"
MQTT.setup(server) ', port, user, pass)
mqttbranch [onNewMsg]
MQTT.subscribe(topic)

[reset]
cls
textbox payload
button "Send", [send]
button "Clear the screen", [reset]
button "End", [end]
wait

[send]
MQTT.publish(topic, payload, 0) 'optional, 1 or 0 for retain
wait

[onNewMsg]
print MQTT.msg()
wait

[end]
end
```

Basic
Basic Interpreter from scratch especialy for the ESP8266.

http://esp8266basic.com

https://docs.google.com/document/d/1EiYugfu12X2_pmfmu2O19CcLX0ALgLM4r2YxKYyJon8/pub

http://www.esp8266basic.com/flashing-instructions.html



The libraries folder contains the libraries currently being used.
Look at licence information for each.

Compile using the arduino ESP8266 package using the stable version 2.3.0

Special thanks to the people who have worked to extend and improve ESP BASIC.
Contributers include MMiscool, Cicciob, Rotohammer, Livetv, Tcpipchip.

ESP Basic allows for browser based development of basic programs to run on the esp8266 microcontroller. 

Once the firmware is installed basic programming is done with a standard web browser. 
No usb to serial cables needed. Can be programmed from any device that has a modern web browser such as chrome.
Even a cell phone or cheap tablet can be used to program your ESP8266 based project with out cables once the firmware is installed. 
