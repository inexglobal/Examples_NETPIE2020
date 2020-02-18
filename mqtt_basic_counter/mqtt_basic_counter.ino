#include <WiFi.h>
#include <PubSubClient.h>  // ผนวกไฟล์ไลบรารี MQTT
const char* ssid = "--your ssid--"; // ชื่อตัวปล่อยสัญญาณ Wi-Fi
const char* password = "--your password--"; //รหัสผ่านตัวปล่อยสัญญาณ Wi-Fi
const char* mqtt_server = "broker.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "--your Client ID--"; // Client ID ที่ได้จากการสร้าง Device
const char* mqtt_username = "--your Token--"; // Token ที่ได้จากการสร้าง Device
const char* mqtt_password = "--your Secret--"; // Secret ได้จากการสร้าง Device
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;
void reconnect() {
  while (!client.connected()) { // วนทำซ้ำจนกว่าจะเชื่อมต่อได้
    Serial.print("Attempting MQTT connection…");
    // ตรวจสอบการเชื่อมต่อไป broker Netpie
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
}
void loop() {
  if (!client.connected()) { //ตรวจสอบการเชื่อมต่อ
    reconnect();  //เชื่อมต่อใหม่
  }
  client.loop();  //ฟังก์ชั่นค่อยตรวจสอบการเชื่อมต่อและข้อมูลที่ส่งเข้ามา
  long now = millis();
  if (now - lastMsg > 5000) { //ตรวจสอบเวลาทุก 5 วินาทีจะทำในเงื่อนไข 1 ครั้ง
    lastMsg = now;
    ++value;  //เพิ่มค่าตัวแปร value ขึ้นที่ละ 1 ค่า
    String strValue = String(value); //เปลี่ยนตัวแปร value ให้อยู่ในรูปแบบชนิดสตริง
    // ส่งค่าจำนวนนับไปยัง broker Netpie
    client.publish("@msg/counter", (char*)strValue.c_str());
    Serial.println(value);
  }
}
