#include "MSP.h"
#include "FS.h"
#include "SD_MMC.h"
#include "WiFi.h"

// Global variables.
String logFileName = "/wardrive0.csv";
String fullDataFrame = "";
MSP msp;


// Look for the last log, then go one up.
void beginLog(fs::FS &fs) {
  int i = 0;
  
  while(fs.exists(logFileName)) {
      i++;
      logFileName = "/wardrive" + String(i) + ".csv";
  }
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    return;
  }
  file.print(message);
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    return;
  }
  file.print(message);
}

void setup() {
  Serial.begin(115200);
  msp.begin(Serial);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  SD_MMC.begin();
  beginLog(SD_MMC);
  writeFile(SD_MMC, logFileName.c_str(), "MAC,SSID,AuthMode,Channel,RSSI,lat,lon\n");


}

void loop() {
  
  msp_raw_gps_t frameGPS;
  if (msp.request(MSP_RAW_GPS, &frameGPS, sizeof(frameGPS))) {

    if (frameGPS.fixType == MSP_GPS_FIX_3D) {
  
      int n = WiFi.scanNetworks();
    
      if (n) {
        for (int i = 0; i < n; ++i) {
          fullDataFrame = WiFi.BSSIDstr(i) + ",";
          fullDataFrame += WiFi.SSID(i); fullDataFrame += ",";
          switch (WiFi.encryptionType(i)) {
          case WIFI_AUTH_OPEN:            fullDataFrame += "open,"; break;
          case WIFI_AUTH_WEP:             fullDataFrame += "WEP,"; break;
          case WIFI_AUTH_WPA_PSK:         fullDataFrame += "WPA,"; break;
          case WIFI_AUTH_WPA2_PSK:        fullDataFrame += "WPA2,"; break;
          case WIFI_AUTH_WPA_WPA2_PSK:    fullDataFrame += "WPA+WPA2,"; break;
          case WIFI_AUTH_WPA2_ENTERPRISE: fullDataFrame += "WPA2-EAP,"; break;
          case WIFI_AUTH_WPA3_PSK:        fullDataFrame += "WPA3,"; break;
          case WIFI_AUTH_WPA2_WPA3_PSK:   fullDataFrame += "WPA2+WPA3,"; break;
          case WIFI_AUTH_WAPI_PSK:        fullDataFrame += "WAPI,"; break;
          default:                        fullDataFrame += "unknown,";
          }
        fullDataFrame += WiFi.channel(i); fullDataFrame += ",";
        fullDataFrame += WiFi.RSSI(i); fullDataFrame +=",";
        fullDataFrame += String(frameGPS.lat) + "," + String(frameGPS.lon) + "\n";

        appendFile(SD_MMC, logFileName.c_str(), fullDataFrame.c_str());
        }
      }
      delay(10);
      WiFi.scanDelete();
    }
  }

delay(5000);
}
