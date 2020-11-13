# About
With the ESP8266 we will be able to read the temperature and humidity from the DHT11 sensor and upload it to Firebase realtime database then the data can be viewed from the web page from anywhere.

![SCREENSHOT](/HTML/IMG.png)

## To get it working

First, you will need to connect the sensor to the esp 
![SCREENSHOT](https://www.engineersgarage.com/wp-content/uploads/2/2/1/5/22159166/dht11-with-nodemcu_orig.png)  

^ Something like this (i did D2 though instead of D3)
then edit the WiFi and Firebase information in the Arduino sketch [Sketch](/Sketch.ino)
Then also add the Firebase config in [index.html](/HTML/index.html)
and you should be good to go :)
