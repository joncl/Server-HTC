// 
// 
// 

#include "Ota.h"


void Ota::Begin()
{
	/*return index page which is stored in serverIndex */
	WebServer::Server()->on("/ota", HTTP_GET, [this](AsyncWebServerRequest *request)
	{
		//const char test1[] PROGMEM = "test this";
		AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", Ota::ota_content);
		response->addHeader("Connection", "close");
		request->send(response);
	});

	/*handling uploading firmware file */
	WebServer::Server()->on("/update", HTTP_POST, [this](AsyncWebServerRequest *request)
	{
		AsyncWebServerResponse *response = request->beginResponse_P(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
		response->addHeader("Connection", "close");
		request->send(response);
		esp_wifi_wps_disable(); ESP.restart();
	},
		[](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
		{
			Serial.printf("%d %d %d %d %d\n", index, len, final, Update.progress(), Update.remaining());
			if (index == 0)
			{
				Serial.printf("UploadStart: %s\n", filename.c_str());
				if (!Update.begin(UPDATE_SIZE_UNKNOWN))
				{
					Serial.println("Update begin failure!");
				}
			}
			if (Update.write(data, len) != len)
			{
				Update.printError(Serial);
			}
			else
			{
				Serial.printf("Write: %d bytes\n", len);
			}

			if (final) {
				Serial.printf("UploadEnd: %s, (%u)\n", filename.c_str(), index + len);
				if (Update.end(true))
				{
					Serial.printf("Update Success: %u\nRebooting...\n");
				}
				else
				{
					Update.printError(Serial);
				}
			}
		}
	);
}

const char Ota::ota_content[] PROGMEM = {
	"<html>\r\n"
	"<head>\r\n"
	"<meta charset='utf-8'>\r\n"
	"<title>Humidity-Relay</title>\r\n"
	"</head>\r\n"
	"<body style='font-family=\"verdana\"'>\r\n"
	"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>\r\n"
	"<div><h3>Firmware Update</h3></div>\r\n"
	"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>\r\n"
	"<input type='file' name='update'>\r\n"
	"<input type='submit' value='Update'>\r\n"
	"</form>\r\n"
	"<br/>\r\n"
	"<br/>\r\n"
	"<div style='font-family=\"verdana\"'><progress id='pbid' max='100' value='0'/></div>\r\n"
	"<div id='plid'></div>"
	"<script>"
	"var pb = document.getElementById('pbid');\r\n"
	"$('form').submit(function(e){\r\n"
	"e.preventDefault();\r\n"
	"var form = $('#upload_form')[0];\r\n"
	"var data = new FormData(form);\r\n"
	" $.ajax({\r\n"
	"url: '/update',\r\n"
	"type: 'POST',\r\n"
	"data: data,\r\n"
	"contentType:false,\r\n"
	"processData:false,\r\n"
	"xhr: function() {\r\n"
		"var xhr = new window.XMLHttpRequest();\r\n"

		"xhr.upload.addEventListener('progress', function(evt) {\r\n"
			"if (evt.lengthComputable) {\r\n"
				"var per = Math.round((evt.loaded / evt.total)*98);\r\n"
				"pb.value = per; }\r\n"
			"$('#plid').html(per + '%');"
			"}, false);\r\n"

		"xhr.addEventListener('readystatechange', function(evt) {\r\n"
			"if (this.readyState === 4) {\r\n"
				"pb.value = 100;\r\n"
				"$('#plid').html('upload complete, now rebooting.'); }\r\n"
			"}, false);\r\n"

		"return xhr;\r\n"
	"},\r\n"
	"success:function(d, s) {\r\n"
	"console.log('success!')\r\n"
	"},\r\n"
	"error: function (a, b, c) {\r\n"
	"}\r\n"
	"});\r\n"
	"});\r\n"
	"</script>\r\n"
	"</body>\r\n"
	"</html>\r\n"
};
