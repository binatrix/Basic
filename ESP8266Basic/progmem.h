PROGMEM const char P_InputFormText[] = R"=====( <input type="text" id="myid" name="input"><input type="submit" value="Submit" name="inputButton"><hr>)=====";
PROGMEM const char P_TextBox[] = R"=====(<input type="text" id="myid" name="variablenumber" value="variablevalue" onchange="objChange(this)">)=====";
PROGMEM const char P_passwordbox[] = R"=====( <input type="password" id="myid" name="variablenumber" value="variablevalue" onchange="objChange(this)">)=====";
PROGMEM const char P_Slider[] = R"=====( <input type="range" id="myid" name="variablenumber" min="minval" max="maxval" onchange="objChange(this)" value=variablevalue >)=====";
PROGMEM const char P_GOTObutton[] =  R"=====(<button id="myid" onclick="cmdClick(this)" name="gotonobranch">gotonotext</button>)=====";
PROGMEM const char P_GOTOimagebutton[] =  R"=====(<input type="image" onclick="cmdClick(this)" id="myid" src="/file?file=gotonotext" value="gotonotext" name="gotonobranch">)=====";
PROGMEM const char P_normalImage[] =  R"=====(<img src="/file?file=name">)=====";
PROGMEM const char P_javascript[] =  R"=====(<script src="/file?file=name"></script>)=====";
PROGMEM const char P_CSSscript[] =  R"=====(<link rel="stylesheet" type="text/css" href="/file?file=name">)=====";
PROGMEM const char P_CSSid[] =  R"=====(<style>#myid {*style*}</style>)=====";
PROGMEM const char P_CSSclass[] =  R"=====(<style>myid {*style*}</style>)=====";
PROGMEM const char P_DropDownList[] =  R"=====(<select name="variablenumber" id="myid" onchange="objChange(this)" size="theSize">options</select><script>document.getElementsByName("variablenumber")[0].value = "VARS|variablenumber";</script>)=====";
PROGMEM const char P_DropDownListOpptions[] =  R"=====(<option>item</option>)=====";
PROGMEM const char P_meter[] =  R"=====(<meter id="myid" name="variablenumber" value="variablevalue" min="minval" max="maxval"></meter>)=====";

PROGMEM const char P_MobileFreindlyWidth[] = R"=====(<meta name="viewport" content="width=device-width, initial-scale=1.0">)=====";

 #if defined(BASIC_TFT)
PROGMEM const char P_DebugPage[] =  R"=====(</script><script src='WebSockets.js'></script><script src='wsport.js'></script>
<div id="fooBar" style="float:left; width:20%;">Vars<hr></div>
<DIV style="float:left; width:35%;">
<button id="run" onclick="dcmdClick(this)">Run</button>
<button id="stop" onclick="dcmdClick(this)">Stop</button>
<button id="pause" onclick="dcmdClick(this)">Pause</button>
<button id="continue" onclick="dcmdClick(this)">Continue</button>
<input type="text" id="connection_status" value="Disconnected">
<button id="clear" onclick="logClear()">Clear</button>
Speed
<input type="text" id="speed" name="speed" value="*speed*"onchange="objChange(this)">
<br>Line No<input type="text" id="lno" value="line no">
<br><select id='code' size=35 style="width:100%"></select>
<textarea rows="20" style="width:100%;height:40%" id="log">log</textarea>
</DIV>
<div id="app" style="float:left; width:40%;">
)=====";
#else
PROGMEM const char P_DebugPage[] =  R"=====( )=====";
#endif

#if defined(BASIC_TFT)
PROGMEM const char P_AdminBarHTML[] = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" type="text/css" href="/file?file=skin.css">
<a href="./vars">[ VARS ]</a> 
<a href="./edit">[ EDIT ]</a>
<a href="./run">[ RUN ]</a>
<a href="./debug">[ DEBUG ]</a>
<a href="./settings">[ SETTINGS ]</a>
<a href="./filemng">[ FILE MANAGER ]</a>
<hr>)=====";
#else
PROGMEM const char P_AdminBarHTML[] = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<a href="./edit">[ EDIT ]</a>
<a href="./run">[ RUN ]</a>
<a href="./settings">[ SETTINGS ]</a>
<a href="./filemng">[ FILE MANAGER ]</a>
<hr>)=====";
#endif



 #if defined(BASIC_TFT)
PROGMEM const char P_UploadPage[] = R"=====(
<form method='POST' action='/filemng' enctype='multipart/form-data'>
<input type='file' name='Upload'>
<input type='submit' value='Upload'>
</form>
<form id="filelist">
<input type="submit" value="Delete" name="Delete">
<input type="submit" value="View" name="View">
<input type="submit" value="Edit" name="Edit">
<input type="submit" value="Rename" name="Rename">
</form>
<select name="fileName" size="25" form="filelist"></select>
</script><script src='filmanws.js'></script>
</script><script src='wsport.js'></script>
)=====";




PROGMEM const char P_FIleManWSJS[] =  R"=====(
function start(websocketServerLocation) {
	connection = new WebSocket(websocketServerLocation);
	connection.onopen = function() {
		connection.send("OK");
		connection.send('filelist')		
	};
	connection.onclose = function() {
		setTimeout(function() { start(websocketServerLocation)}, 1000);
	};

	connection.onmessage = function(e) {
		var res = e.data.split("~^`");

		
		if (res[0].toLowerCase() == "filename") {
			connection.send("OK");
			
			var x = document.getElementsByName("fileName")[0];
			var option = document.createElement("option");
			option.text = res[1];
			x.add(option);
			return;
		}
	};
}
var aliveme = setInterval(aliveTimer, 5000);
function aliveTimer() {
	connection.send("OK");
}
)=====";
#else
PROGMEM const char P_UploadPage[] = R"=====(
<form method='POST' action='/filemng' enctype='multipart/form-data'>
<input type='file' name='Upload'>
<input type='submit' value='Upload'>
</form>
<form id="filelist">
<input type="submit" value="Delete" name="Delete">
<input type="submit" value="View" name="View">
<input type="submit" value="Edit" name="Edit">
<input type="submit" value="Rename" name="Rename">
</form>
<select name="fileName" size="25" form="filelist">*table*</select>
<script>
function call() {
	var x = document.getElementsByName("fileName")[0];
	var optionVal = new Array();
	for (i = 0; i < x.length; i++) {
		optionVal.push(x.options[i].text);
	}
	for (i = x.length; i >= 0; i--) {
		x.remove(i);
	}
	optionVal.sort();
	for (var i = 0; i < optionVal.length; i++) {
		var opt = optionVal[i];
		var el = document.createElement("option");
		el.textContent = opt;
		el.value = opt;
		x.appendChild(el);
	}
}
</script>
<body onload="call()">
)=====";
#endif




//<a href="http://www.esp8266basic.com/help"  target="_blank">[ HELP ]</a>

PROGMEM const char P_EditorPageHTML[] =  R"=====(
<meta charset="utf-8">
<script src="editor.js"></script>
<script src="/file?file=codemirror.js.gz"></script>
<link rel="stylesheet" href="/file?file=codemirror.css.gz">
<form action='edit' id="usrform">
<input type="text" id="FileName" name="name" value="*program name*">
<input type="submit" value="Open" name="open">
</form>
<button onclick="SaveTheCode()">Save</button>
<br>
<textarea rows="30" style="width:100%" name="code" id="code">*program txt*</textarea><br>
<input type="text" id="Status" value="">
<script>
var editor = CodeMirror.fromTextArea(document.getElementById("code"), {
lineNumbers: true,
indentUnit: 4});
</script>
)=====";



PROGMEM const char P_WebSocketsJS[] =  R"=====(
function start(websocketServerLocation) {
	connection = new WebSocket(websocketServerLocation);
	connection.onopen = function() {
		connection.send('OK');
		connection.send("vars");
		document.getElementById("connection_status").value = "Connected";
	};
	connection.onclose = function() {
		setTimeout(function() {
			start(websocketServerLocation)
		}, 1000);
		document.getElementById("connection_status").value = "Disconnected";
	};

	connection.onmessage = function(e) {
		var res = e.data.split("~^`");
		if (res[0].toLowerCase() == "var") {
			connection.send("OK");
			for (i = 0; i < document.getElementsByName(res[1]).length; i++) {
				document.getElementsByName(res[1])[i].value = res[2];
			}

			return;
		}
		if (res[0].toLowerCase() == "varname") {
			connection.send("OK");
			document.getElementsByName("var" + res[1].toString())[0].value = res[2];
			return;
		}
		if (res[0].toLowerCase() == "code") {
			connection.send("OK");
			document.getElementById("lno").value = res[1];
			document.getElementById('code').value = res[1];
			return;
		}

		if (res[0].toLowerCase() == "print") {
			AddToBody('<hr>' + res[1]);
			connection.send("vars");
			return;
		}
		if (res[0].toLowerCase() == "gupdate") {
			document.getElementsByName('gra')[0].contentWindow.location.reload();
			return;
		}

		if (res[0].toLowerCase() == "guicls") {
			if (document.getElementById("app")) 
				{
				var bla = document.getElementById('app');
				bla.innerHTML = '';
				}
			else
				{
				document.body.innerHTML = '';
				}
			location.reload();
			return;
		}
		if (res[0].toLowerCase() == "wprint") {
			AddToBody(res[1]);
			connection.send("vars");
			return;
		} else if (res[0].toLowerCase() == "get") {
			var obj = document.getElementById(res[1]);
			if (obj == null) {
				connection.send("unknown object");
				return;
			}
			connection.send(obj.value);
			return;
		} else if (res[0].toLowerCase() == "log") {
			connection.send('OK');
			var log = document.getElementById("log");
			log.value = "\n" + res[1] + log.value;
			log.selectionStart = log.selectionEnd = log.value.length;
			return;
		} else if (res[0].toLowerCase() == "call") {
			connection.send('OK');
			eval(res[1] + '();');
			return;
		} else {
			// default 
			connection.send("KO");
		}
	};
}

function dcmdClick(e) {
	connection.send("cmd:" + e.id);
}

function cmdClick(e) {
	connection.send("guicmd:" + e.name);
}

function logClear() {
	document.getElementById('app').innerHTML ='';
	document.getElementById("log").value = '';
}

function objEvent(e) {
	connection.send("guievent:" + e.name + ":" + document.getElementById(e.id).value);
}

function objChange(e) {
	connection.send("guichange~" + e.name + "~" + document.getElementById(e.id).value);
}

var aliveme = setInterval(aliveTimer, 5000);

function aliveTimer() {
	connection.send("OK");
}

window.onload = function makeVarList() {
	for (xxx = 0; xxx < 101; xxx++) {
		add(xxx.toString(), "");
	}
	var arrayOfLines = localStorage.getItem("lastcode").split("\n");
	var sel = document.getElementById('code');
	for (i = 0; i < arrayOfLines.length; i++) 
	{ 
	  x = i + 1;
	  if (arrayOfLines[i] != "undefined")
	  {
		var opt = document.createElement('option');
		opt.innerHTML = pad('.....',i+1,true)+ ':    ' + arrayOfLines[i];
		opt.value = i+1;
		sel.appendChild(opt);
	  }
	}
}


function pad(pad, str, padLeft) {
  if (typeof str === 'undefined') 
	return pad;
  if (padLeft) {
	return (pad + str).slice(-pad.length);
  } else {
	return (str + pad).substring(0, pad.length);
  }
}

//Create text boxes for variables
function add(itemName, itemValue) {

	var element = document.createElement("input");
	element.setAttribute("type", "text");
	element.setAttribute("value", itemValue);
	element.setAttribute("name", "var" + itemName);
	element.setAttribute("style", "width:50%");

	var element1 = document.createElement("input");
	element1.setAttribute("type", "text");
	element1.setAttribute("value", itemValue);
	element1.setAttribute("name", itemName);
	element1.setAttribute("style", "width:50%");


	document.getElementById("fooBar").appendChild(element);
	document.getElementById("fooBar").appendChild(element1);
}


function removeSpecials(str) {
	var lower = str.toLowerCase();
	var upper = str.toUpperCase();

	var res = "";
	for(var i=0; i<lower.length; ++i) {
		if(lower[i] != upper[i] || lower[i].trim() === '')
			res += str[i];
	}
	return res;
}

function AddToBody(str){
if (document.getElementById("app")) 
	{
	var bla = document.getElementById('app');
	bla.innerHTML += str ;}
else
	{
	document.body.innerHTML +=str;
	}
}
)=====";






PROGMEM const char P_editCodeJavaScript[] =  R"=====(
function SaveTheCode() {
  if (typeof editor != "undefined") {editor.save();}
  var textArea = document.getElementById("code");
  localStorage.setItem("lastcode", textArea.value.trim());
  var arrayOfLines = textArea.value.trim().split("\n");
  httpGet("/codein?SaveTheCode=start&FileName="+document.getElementById("FileName").value);
  httpGet("/codein?SaveTheCode=yes");
  block = 0;
  document.getElementById("Status").value = "";
for (i = 0; i < arrayOfLines.length; i++) 
{ 
  x = i + 1;
  if (arrayOfLines[i] != "undefined")
  {
	stocca(encodeURIComponent(arrayOfLines[i]));
	
  }
}
stocca(">>-save_<<");
document.getElementById("Status").value = "Saved";
  httpGet("/codein?SaveTheCode=end");
alert("Saved");
}
var Sendy="";
var block = 0;
function stocca(s)
{
 Sendy += s;
 block++;
 if ((Sendy.length > 1024) || (s == ">>-save_<<"))
 {
  if (s == ">>-save_<<")
   Sendy = Sendy.substr(0,Sendy.length-10); 
   var WhatToSend = "/codein?line=" + block + "&code="+Sendy;
   httpGet(WhatToSend);
   Sendy = "";
 }
 else
 {
   Sendy += "%0D%0A";
 }
}
function httpGet(theUrl)
{
	var xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", theUrl, false ); // false for synchronous request
	xmlHttp.send( null );
	return xmlHttp.responseText;
}
function replaceAll(str, find, replace) {
  for (x = 0; x <= 10; x++) 
  {
	str = str.replace(find, replace);
  }
  return str;
}
)=====";


PROGMEM const char P_SettingsPageHTML[] =  R"=====(
<form action='settings' id="usrform">
*BasicVersion*
<style>div {width: 270px;text-align: right;}</style>
<div>
<b>Station Mode (Connect to router):</b>
<div>Name:<input type="text" name="staName" value="*sta name*"></div>
<div>Pass:<input type="password" name="staPass" value="*sta pass*"></div>
</div>
<div>
<b>Ap mode (brocast out its own ap):</b>
<div>Name:<input type="text" name="apName" value="*ap name*"></div>
<div>Pass (8 characters):<input type="password" name="apPass" value="*ap pass*"></div>
</div>
<div>
<b>IP (STA or AP mode):</b>
<div>IP address:<input type="text" name="ipaddress" value="*ipaddress*"></div>
<div>Subnet mask:<input type="text" name="subnetmask" value="*subnetmask*"></div>
<div>gateway:<input type="text" name="gateway" value="*gateway*"></div>
<div>HTTP port:<input type="text" name="listenport" value="*listenport*"></div>
<div>WS port:<input type="text" name="wsport" value="*wsport*"></div>
</div>
<div>
<div>Log In Key:<input type="password" name="LoginKey" value="*LoginKey*"></div>
<div>Menu bar Disable:<input type="checkbox" name="showMenueBar" value="off" **checked**></div>
<div>Run default.bas at startup:<input type="checkbox" name="autorun" value="on" **autorun**></div>
<div>OTA URL<input type="text" name="otaurl" value="*otaurl*"></div>
</div>
<div>
<input type="submit" value="Save" name="save">
<input type="submit" value="Format" name="format">
<input type="submit" value="Update" name="update">
<input type="submit" value="Restart" name="restart">
</div></form><br>)=====";







PROGMEM const char P_LogInPage[] =  R"=====(
<form action='settings' id="usrform">
Log In Key
<input type="password" name="key" value="">
<input type="submit" value="login" name="login">
</form>
)=====";


//Graphics HTML CODE
#if defined(BASIC_TFT)

PROGMEM const char P_GraphicsStartCode[] =  R"=====(<svg width="*wid*" height="*hei*">)=====";

PROGMEM const char P_GraphicsLineCode[] =  R"=====(<line x1="*x1*" y1="*y1*" x2="*x2*" y2="*y2*" stroke="*collor*"/>)=====";

PROGMEM const char P_GraphicsCircleCode[] =  R"=====(<circle cx="*x1*" cy="*y1*" r="*x2*" fill="*collor*"/>)=====";

PROGMEM const char P_GraphicsEllipseCode[] =  R"=====(<ellipse cx="*x1*" cy="*y1*" rx="*x2*" ry="*y2*" fill="*collor*"/>)=====";

PROGMEM const char P_GraphicsRectangleCode[] =  R"=====(<rect x="*x1*" y="*y1*" width="*x2*" height="*y2*" style="fill:*collor*"/>)=====";

PROGMEM const char P_GraphicsTextCode[] =  R"=====(<text x="*x1*" y="*y1*" style="fill:*collor*" transform="rotate(*x2* *x1*,*y1*)">*text*</text>)=====";
#endif

#define P_STR(x) String((__FlashStringHelper*)x)
