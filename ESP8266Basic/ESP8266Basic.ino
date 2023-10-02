#include "tft.on.h"
//#define BASIC_TFT

//ESP8266 Basic Interpreter
//HTTP://ESP8266BASIC.COM
//
//The MIT License (MIT)
//
//Copyright (c) 2015 Michael Molinari
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

//Onewire temperature sensor code contributed by Rotohammer.

//Significant speed improvements submitted by cicciocb
//Uploading of bas files improvement added by cicciocb
//JSON parsing routine added by cicciocb



// Comment or uncomment following lines to produce a custom build omitting features.








#include <WebSocketsServer.h>
#include <Hash.h>
#include <WebSockets.h>


//#include <ArduinoJson.h>
//#include "spiffs/spiffs.h"
#include <FS.h>
#include <ESP8266mDNS.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
//#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
//#include <ESP8266httpUpdate.h>               // that file need to be copied into the folder for 2.0.0-rc1
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Base64.h"

//#include <WiFiClientSecure.h>
#if defined(BASIC_TFT)
#include "ESP8266httpUpdate.h"
#endif
#include <time.h>
#include <ESP8266HTTPClient.h>              // that line needs to be added for the esp8266-2.0.0 and 2.1.0-rc2

//Ping stuff
#include <ESP8266Ping.h>



//LCD Stuff
 #if defined(BASIC_TFT)
#include <LiquidCrystal_SR.h>
#include <I2CIO.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_SR3W.h>
#include <LCD.h>
#include <LiquidCrystal_SR2W.h>
#endif
//#include <FastIO.h>
#include <LiquidCrystal_I2C.h>

//PS2 Key Board
//#include <PS2Keyboard.h>


#include <Time.h>

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel *pixels = NULL;

#include <SoftwareSerial.h>
SoftwareSerial *swSer = NULL;

#include "VERSION.h"



//ThingSpeak Stuff




//wifi mode exclusivity 
bool wifiApStaModeOn = 0;
//WIFI SCANN STUFF
int numberOfWifiScanResults;


// SPI STUFF
#include <SPI.h>

// Infrared Stuff
#include <IRremoteESP8266.h>
IRsend *irsend = NULL; //irsend(0); //an IR led is connected to GPIO pin 0
IRrecv *irrecv = NULL;
decode_results IRresults;
int IRBranchLine = 0;



#if defined(BASIC_TFT)
//ILI9341 Stuff
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
// For the Adafruit shield, these are the default.
//#define TFT_DC 4
//#define TFT_CS 5
// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 *tft = NULL; //= Adafruit_ILI9341(TFT_CS, TFT_DC);
byte TFT_CS_pin, TFT_DC_pin;
#include "TFT_Objects.h"
TFT_Form form1(40);

// touch stuff
byte Touch_CS_pin;
int TouchBranchLine = 0;
int Touch_p = 0;
unsigned int Touch_millis = 0;
uint16_t touchX, touchY;
uint16_t touchX_raw, touchY_raw;
#endif

// The Math precision is defined, by default, inside expression_parser_string.h
// there is a definition PARSER_PREC that can be double or float
#include "expression_parser_string.h"
char* _parser_error_msg;

PARSER_PREC numeric_value;
String string_value ;
int parser_result;

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);



#include <DHT.h>   // adafruit library
//DHT dht(5, DHT21);   // 5 is GPIO5, DHT21 you may want change at DHT11 or DHT22
DHT *dht = NULL;


 #if defined(BASIC_TFT)
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address
#endif

// i2c OLED Display

#if defined(BASIC_TFT)
#include "SSD1306.h" 
SSD1306  display(0x3c);
#endif




//PS2Keyboard keyboard;

//WiFiClient EmailClient;
String EmailServer;
int Emailport;
String EmailSMTPuser;
String EmailSMTPpassword;

// udp client
WiFiUDP udp;

// WebSockets
WebSocketsServer *webSocket;
String WebSockMessage;
int WebSockEventBranchLine = 0;
int WebSockChangeBranchLine = 0;
String WebSockEventName;
String WebSockChangeName;
int WebSocketTimeOut[5];
int delaytime;

WiFiClient client;
WiFiClient clientTelnet;
int telnetBranch;

//MQTT Stuff
#include <PubSubClient.h>
//#include "Adafruit_MQTT.h"
//#include "Adafruit_MQTT_Client.h"
PubSubClient MQTTclient(client);
String MQTTServer;
String MQTTUser, MQTTPass;
String MQTTlatestMsg;
bool MQTTnewMsgReceived ;
bool MQTTActivated;
String MQTTBranch;
String MQTTSubscribeTopic;
String MQTTPublishTopic;
bool MQTTPublishRetain;
String MQTTPublishMSG;
int MQTTTimeFromLastCheck;
String MQTTClientID;


ESP8266WebServer *server; //server(80);

//Web Server Variables
byte WebGuiOff = 0;
String HTMLout;
#include "progmem.h"

String LastElimentIdTag;
byte WaitForTheInterpertersResponse = 1;
byte numberButtonInUse = 0;
//String ButtonsInUse[20];


bool NewGuiItemAddedSinceLastWait;
bool NewGraphicItemAddedSinceLastWait;

int  msgbranch;
String   MsgBranchRetrnData;

int UdpBranchLine = 0;
String  UdpBuffer = "";
IPAddress UdpRemoteIP;
int UdpRemotePort;


// place where the program will jump on serial data reception
int SerialBranchLine  = 0;
int Serial2BranchLine = 0;


// Buffer to store incoming commands from serial port
String inData;

const int TotalNumberOfLines = 5000;            //this is the maximum number of lines that can be saved/loaded; it can go until 65535
int  program_nb_lines = 0;                      //this is the number of program lines read from the file

int LastVarNumberLookedUp;                                 //Array to hold all of the basic variables
bool VariableLocated;

const int TotalNumberOfVariables = 100;
#define VariablesNameLength  30
#include "Classes.h"

bool RunningProgram = 1;                                //Will be set to 1 if the program is currently running
int RunningProgramCurrentLine = 0;                     //Keeps track of the currently running line of code
//byte NumberOfReturns;
bool BasicDebuggingOn;
int debugDelaySpeed;
//uint16_t ReturnLocations[254];

int TimerWaitTime;
int timerLastActiveTime;
int timercbLastActiveTime;
String TimerBranch;

String refreshBranch;

uint16_t GraphicsEliments[100][7];
String GraphicsText[100];

File fsUploadFile;

int noOfLinesForEdit;
String ProgramName = "/default.bas";

bool fileOpenFail;


bool inputPromptActive = 0;

int LoggedIn = 0;

int SerialTimeOut;


int TimerCBtime = 0;
int TimerCBBranchLine;

//uint16_t ForNextReturnLocations[255];



Servo servos[11];


String  PinListOfStatus[17];
int  PinListOfStatusValues[17];


//time Stff
int timezone = 3;
int dst = 0;



FSInfo fs_info;

#include "Sound.h"
Sound sound;

void setup() {
  //  dht.begin();
  //  pixels.begin();
  SPIFFS.begin();
  Serial.begin(9600);
  // gets the listening port
  String listenport = LoadDataFromFile("listenport");
  String WebSocketslistenport = LoadDataFromFile("wsport");
  // listening port - by default goes to 80 -
  if (listenport.toInt() == 0) listenport = "80";
  if (WebSocketslistenport.toInt() == 0) {WebSocketslistenport = F("81"); SaveDataToFile(F("wsport"), WebSocketslistenport);}

  // print the listening port in the console
  Serial.println("HTTP Port: " + listenport);
  // create the instance of the web server
  server = new ESP8266WebServer(listenport.toInt());
  webSocket = new WebSocketsServer(WebSocketslistenport.toInt());
  
  

  //Serial.setDebugOutput(true);
  WiFi.setAutoConnect(false) ;
  WiFi.mode(WIFI_AP_STA);
  PrintAndWebOut(P_STR(P_BasicVersion));
  PrintAndWebOut("MAC: " + WiFi.softAPmacAddress());
  //CheckWaitForRunningCode();

  String mac = WiFi.macAddress();
  mac.replace(":", "");
  mac.toUpperCase();
  MQTTClientID = "ESPClient" + mac;
  MQTTclient.setCallback(MQTTcallback);


  server->on("/", []()
  {
	String WebOut;
	if (LoadDataFromFile("ShowMenueBar") != "off") WebOut = P_STR(P_AdminBarHTML);
	WebOut += RunningProgramGui();
	server->send(200, F("text/html"), WebOut);
  });


  server->on("/settings", []()
  {

	server->send(200, F("text/html"), SettingsPageHandeler());
  });


#if defined(BASIC_TFT)
  server->on("/vars", []()
  {
	String WebOut = P_STR(P_AdminBarHTML);
	String FixSpaces;
	if ( CheckIfLoggedIn() )
	{
	  WebOut = P_STR(P_LogInPage);
	}
	else
	{
	  WebOut += F("<div style='float: left;'>Variable Dump:");
	  for (int i = 0; i < TotalNumberOfVariables; i++)
	  {
		FixSpaces = AllMyVariables[i].getVar();
		FixSpaces.replace(' ' , char(160));
		if ( AllMyVariables[i].getName() != "") WebOut += String("<hr>" + AllMyVariables[i].getName() + " = " + (AllMyVariables[i].Format == PARSER_STRING ? "\"" : "") +
			  FixSpaces         + (AllMyVariables[i].Format == PARSER_STRING ? "\"" : "") );
	  }


	  WebOut += F("<hr></div><div style='float: right;'>Pin Stats");
	  for (byte i = 0; i <= 15; i++)
	  {
		if ( i < 6 | i > 11) WebOut += String("<hr>" + String(i) + " = " + PinListOfStatus[i] + "  , " + String(PinListOfStatusValues[i]));
	  }
	  WebOut += "</div>";
	}

	server->send(200, F("text/html"), WebOut);
  });
#endif

  server->on("/run", []()
  {
	clientTelnet.stop();
	BasicDebuggingOn = 0;
	telnetBranch = 0;
	String WebOut;
	RunningProgram = 1;
	RunningProgramCurrentLine = 0;
	WaitForTheInterpertersResponse = 0 ;
	numberButtonInUse = 0;
	HTMLout = "";
	TimerWaitTime = 0;
	TimerCBtime = 0;
	GraphicsEliments[0][0] = 0;
	WebOut = F(R"=====(  <meta http-equiv="refresh" content="0; url=./input?" />)=====");

	clear_stacks();
	server->send(200, F("text/html"), WebOut);
  });

  #if defined(BASIC_TFT)
  server->on("/debug", []()
  {
	clientTelnet.stop();
	BasicDebuggingOn = 1;
	telnetBranch = 0;
	String WebOut;
	RunningProgramCurrentLine = 0;
	WaitForTheInterpertersResponse = 0 ;
	numberButtonInUse = 0;
	HTMLout = "";
	TimerWaitTime = 0;
	TimerCBtime = 0;
	GraphicsEliments[0][0] = 0;
	WebOut = F(R"=====(  <meta http-equiv="refresh" content="0; url=./" />)=====");

	clear_stacks();
	server->send(200, F("text/html"), WebOut);

  });
  #endif
  
  

  server->on("/wsport.js", []()
  {
	server->send(200, F("text/html"), String("start('ws://' + location.hostname + ':" + LoadDataFromFile("wsport") + "/');"));
  });

  
  
  
  
#if defined(BASIC_TFT)
  server->on("/graphics.htm", []()
  {
	server->send(200, F("text/html"), BasicGraphics());
  });
#endif

  server->onFileUpload(handleFileUpdate);

  server->on("/filemng", []()
  {
	DoSomeFileManagerCode();
  });
  
  #if defined(BASIC_TFT)
  server->on("/filmanws.js", []()
  {
	server->send(200, F("text/html"), P_STR(P_FIleManWSJS));
  });
  #endif

  server->on("/edit", []()
  {
	WebGuiOff = 2;
	String WebOut;
	if (CheckIfLoggedIn())
	{
	  WebOut = P_STR(P_LogInPage);
	  server->send(200, F("text/html"), P_STR(P_AdminBarHTML) + WebOut );
	  WebGuiOff = 0;return;
	}
	else
	{
	  String CRLF = F("\r\n");
	  WaitForTheInterpertersResponse = 1;

	  String TextboxProgramBeingEdited;
	  //String ProgramName;
	  //WebOut = String("<form action='input'>" + HTMLout + "</form>");
	  WebOut = P_STR(P_EditorPageHTML);

	  if ( server->arg("open") == F("Open") )
	  {
		// really takes just the name for the new file otherwise it uses the previous one
		ProgramName = GetRidOfurlCharacters(server->arg("name"));

		ProgramName.trim();
		if (ProgramName == "")
		{
		  ProgramName = F("/default.bas");
		}
		ProgramName  = MakeSureFileNameStartsWithSlash(ProgramName );
		LoadBasicProgramFromFlash( ProgramName);
	  }
	  // the goal here is to replace the server send function by an equivalent that
	  // permit to handle big buffers; this is acheived using the "chunked transfer"
	  WebOut = P_STR(P_EditorPageHTML);
	  WebOut = WebOut.substring(0, WebOut.indexOf(F("*program txt*")) );
	  WebOut.replace(F("*program name*"), ProgramName);

	  server->sendContent(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection:close\r\nTransfer-Encoding: chunked\r\nAccess-Control-Allow-Origin: *\r\n\r\n"));
	  delay(0);
	  Serial.println(F("start sending"));
	  // each "chunk" is composed of :
	  // the size of the block (in hex) terminated by \r\n
	  server->sendContent(String(P_STR(P_AdminBarHTML).length(), 16) + CRLF);
	  // the block terminated by \r\n
	  server->sendContent(P_STR(P_AdminBarHTML) + CRLF);
	  /////// end of first chunk ///////////
	  delay(0);
	  server->sendContent(String(WebOut.length(), 16) + CRLF);
	  server->sendContent(WebOut + CRLF);
	  delay(0);
	  int iii;
	  int i;
	  fileOpenFail = 0;
	  TextboxProgramBeingEdited = BasicProgram(1);
	  for (i = 2; (i < program_nb_lines); i++)
	  {
		TextboxProgramBeingEdited = TextboxProgramBeingEdited + "\n" + BasicProgram(i);

		if (TextboxProgramBeingEdited.length() > 2048)
		{
		  server->sendContent(String(TextboxProgramBeingEdited.length(), 16) + CRLF);
		  server->sendContent(TextboxProgramBeingEdited + CRLF);
		  TextboxProgramBeingEdited = "";
		  delay(0);
		}
	  }
	  if (TextboxProgramBeingEdited.length() > 0)
	  {
		server->sendContent(String(TextboxProgramBeingEdited.length(), 16) + CRLF);
		server->sendContent(TextboxProgramBeingEdited + CRLF);
		delay(0);
	  }

	  WebOut = P_STR(P_EditorPageHTML);
	  WebOut = WebOut.substring(WebOut.indexOf(F("</textarea>")));
	  server->sendContent(String(WebOut.length(), 16) + CRLF);
	  server->sendContent(WebOut + CRLF);
	  // end of transmission
	  server->sendContent(F("0\r\n\r\n"));
	  server->sendContent(F("0\r\n\r\n"));
	  delay(0);
	  Serial.println(F("End of Open"));
	  WebGuiOff = 0;
	}

  });



  server->on("/editor.js", []() {
	server->send(200, F("text/html"), P_STR(P_editCodeJavaScript));
  });



  server->on("/WebSockets.js", []() {
	server->send(200, F("text/html"), P_STR(P_WebSocketsJS));
  });



  server->on("/codein", []() {
	//    ProgramName.trim();
	//    if (ProgramName == "")
	//    {
	//      ProgramName = F("/default.bas");
	//    }

	if (server->arg("SaveTheCode") == F("start"))
	{
	  inData = "end";
	  ExicuteTheCurrentLine();
	  HaltBasic("");
	  Serial.println(F("start save"));
	  ProgramName = GetRidOfurlCharacters(server->arg("FileName"));
	  if (ProgramName == "")ProgramName = F("/default.bas");
	  ProgramName.trim();
	  if (ProgramName[0] != '/') ProgramName = "/" + ProgramName;
	  OpenToWriteOnFlash( ProgramName );
	}

	if (server->arg("SaveTheCode") != F("yes") & server->arg("SaveTheCode") != F("start") & server->arg("SaveTheCode") != F("end"))
	{
	  String LineNoForWebEditorIn;
	  LineNoForWebEditorIn = server->arg("line");
	  int y = LineNoForWebEditorIn.toInt();
	  delay(0);
	  //Serial.println(server->arg("code"));
	  Serial.println(ProgramName + F("/") + String(y));
	  //BasicProgramWriteLine(y, GetRidOfurlCharacters(server->arg("code")));
	  WriteBasicLineOnFlash(GetRidOfurlCharacters(server->arg("code")));
	  delay(0);
	  noOfLinesForEdit = y;

	} 

	if (server->arg("SaveTheCode") == F("end"))
	{
	  // terminate the save
	  Serial.println(F("end of save!!"));
	  CloseWriteOnFlash();
	  HaltBasic("");
	  LoadBasicProgramFromFlash( ProgramName );
	}

	if (server->arg("SaveTheCode") == F("yes"))
	{
		HaltBasic("");
	}
	server->send(200, F("text/html"), F("good"));
  });





  server->on("/msg", []() {

	MsgBranchRetrnData = F("No MSG Branch Defined");

	if (msgbranch != 0)
	{
		byte oldWebGuiOff;
		oldWebGuiOff = WebGuiOff;
		Serial.println(msgbranch);
		//WebGuiOff = 1;
		RunningProgramCurrentLine = msgbranch;
		WaitForTheInterpertersResponse = 0;
		runTillWaitPart2();
		WebGuiOff = oldWebGuiOff;
	}

	server->send(200, F("text/html"), MsgBranchRetrnData);
  });





  server->on("/input", []() {
	server->send(200, F("text/html"), RunningProgramGui());
  });

  server->onNotFound ( []() {
	String fileNameToServeUp;
	fileNameToServeUp = GetRidOfurlCharacters(server->arg("file"));
	File mySuperFile = SPIFFS.open(String(F("/uploads/")) + fileNameToServeUp, "r");
	if (mySuperFile)
	{
	  server->streamFile(mySuperFile, getContentType(fileNameToServeUp));
	  //server->send(200, getContentType(fileNameToServeUp), mySuperFile.readString());

	}
	else
	{
	  server->send(200, F("text/html"), RunningProgramGui());
	}
	mySuperFile.close();
  });

  //LoadBasicProgramFromFlash("");


  if (  ConnectToTheWIFI(LoadDataFromFile("WIFIname"), LoadDataFromFile("WIFIpass"), LoadDataFromFile("ipaddress"), LoadDataFromFile("gateway"), LoadDataFromFile("subnetmask")) == 0)
  {
	if (LoadDataFromFile("APname") == "")
	{
	  CreateAP("", "", LoadDataFromFile("ipaddress"), LoadDataFromFile("gateway"), LoadDataFromFile("subnetmask"));
	}
	else
	{
	  CreateAP(LoadDataFromFile("APname"), LoadDataFromFile("APpass"), LoadDataFromFile("ipaddress"), LoadDataFromFile("gateway"), LoadDataFromFile("subnetmask"));
	}
  }


  Wire.begin(0, 2);

  //  keyboard.begin(14, 12); //For PS2 keyboard input
 #if defined(BASIC_TFT)
  StartUp_OLED();
  display.init();
  lcd.begin(16, 2); // initialize the lcd for 16 chars 2 lines and turn on backlight
#endif
  sensors.begin();

  if (!SPIFFS.exists(ProgramName))
  {
	OpenToWriteOnFlash(ProgramName);
	delay(0);
	WriteBasicLineOnFlash("  ");
	CloseWriteOnFlash();
  }
  
  
  LoadBasicProgramFromFlash( ProgramName);

  server->begin();
  RunningProgram = 0;
  WaitForTheInterpertersResponse = 1;
  StartUpProgramTimer();
  InitCommandParser(); // init the commands parser

  // start webSocket server
  webSocket->begin();
  webSocket->onEvent(webSocketEvent);
}


String SettingsPageHandeler()
{
  if ( server->arg("key") == LoadDataFromFile("LoginKey"))
  {
	LoggedIn = millis();
  }


  WaitForTheInterpertersResponse = 1;
  String WebOut = P_STR(P_AdminBarHTML) + P_STR(P_SettingsPageHTML);
  String staName = LoadDataFromFile(F("WIFIname"));
  String staPass = LoadDataFromFile(F("WIFIpass"));
  String apName = LoadDataFromFile(F("APname"));
  String apPass = LoadDataFromFile(F("APpass"));
  String LoginKey = LoadDataFromFile(F("LoginKey"));
  String ShowMenueBar = LoadDataFromFile(F("ShowMenueBar")); 
  String otaUrl = LoadDataFromFile(F("otaUrl"));
  String autorun = LoadDataFromFile(F("autorun"));
  String listenport = LoadDataFromFile(F("listenport"));
  String wsport = LoadDataFromFile(F("wsport"));
  String ipaddress = LoadDataFromFile(F("ipaddress"));
  String subnetmask = LoadDataFromFile(F("subnetmask"));
  String gateway = LoadDataFromFile(F("gateway"));
  
  
  

  // listening port - by default goes to 80 -
  if (listenport.toInt() == 0)
	listenport = F("80");

  //Serial.print("Loading Settings Files");

  if (millis() > LoggedIn + 600000 || LoggedIn == 0 )
  {
	WebOut = P_STR(P_LogInPage);
  }
  else
  {

	if ( server->arg("restart") == F("Restart") ) ESP.restart();

    #if defined(BASIC_TFT)
	if ( server->arg("update") == F("Update") )
	{

	  //        Serial.println(BasicOTAupgrade());
	  if (LoadDataFromFile("otaUrl") == "")
	  {
		t_httpUpdate_return  ret = ESPhttpUpdate.update(F("esp8266basic.smbisoft.com"), 80, F("/4M/ESP8266Basic.cpp.bin"));
		if (ret == HTTP_UPDATE_FAILED ) Serial.println(F("Update failed"));
	  }
	  else
	  {
		String URLtoGet = LoadDataFromFile("otaUrl");
		String ServerToConnectTo;
		String PageToGet;
		ServerToConnectTo = URLtoGet.substring(0, URLtoGet.indexOf("/"));
		PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));
		t_httpUpdate_return  ret = ESPhttpUpdate.update(ServerToConnectTo, 80, PageToGet);
		if (ret == HTTP_UPDATE_FAILED ) Serial.println(F("Update failed"));
	  }
	  //t_httpUpdate_return  ret = ESPhttpUpdate.update("cdn.rawgit.com", 80, "/esp8266/Basic/master/Flasher/Build/4M/ESP8266Basic.cpp.bin");

	}
    #endif

	if ( server->arg("save") == F("Save") )
	{
	  staName      = GetRidOfurlCharacters(server->arg("staName"));
	  staPass      = GetRidOfurlCharacters(server->arg("staPass"));
	  apName       = GetRidOfurlCharacters(server->arg("apName"));
	  apPass       = GetRidOfurlCharacters(server->arg("apPass"));
	  LoginKey     = GetRidOfurlCharacters(server->arg("LoginKey"));
	  ShowMenueBar = GetRidOfurlCharacters(server->arg("showMenueBar"));
	  otaUrl       = GetRidOfurlCharacters(server->arg("otaurl"));
	  autorun      = GetRidOfurlCharacters(server->arg("autorun"));
	  ipaddress   = GetRidOfurlCharacters(server->arg("ipaddress"));
	  subnetmask   = GetRidOfurlCharacters(server->arg("subnetmask"));
	  gateway   = GetRidOfurlCharacters(server->arg("gateway"));
	  listenport   = GetRidOfurlCharacters(server->arg("listenport"));
	  wsport   = GetRidOfurlCharacters(server->arg("wsport"));

	  SaveDataToFile("WIFIname" , staName);
	  SaveDataToFile("WIFIpass" , staPass);
	  SaveDataToFile("APname" , apName);
	  SaveDataToFile("APpass" , apPass);
	  SaveDataToFile("LoginKey" , LoginKey);
	  SaveDataToFile("ShowMenueBar" , ShowMenueBar);
	  SaveDataToFile("otaUrl" , otaUrl);
	  SaveDataToFile("autorun" , autorun);
	  SaveDataToFile("ipaddress" , ipaddress);
	  SaveDataToFile("subnetmask" , subnetmask);
	  SaveDataToFile("gateway" , gateway);
	  SaveDataToFile("listenport" , listenport);
	  SaveDataToFile("wsport" , wsport);

	}

	if ( server->arg("format") == F("Format") )
	{
	  // BasicFileOpened.close();
	  Serial.println(F("Formating "));
	  Serial.print(SPIFFS.format());
	}

	WebOut.replace(F("*sta name*"), staName);
	WebOut.replace(F("*sta pass*"), staPass);
	WebOut.replace(F("*ap name*"),  apName);
	WebOut.replace(F("*ap pass*"),  apPass);
	WebOut.replace(F("*LoginKey*"), LoginKey);
	WebOut.replace(F("*BasicVersion*"), P_STR(P_BasicVersion));
	WebOut.replace(F("*otaurl*"), otaUrl);

	WebOut.replace(F("*ipaddress*"), ipaddress);
	WebOut.replace(F("*subnetmask*"), subnetmask);
	WebOut.replace(F("*gateway*"), gateway);    
	WebOut.replace(F("*listenport*"), listenport);
	WebOut.replace(F("*wsport*"), wsport);

	if ( ShowMenueBar == F("off"))
	{
	  WebOut.replace(F("**checked**"), F("checked"));
	}
	else
	{
	  WebOut.replace(F("**checked**"), "");
	}

	if ( autorun == F("on"))
	{
	  WebOut.replace(F("**autorun**"), F("checked"));
	}
	else
	{
	  WebOut.replace(F("**autorun**"), "");
	}

  }
  return WebOut;
}



String getContentType(String filename) {
  String ret = F("text/plain");
  if (filename.endsWith(".gz"))
  {
	ret = F("application/x-gzip");
	filename = filename.substring(0, filename.length()-3);
	Serial.println("getcontenttype " + filename);
  }
  
	   if (filename.endsWith(".htm"))  return F("text/html");
  else if (filename.endsWith(".html")) return F("text/html");
  else if (filename.endsWith(".css"))  return F("text/css");
  else if (filename.endsWith(".js"))   return F("application/javascript");
  else if (filename.endsWith(".png"))  return F("image/png");
  else if (filename.endsWith(".gif"))  return F("image/gif");
  else if (filename.endsWith(".jpg"))  return F("image/jpeg");
  else if (filename.endsWith(".ico"))  return F("image/x-icon");
  else if (filename.endsWith(".xml"))  return F("text/xml");
  else if (filename.endsWith(".pdf"))  return F("application/x-pdf");
  else if (filename.endsWith(".zip"))  return F("application/x-zip");
  else if (filename.endsWith(".gz"))   return F("application/x-gzip");
  return ret;
}

void StartUpProgramTimer()
{
  int Startuptimer  = LoadDataFromFile("starttimer").toInt();
  if (Startuptimer == 0) Startuptimer = 30;
  pinMode(0, INPUT_PULLUP);  // set GPIO0 to input with pullup; so if float should read 1, 0 on ground
  while  (millis() < Startuptimer * 1000)
  {
	delay(0);
	//Serial.println(millis());
	server->handleClient();
	if (WaitForTheInterpertersResponse == 0) return;

	// MOD cicciocb April 2016
	// if the pin GPIO0 is taken to GND, the program will not start
	if (digitalRead(0) == 0)  return; // if the pin GPIO0 is at GND , stop the autorun
  }
  if (LoadDataFromFile("autorun") != "on")  return; // if the autorun option is disabled, returns
  Serial.println(F("Starting Default Program"));
  RunningProgram = 1;
  RunningProgramCurrentLine = 0;
  WaitForTheInterpertersResponse = 0 ;
  numberButtonInUse = 0;
  HTMLout = "";

  clear_stacks();
  return;
}



void DoSomeFileManagerCode()
{
  String WholeUploadPage = P_STR(P_UploadPage);
  String FileListForPage ;

  if (CheckIfLoggedIn())
  {
	WholeUploadPage = P_STR(P_LogInPage);
  }
  else
  {
	if (server->arg("Delete") != "")
	{
	  String FIleNameForDelete = server->arg("fileName");
	  FIleNameForDelete = GetRidOfurlCharacters(FIleNameForDelete);
	  Serial.println(FIleNameForDelete);
	  Serial.println(SPIFFS.remove(FIleNameForDelete));
	  //Serial.println(SPIFFS.remove("uploads/settings.png"));
	}

#if !defined(BASIC_TFT)
	Dir dir = SPIFFS.openDir(String(F("/") ));
	while (dir.next()) {
	  FileListForPage += String(F("<option>")) + dir.fileName() + String(F("</option>"));
	  delay(0);
	}

	WholeUploadPage.replace("*table*", FileListForPage);
#endif

	if (server->arg("View") != "")
	{
	  String FileNameToView = server->arg("fileName");
	  FileNameToView = GetRidOfurlCharacters(FileNameToView);
	  FileNameToView.replace("/uploads/", "");
	  WholeUploadPage = F(R"=====(  <meta http-equiv="refresh" content="0; url=./file?file=item" />)=====");
	  WholeUploadPage.replace("item", FileNameToView);
	}


	if (server->arg("Edit") != "")
	{
	  String FileNameToView = server->arg("fileName");
	  FileNameToView = GetRidOfurlCharacters(FileNameToView);
	  //FileNameToView.replace("/uploads/", "");
	  WholeUploadPage = F(R"=====(  <meta http-equiv="refresh" content="1; url=./edit?name=item&open=Open" />)=====");
	  WholeUploadPage.replace("item", FileNameToView);
	}

	if (server->arg("Rename") != "")
	{
	  String FileNameToView = server->arg("fileName");
	  FileNameToView = GetRidOfurlCharacters(FileNameToView);
	  String newfileName = server->arg("newfileName");
	  newfileName  = GetRidOfurlCharacters(newfileName );
	  WholeUploadPage = F(R"=====(<form id="filelist">Old Name<br><input type="text" name="fileName" value="*item name*"><br>New Name<br><input type="text" name="newfileName" value="*item name*"><input type="submit" value="NewName" name="Rename"></form>)=====");
	  WholeUploadPage.replace("*item name*", FileNameToView);
	  if (newfileName != "" )
	  {
		newfileName = MakeSureFileNameStartsWithSlash(newfileName);
		WholeUploadPage = F(R"=====(  <meta http-equiv="refresh" content="0; url=./filemng" />)=====");
		SPIFFS.rename(FileNameToView , newfileName);
	  }
	}
  }
  server->send(200, F("text/html"),  P_STR(P_AdminBarHTML) + WholeUploadPage );
}



void handleFileUpdate()
{
  //if (server->uri() != "/edit") return;
  HTTPUpload& upload = server->upload();
  if (upload.status == UPLOAD_FILE_START) {
	String filename = upload.filename;
	//DBG_OUTPUT_PORT.print("Upload Name: "); DBG_OUTPUT_PORT.println(filename);
	fsUploadFile = SPIFFS.open(String("/uploads/" + filename), "w");
	filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
	//DBG_OUTPUT_PORT.print("Upload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
	if (fsUploadFile)
	  fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
	if (fsUploadFile)
	  fsUploadFile.close();
	//DBG_OUTPUT_PORT.print("Upload Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}








String  getSerialInput()
{
  unsigned long  serialTimeOutStart = millis();
  bool donereceivinginfo = 0;
  Serial.println(">");

  String someInput;
  while (donereceivinginfo == 0)
  {
	if (serialTimeOutStart + SerialTimeOut < millis() & SerialTimeOut != 0) return someInput;
	delay(0);
	while (Serial.available() > 0)
	{
	  char recieved = Serial.read();
	  // Process message when new line character is recieved
	  if (recieved == '\n')
	  {
		Serial.println(someInput);
		donereceivinginfo = 1;
		return someInput;
	  }
	  someInput += recieved;
	}
  }
}






//String GetPS2input()
//{
//  String PS2inputString;
//
//
//  bool donereceivinginfo = 0;
//
//  while (donereceivinginfo == 0)
//  {
//    delay(0);
//    while (keyboard.available())
//    {
//      delay(0);
//      // read the next key
//      char c = keyboard.read();
//
//      // check for some of the special keys
//      if (c == PS2_ENTER) {
//        return PS2inputString;
//      } else if (c == PS2_TAB) {
//        //Serial.print("[Tab]");
//      } else if (c == PS2_ESC) {
//        return "";
//      } else if (c == PS2_PAGEDOWN) {
//        //Serial.print("[PgDn]");
//      } else if (c == PS2_PAGEUP) {
//        //Serial.print("[PgUp]");
//      } else if (c == PS2_LEFTARROW) {
//        //Serial.print("[Left]");
//      } else if (c == PS2_RIGHTARROW) {
//        //Serial.print("[Right]");
//      } else if (c == PS2_UPARROW) {
//        //Serial.print("[Up]");
//      } else if (c == PS2_DOWNARROW) {
//        //Serial.print("[Down]");
//      } else if (c == PS2_DELETE) {
//        //Serial.print("[Del]");
//      } else {
//        if (BasicDebuggingOn == 1) Serial.print(c);
//        PS2inputString += c;
//      }
//    }
//  }
//}






bool CheckIfLoggedIn()
{
  if (LoadDataFromFile("LoginKey") != "")
  {
	if ( millis() > LoggedIn + 600000 || LoggedIn == 0 )     return 1;
  }
  return 0;
}










void loop()
{

  RunBasicTillWait();
  delay(0);
  if (delaytime == 0)server->handleClient();
  webSocket->loop();
  
  if (MQTTActivated)
  { 
	  if (!MQTTclient.connected()) {
	  	MQTTreconnect();
	  }
    if (MQTTPublishTopic != "") 
    {
      bool MQTTSendSucess = 0;
      while (!MQTTSendSucess)
      {
        //Serial.print("Atempting to send");
        Serial.println(MQTTPublishMSG);
        MQTTSendSucess = MQTTclient.publish(MQTTPublishTopic.c_str(), MQTTPublishMSG.c_str(),MQTTPublishRetain);
        if (MQTTSendSucess) 
        {
          MQTTPublishTopic = "";
          //Serial.println(MQTTSendSucess);
        }
      }
    }
	  if (MQTTnewMsgReceived == 1 & MQTTBranch != "")
	  {     
			MQTTnewMsgReceived = 0;
      inData = String(" goto " + MQTTBranch + " ");
			WaitForTheInterpertersResponse = 0;
			ExicuteTheCurrentLine();
			runTillWaitPart2();
	  } 
    MQTTclient.loop();
  }

  sound.loop();
}



void RunBasicTillWait()
{
  webSocket->loop();
  if (delaytime > millis() && delaytime != 0) {return;} else {delaytime = 0;}
  runTillWaitPart2();
  if (RunningProgramCurrentLine > TotalNumberOfLines)
  {
	inData = String(" end");
	WaitForTheInterpertersResponse = 0;
	ExicuteTheCurrentLine();
	return;
  }



  
  
  if (TimerWaitTime + timerLastActiveTime <= millis() &&  TimerWaitTime != 0  & WaitForTheInterpertersResponse )
  {
	inData = String(" goto " + TimerBranch + " ");
	WaitForTheInterpertersResponse = 0;
	timerLastActiveTime = millis() ;
	ExicuteTheCurrentLine();
	runTillWaitPart2();
  }
  if (telnetBranch != 0 &&  clientTelnet.available())
  {
	RunningProgramCurrentLine = telnetBranch;
	WaitForTheInterpertersResponse = 0;
	runTillWaitPart2();
  }

  
  
  
  
  delay(0);
  for (int pinnn = 0; pinnn <= 15 ; pinnn++)
  {
	delay(0);
	//Serial.println(pinnn);
	if ((PinListOfStatus[pinnn] != "po") & ( PinListOfStatus[pinnn] != "pi") & ( PinListOfStatus[pinnn] != "pip") & (PinListOfStatus[pinnn] != "pwi") & (PinListOfStatus[pinnn] != "pwo") & (PinListOfStatus[pinnn] != "servo") & ( PinListOfStatus[pinnn] != ""))
	{
	  //Serial.println("Foud interupt pin");
	  if ( PinListOfStatusValues[pinnn] != UniversalPinIO("pi", String(pinnn), 0) | PinListOfStatusValues[pinnn] != UniversalPinIO("pip", String(pinnn), 0))
	  {
		inData = String(" goto " + PinListOfStatus[pinnn] + " ");
		WaitForTheInterpertersResponse = 0;
		//Serial.println(PinListOfStatus[pinnn]);
		ExicuteTheCurrentLine();
		runTillWaitPart2();
	  }
	}
  }
}


void runTillWaitPart2()
{
  
  if (NewGuiItemAddedSinceLastWait) WebSocketSend(  "guicls");NewGuiItemAddedSinceLastWait = 0;
  if (NewGraphicItemAddedSinceLastWait) WebSocketSend("gupdate");NewGraphicItemAddedSinceLastWait = 0;

  //Serial.println("Delay time" + String(delaytime));
  //Serial.println("CUrent Line" + String(RunningProgramCurrentLine));
  //Serial.println("Wait = " + String(WaitForTheInterpertersResponse));
  while (RunningProgram == 1 && RunningProgramCurrentLine < TotalNumberOfLines && WaitForTheInterpertersResponse == 0 && delaytime == 0)
  {
	//Serial.println(inData);

	delay(0);
	RunningProgramCurrentLine++;
	inData = BasicProgram(RunningProgramCurrentLine);
	inData = StripCommentsFromLine(inData);
	if (fileOpenFail == 1) inData  = "end";
	ExicuteTheCurrentLine();
	delay(0);
	CheckForUdpData();
	webSocket->loop();
  }
  if (RunningProgram == 1 && RunningProgramCurrentLine < TotalNumberOfLines && WaitForTheInterpertersResponse == 1 )
  {
	//Serial.print("sto in wait ");
	//Serial.println(RunningProgramCurrentLine);
	CheckForUdpData();
	webSocket->loop();
  }


}

String StripCommentsFromLine(String ret)
{
  // comment handling stuff
  // we try to remove all the comment from the line
  // the comment is defined as all the text following the ' (single quote) until the end of the line
  // the problem is that the ' can be also inside a string (text included between quotes " )
  // so we need to understand if the ' is inside quotes or not
  // let's go
  bool quotes = false;
  for (int i = 0; i < ret.length(); i++)
  {
	if (ret[i] == '"')
	  quotes = !quotes;

	if ( (ret[i] == '\'') && (quotes == false) )
	{
	  ret = ret.substring(0, i);  // cut the line at the current position (removes all the comments from the line
	  break;
	}
  }
  return ret;
}

void CheckForUdpData()
{
  int numBytes = udp.parsePacket();
  if ( numBytes)
  {
	//    Serial.print("Packet received ");
	//    Serial.print(RunningProgramCurrentLine);
	//    Serial.print("  ");
	//    Serial.println(udp.available());

	if (numBytes > 0)
	{
	  char Buffer[numBytes + 1];
	  UdpRemoteIP = udp.remoteIP();
	  UdpRemotePort = udp.remotePort();
	  delay(0);
	  udp.read(Buffer, numBytes);
	  Buffer[numBytes] = '\0'; // terminate the string with '\0'
	  UdpBuffer = String(Buffer);
	}

	//// test of gosub ///////
	if (UdpBranchLine > 0)
	{
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = UdpBranchLine + 1; // gosub after the udpbranch label
	  UdpBranchLine = - UdpBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  ////
  if (Serial.available() > 0)
  {
	delay(50); // insure that the data can be received
	//// put like a gosub ///////
	if (SerialBranchLine > 0)
	{
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = SerialBranchLine + 1; // gosub after the SerialBranch label
	  SerialBranchLine = - SerialBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  if ((swSer != NULL))
  {
	if (swSer->available() > 0)
	{
	  delay(50); // insure that the data can be received
	  //// put like a gosub ///////
	  if (Serial2BranchLine > 0)
	  {
		// if the program is in wait, it returns to the previous line to wait again
		return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
		WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
		RunningProgramCurrentLine = Serial2BranchLine + 1; // gosub after the SerialBranch label
		Serial2BranchLine = - Serial2BranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	  }
	}
  }
  if (irrecv->decode(&IRresults)) {
	//Serial.println(IRresults.value, HEX);
	//dump(&IRresults);
	//irrecv->resume(); // Receive the next value
	if (IRBranchLine > 0)
	{
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = IRBranchLine + 1; // gosub after the IRBranch label
	  IRBranchLine = - IRBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  if (TimerCBtime > 0)
  {
	if (millis() > (timercbLastActiveTime + TimerCBtime) )
	{
	  timercbLastActiveTime = millis();
	  // if the program is in wait, it returns to the previous line to wait again
	  return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
	  WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
	  RunningProgramCurrentLine = TimerCBBranchLine + 1; // gosub after the IRBranch label
	  TimerCBBranchLine = - TimerCBBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command
	}
  }
  #if defined(BASIC_TFT)
  if (TouchBranchLine > 0)
  {
	if ( millis() > (Touch_millis + 100) ) 
	{
	  int raw;
	  int tt = ReadTouchXY(0, &raw);
	  if ((tt != -1) && (Touch_p == -1))
	  {
//        Serial.print(tt & 0xffff);
//        Serial.print(":");
//        Serial.print(tt >> 16);
//        Serial.print(" ");
//        Serial.print(raw & 0xffff);
//        Serial.print(":");
//        Serial.print(raw >> 16);
//        Serial.print(" ");
//        Serial.println(Touch_p);
		touchX = tt & 0xffff;
		touchY = tt >> 16;
		touchY_raw = raw & 0xffff;
		touchY_raw = raw >> 16;
		// if the program is in wait, it returns to the previous line to wait again
		return_Stack.push(RunningProgramCurrentLine - WaitForTheInterpertersResponse); // push the current position in the return stack
		WaitForTheInterpertersResponse = 0;   //exit from the wait state but comes back again after the gosub
		RunningProgramCurrentLine = TouchBranchLine + 1; // gosub after the IRBranch label
		TouchBranchLine = - TouchBranchLine; // this is to avoid to go again inside the branch; it will be restored back by the return command  
	  }
	  delay(0);
	  Touch_millis = millis();
	  Touch_p = tt;
	}
  }  
  #endif
}

String getValueforPrograming(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
	0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
	if (data.charAt(i) == separator || i == maxIndex) {
	  found++;
	  strIndex[0] = strIndex[1] + 1;
	  strIndex[1] = (i == maxIndex) ? i + 1 : i;
	}
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}




String getValue(String data, char separator, int index)
{
  data = String(data + String(separator)+ String(separator));
  int maxIndex = data.length() - 1;
  int j = 0;
  byte WaitingForQuote;
  String chunkVal = "";
  String ChunkReturnVal;
  for (int i = 0; i <= maxIndex && j <= index; i++)
  {
	if (data[i] == '\"' )
	{
	  i++;
	  while (i <= maxIndex && data[i] != '\"' ) {
		chunkVal.concat(data[i]);
		i++;
		delay(0);
	  }
	}
	else if (data[i] == '|' )
	{
	  i++;
	  while (i <= maxIndex && data[i] != '|' ) {
		chunkVal.concat(data[i]);
		i++;
		delay(0);
	  }
	}
	else
	{
	  if (data[i] != separator) chunkVal.concat(data[i]);
	}

	if (data[i] == separator & data[i - 1] != separator)
	{
	  j++;
	  if (j > index)
	  {
		//chunkVal.trim();
		if (chunkVal != String(separator))
		{
		  ChunkReturnVal = chunkVal;
		  break;
		}
	  }
	  chunkVal = "";
	}
  }
  //    Serial.println("index");
  //    Serial.println(index);
  //    Serial.println(j);

  if (j == index + 1)
  {

	return ChunkReturnVal;
  }
}




String FetchOpenWeatherMapApi(String URLtoGet, String index)
{
  String ServerToConnectTo = URLtoGet.substring(0, URLtoGet.indexOf("/"));
  String PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));

  String s = "";
  char c;
  int cnt = 0;
  int phase = 0;
  int graffe = 0;

  const char lookforLIST[] = "\"list\":[";
  const char lookforBEGIN[] = "{\"";
  char ptr = 0;

  int list = index.toInt();

  if (list == 0)  // if the index is 0, it takes the first part, the root
	phase = 3;

  if (client.connect(ServerToConnectTo.c_str() , 80))
  {
	client.print(String("GET " + PageToGet + " HTTP/1.1\r\nHost: " +  ServerToConnectTo + "\r\n\r\n"));
	delay(300);

	while (client.available())
	{
	  delay(0);
	  //delay(1);
	  c = client.read();
	  delay(0);
	  //Serial.print(c);
	  switch (phase)
	  {
		case 0:
		  if (c == lookforLIST[ptr])
			ptr++;
		  else
			ptr = 0;

		  if (ptr == strlen(lookforLIST))
		  {
			phase = 1;
			list = list - 1;
			if (list == 0)
			{
			  phase = 2;
			}
			//Serial.println("phase 0 OK");
		  }
		  break;

		case 1:
		  if (c == '{')
			graffe++;
		  else if (c == '}')
			graffe--;

		  if (graffe == 0)
		  {
			if ( (c == ',') || (c == ']') )
			  list--;
		  }
		  if (list == 0)
		  {
			phase = 2;
			//Serial.println("phase 1 OK");
		  }
		  break;

		case 2:
		  s.concat(c);
		  cnt++;
		  if (c == '{')
			graffe++;
		  else if (c == '}')
			graffe--;

		  if ((graffe == 0) || (cnt > 600)) // max 600 chars
		  {
			//Serial.println("phase 2 OK");
			client.stop();
			return s;
		  }

		  break;

		case 3:    // search the beginning of the message starting with  {"
		  if (c == lookforBEGIN[ptr])
			ptr++;
		  else
			ptr = 0;

		  if (ptr == strlen(lookforBEGIN))
		  {
			cnt = 2;
			phase = 4;
			//Serial.println("phase 3 OK");
			s = F("{\"");
		  }
		  break;

		case 4:
		  s.concat(c);
		  cnt++;
		  if (c == lookforLIST[ptr])
			ptr++;
		  else
			ptr = 0;

		  if ( (ptr == strlen(lookforLIST)) || (cnt > 1000)) // max 1000 chars
		  {
			//Serial.println("phase 4 OK");
			client.stop();
			s.concat("]}");
			return s;
		  }
		  break;
	  }

	  if (client.available() == false)
	  {
		// if no data, wait for 300ms hoping that new data arrive
		delay(300);
	  }

	}//while
	client.stop();
	return F("END OF DATA REACHED");

  }
  client.stop();
  return "";
}








String FetchWebUrl(String URLtoGet, int PortNoForPage)
{
  if (PortNoForPage == 0) PortNoForPage = 80;
  String str = "             ";
  String ServerToConnectTo = URLtoGet.substring(0, URLtoGet.indexOf("/"));
  String PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));
  byte numberOwebTries = 0;
  // ServerToConnectTo ;
  //PageToGet = URLtoGet.substring(URLtoGet.indexOf("/"));

  //  Serial.println(ServerToConnectTo);
  //  Serial.println(PageToGet);


  if (client.connect(ServerToConnectTo.c_str() , PortNoForPage))
  {
	client.print(String("GET " + PageToGet + " HTTP/1.1\r\nHost: " +  ServerToConnectTo + "\r\n\r\n"));
	// wait for maximum 12 x 300msec = 3.6 seconds
	while ( ! client.available() && numberOwebTries < 12 ) {
	  numberOwebTries++;
	  delay(300);   // 300ms
	}

	while (client.available())
	{
	  delay(0);
	  if (str.endsWith(String("\r\n\r\n")))  str = "";

	  str.concat( (const char)client.read());
	  delay(0);
	  if (client.available() == false)
	  {
		// if no data, wait for 300ms hoping that new data arrive
		delay(300);
	  }

	}



	client.stop();
	return str.substring(0, str.indexOf(String(String(char(10)) + "0" )  ));
  }
  client.stop();
  return "";
}



void serialFlush()
{

  while (Serial.available() > 0)
  {
	delay(0);
	char t = Serial.read();
  }
}


void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
	String MQTTNewMSg = "";
  for (int i = 0; i < length; i++) 
  {
	  MQTTNewMSg = MQTTNewMSg + (char)payload[i];
  }
	MQTTnewMsgReceived  = 1;  
	MQTTlatestMsg = MQTTNewMSg;
}


void MQTTreconnect() {
  // Loop until we're reconnected
  byte reconnectAttempts = 0;
  bool success = false;
  while (!MQTTclient.connected()) {
    if (MQTTUser != "") 
      success = MQTTclient.connect(MQTTClientID.c_str(), MQTTUser.c_str(), MQTTPass.c_str());
    else
      success = MQTTclient.connect(MQTTClientID.c_str());
    if (success) {
	    MQTTclient.subscribe(MQTTSubscribeTopic.c_str());
	    Serial.println("connected");
    } else {
      reconnectAttempts++;
      if (reconnectAttempts > 3) return;
  	  Serial.print("failed, rc=");
  	  Serial.println(MQTTclient.state());
  	  delay(10);
  	}
  } 
}
