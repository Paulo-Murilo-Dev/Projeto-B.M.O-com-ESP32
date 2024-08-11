#include <WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#define RELAY_PIN 13
#define BUTTON_PIN 2
#define LED_PIN 12 
// o pin 12 é só um led para mostrar q o sistema está operando e que ele esta enviando sinal para o portão

const char* ssid = "<SEU WIFI>";
const char* password = "<SUA SENHA>";

#define APP_KEY "<SUA API KEY>"
#define APP_SECRET "SUA API SECRET KEY"
#define DEVICE_ID "SEU DEVICE ID"
// QUANDO FOR CRIAR O SEU APARELHO NO SITE DA SINCRIC
// PFV USE SWITCH, SE N VAI TER Q MUDAR AS FUÇÕES E ESTADOS
// O SWITCH NA MINHA OPINIÃO FUNCIONA MUITO MELHOR Q O GARAGE DOOR
// BOA SORTE
bool switchState = true;
bool lastButtonState = HIGH;

bool onSwitchState(const String &deviceId, bool &state) {
  Serial.printf("Comando recebido: %s\n", state ? "Abrir" : "Fechar");
  
  if (state) {
    digitalWrite(RELAY_PIN, switchState ? HIGH : LOW);
    Serial.println("Ligando switch...");
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, LOW);
    delay(500);q
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, HIGH);
  }
  Serial.println(state ? "Switch ligado." : "Switch desligado.");
  
  return true;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);
     
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao WiFi");

  SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
  mySwitch.onPowerState(onSwitchState);

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void loop() {
  SinricPro.handle();

  bool buttonState = digitalRead(BUTTON_PIN);
  //Esse bom tão funcionava bem apra testar o app sem precisar
  //ficar subindo o codigo toda hora, para mim q sou iniciante é muito melhor
  //fique avontade para remover, eu não o removi pois ele ainda é um bom reseter.
  if (buttonState == LOW && lastButtonState == HIGH) {
    switchState = !switchState;
    Serial.printf("Botão pressionado. Switch %s.\n", switchState ? "ligado" : "desligado");
    digitalWrite(RELAY_PIN, switchState ? HIGH : LOW);
  }
  lastButtonState = buttonState;
}

