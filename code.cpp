
#include <WiFi.h>
#include <ESP_Mail_Client.h>

// WiFi Credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Gmail SMTP Settings
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

// Your Gmail Login
#define AUTHOR_EMAIL "b.vivek.b001@gmail.com"
#define AUTHOR_PASSWORD "abcdefghijkjk1"

// Receiver Email
#define RECIPIENT_EMAIL "receiver@gmail.com"

#define PIR_PIN 2
#define LED_PIN 15

SMTPSession smtp;

unsigned long lastAlert = 0;
const unsigned long cooldown = 30000;   // 30 sec delay

void sendEmail() {

  SMTP_Message message;

  message.sender.name = "ESP32 Security";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "Motion Alert!";
  message.addRecipient("User", RECIPIENT_EMAIL);

  String textMsg = "Motion detected in your room!";
  message.text.content = textMsg.c_str();

  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;

  if (!smtp.connect(&config)) {
    Serial.println("Connection failed");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending Email");
  } else {
    Serial.println("Email Sent Successfully");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void loop() {

  if (digitalRead(PIR_PIN) == HIGH) {

    digitalWrite(LED_PIN, HIGH);

    if (millis() - lastAlert > cooldown) {
      Serial.println("Motion Detected!");
      sendEmail();
      lastAlert = millis();
    }

  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(200);
}
```
