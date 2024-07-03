#include <Arduino.h>
#include <ArduinoOTA.h>

//Verificar o Windows Defender Firewall, pois ele bloqueia o OTA

class OTA_c
{
public:
  void otaStart(const char *hostname);  
};

inline void OTA_c::otaStart(const char *hostname) {
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    WSerial.println("Start updating " + type);
  });
  ArduinoOTA
  .onEnd([]() {
    WSerial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    WSerial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    WSerial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      WSerial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      WSerial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      WSerial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      WSerial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      WSerial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
}