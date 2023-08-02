#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Initialize Wifi connection to the router
char ssid[] = "Extension..,";     // your network SSID (name)
char password[] = "12345678www."; // your network key

// Initialize Telegram BOT
#define TELEGRAM_BOT_TOKEN "6624131525:AAFrKmYxQJs9BFAornuJRukS_3_-EXRB9G8"  // your Bot Token (Get from Botfather)

WiFiClientSecure client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, client);

#define LED_PIN   2  // D1
#define LED2_PIN  4  // D2
#define LED3_PIN  5  // D4
#define LED4_PIN  12 // D6

int delayBetweenChecks = 1000; // mean time between scan messages
unsigned long lastTimeChecked; // last time messages' scan has been done

int ledStatus[] = {0, 0, 0, 0}; // stores the status of each LED (0 - OFF, 1 - ON)

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  client.setInsecure();
}

void handleNewMessages(int numNewMessages) {
  Serial.println("ChaoukiNodeMcu:Message");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (from_name == "") from_name = "chaouki";

    if (text == "/ledon1") {
      digitalWrite(LED_PIN, HIGH);
      ledStatus[0] = 1;
      bot.sendMessage(chat_id, "Led1 is ON", "");
    }

    if (text == "/ledon2") {
      digitalWrite(LED2_PIN, HIGH);
      ledStatus[1] = 1;
      bot.sendMessage(chat_id, "Led2 is ON", "");
    }

    if (text == "/ledon3") {
      digitalWrite(LED3_PIN, HIGH);
      ledStatus[2] = 1;
      bot.sendMessage(chat_id, "Led3 is ON", "");
    }

    if (text == "/ledon4") {
      digitalWrite(LED4_PIN, HIGH);
      ledStatus[3] = 1;
      bot.sendMessage(chat_id, "Led4 is ON", "");
    }

    if (text == "/ledoff1") {
      ledStatus[0] = 0;
      digitalWrite(LED_PIN, LOW);
      bot.sendMessage(chat_id, "Led1 is OFF", "");
    }

    if (text == "/ledoff2") {
      ledStatus[1] = 0;
      digitalWrite(LED2_PIN, LOW);
      bot.sendMessage(chat_id, "Led2 is OFF", "");
    }

    if (text == "/ledoff3") {
      ledStatus[2] = 0;
      digitalWrite(LED3_PIN, LOW);
      bot.sendMessage(chat_id, "Led3 is OFF", "");
    }

    if (text == "/ledoff4") {
      ledStatus[3] = 0;
      digitalWrite(LED4_PIN, LOW);
      bot.sendMessage(chat_id, "Led4 is OFF", "");
    }

    if (text == "/status") {
      String statusMsg = "LED Status:\n";
      statusMsg += "Led1: " + String(ledStatus[0] ? "ON" : "OFF") + "\n";
      statusMsg += "Led2: " + String(ledStatus[1] ? "ON" : "OFF") + "\n";
      statusMsg += "Led3: " + String(ledStatus[2] ? "ON" : "OFF") + "\n";
      statusMsg += "Led4: " + String(ledStatus[3] ? "ON" : "OFF") + "\n";
      bot.sendMessage(chat_id, statusMsg, "");
    }

    if (text == "/start") {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Flash Led Bot example.\n\n";
      welcome += "/ledon1 : to switch Led1 ON\n";
      welcome += "/ledoff1 : to switch Led1 OFF\n";
      welcome += "/ledon2 : to switch Led2 ON\n";
      welcome += "/ledoff2 : to switch Led2 OFF\n";
      welcome += "/ledon3 : to switch Led3 ON\n";
      welcome += "/ledoff3 : to switch Led3 OFF\n";
      welcome += "/ledon4 : to switch Led4 ON\n";
      welcome += "/ledoff4 : to switch Led4 OFF\n";
      welcome += "/status : Returns current status of LEDs\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void loop() {
  if (millis() > lastTimeChecked + delayBetweenChecks) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("la reponse:");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    lastTimeChecked = millis();
  }
}
