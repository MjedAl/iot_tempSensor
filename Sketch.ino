#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <SimpleDHT.h>

#define FIREBASE_HOST "" // Change me
#define FIREBASE_AUTH "" // Change me
#define WIFI_SSID "" // Change me
#define WIFI_PASSWORD "" // Change me

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

FirebaseJson json;

int C;   // temperature C readings are integers
int H;   // humidity readings are integers

SimpleDHT11 dht11(D2);

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);

}

String path = "/playground";
byte temperature = 0;
byte humidity = 0;

void loop()
{
  // read status from firebase
  if (Firebase.getInt(firebaseData, path + "/readDHT11"))
  {
    if (firebaseData.intData() == 1) {

      int err = SimpleDHTErrSuccess;
      if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
        Serial.print("Read DHT11 failed, err=");
        Serial.println(err);
      } else {
        // Sample OK
        json.clear().add("temperature", ((int)temperature));
        json.add("humidity", ((int)humidity));

        if (!Firebase.setJSON(firebaseData, path + "/dht11", json))
        {
          Serial.println("FAILED");
          Serial.println("REASON: " + firebaseData.errorReason());
          Serial.println("------------------------------------");
          Serial.println();
        }
      }
    }else{
      Serial.println("Stopped from Firebase");
    }
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  // DHT11 sampling rate is 1HZ.
  delay(1500);
}
