#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define LED 5

const char* ssid = "Extension..,";
const char* password = "12345678www.";

const char BotToken[] = "6657387859:AAHGtQdHiNY17FS35g8lmSSo9NclOMqwYng";

WiFiClientSecure client;
UniversalTelegramBot bot(BotToken, client);

void setup()
{
  Serial.begin(115200);
  while (!Serial) {} //Start running when the serial is open
  delay(3000);
  Serial.print("Connecting WiFi.");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  bot.begin();
  pinMode(LED, OUTPUT);
}
void loop() 
{  
  message m = bot.getUpdates(); // Read new messages  
  if (m.text.equals("on")) 
  {  
    digitalWrite(LED, 1);   
    bot.sendMessage(m.chat_id, "LED is ON");
    Serial.println("LED is ON");  
  }  
  else if (m.text.equals("off")) 
  {  
    digitalWrite(LED, 0);   
    bot.sendMessage(m.chat_id, "LED is OFF");  
    Serial.println("LED is OFF");
  } 
}
