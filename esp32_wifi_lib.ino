//#include <WiFi.h>
//
//#define LED_BUILTIN 2
//
//char ssid[] = "myhomeismyKassel";     //  your network SSID (name)
//char pass[] = "Kassel1277";  // your network password
//
//void setup() {
//  Serial.begin(115200);
//  WiFi.mode(WIFI_STA);
//  
//  Serial.println("entered setup");
//
//  Serial.println("checked for wifi shield");
//  tryToConnect();
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print('.');
//    delay(1000);
//  }
//}
//
//void tryToConnect(){
//  // Check for available WiFis
//  
//  // Open BLE Connection
//
//
//  // Wait for WiFi Credentials
//
//  // Connect to WiFi
//  connectToWiFi();
//}
//
//void connectToWiFi() {
//  WiFi.begin(ssid, pass);
//  delay(10000);
//  if (WiFi.status() == WL_CONNECTED) {
//    digitalWrite(LED_BUILTIN, HIGH);
//  }
//}
//
//void loop() {
//  // Check for Connection Status
//  while (WiFi.status() == WL_CONNECTED)
//  {
//    Serial.println("Connected to wifi");
//    IPAddress myAddress = WiFi.localIP();
//    Serial.print("RRSI: ");
//    Serial.println(WiFi.RSSI());
//    Serial.print("SSID: ");
//    Serial.println(WiFi.SSID());
//    delay(1000);
//  }
//  
//}
//
//
//void stopSketch() {
//  noInterrupts();
//  while(true) {}
//}
