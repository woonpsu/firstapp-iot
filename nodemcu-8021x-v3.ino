extern "C" {
#include "user_interface.h"
#include "wpa2_enterprise.h"
}

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "firstapp-iot.firebaseio.com"
#define FIREBASE_AUTH "rttttttttttttttttttttttttttttttttttttttttttttttt"

String xcolor = "WHITE";

//-------- WiFi config --------
// SSID to connect to
static const char* ssid = "PSU WiFi (802.1x)";
// Username for authentification
static const char* username = "your_user";
// Password for authentification
static const char* password = "your_password";

int ledPin1 = D1; // GPIO1 of ESP8266
int ledPin2 = D2; // GPIO2 of ESP8266

void setup() {
  //-------- setup ---------
  // Set
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  // เปิดไฟทุกสี จะได้แสงสีขาว
  analogWrite(D5, 255);
  analogWrite(D6, 255);
  analogWrite(D7, 255);

  //--------- connect WiFi ---------
  Serial.begin(115200);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.print("Try connect to ");
  Serial.println(ssid);

  wifi_station_disconnect();
  struct station_config wifi_config;
  memset(&wifi_config, 0, sizeof(wifi_config));
  strcpy((char*)wifi_config.ssid, ssid);
  wifi_station_set_config(&wifi_config);
  wifi_station_clear_cert_key();
  wifi_station_clear_enterprise_ca_cert();
  wifi_station_set_wpa2_enterprise_auth(1);
  wifi_station_set_enterprise_identity((uint8*)username, strlen(username));
  wifi_station_set_enterprise_username((uint8*)username, strlen(username));
  wifi_station_set_enterprise_password((uint8*)password, strlen(password));
  wifi_station_connect();

  Serial.print("Status: ");
  Serial.println(wifi_station_get_connect_status());

  // Wait for connection AND IP address from DHCP
  while (WiFi.status() != WL_CONNECTED) {
    delay(800);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.println("https://firstapp-iot.web.app/");

  // connect
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
} //---------- End setup ----------

void loop() { //---------- loop -----------
  // put your main code here, to run repeatedly:
  // check new status both LED
  digitalWrite(ledPin1, Firebase.getInt("device/led_control/LED1"));
  digitalWrite(ledPin2, Firebase.getInt("device/led_control/LED2"));
  xcolor = Firebase.getString("device/led_control/LEDCOLOR");
  if (Firebase.failed()) {
    // เปิดไฟทุกสี จะได้แสงสีขาว
    analogWrite(D5, 255);
    analogWrite(D6, 255);
    analogWrite(D7, 255);
    delay(2000);
  } else {
    Serial.println(xcolor);
  }
  if (xcolor == "BLUE") {
    // เปิดไฟสีน้ำเงิน
    analogWrite(D5, 255);
    analogWrite(D6, 0);
    analogWrite(D7, 0);
    delay(2000);
  }
  if (xcolor == "GREEN") {
    // เปิดไฟสีเขียว
    analogWrite(D5, 0);
    analogWrite(D6, 0255);
    analogWrite(D7, 0);
    delay(2000);
  }
  if (xcolor == "RED") {
    // เปิดไฟสีน้ำแดง
    analogWrite(D5, 0);
    analogWrite(D6, 0);
    analogWrite(D7, 255);
    delay(2000);
  }
  // ปิดไฟทั้งหมด
  analogWrite(D5, 0);
  analogWrite(D6, 0);
  analogWrite(D7, 0);
  delay(2000);
    
} //--------- End loop ----------

