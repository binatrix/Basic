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
