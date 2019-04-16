const int but_pin = D5;

void setup() {
  pinMode(but_pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  buttonToggleLed();
}

void buttonToggleLed() {
  static bool old_but_state = 0; // affected only 1st time

  if (digitalRead(but_pin) == 0 && old_but_state == 1) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);

    Serial.println("but press ");
  }

  //Serial.println(digitalRead(but_pin));
  old_but_state = digitalRead(but_pin);
}


//// the loop routine runs over and over again forever:
//int sensorBtn;
//int ledState;
//  // read the input on analog pin 0:
//  sensorBtn = digitalRead(but_pin);
//  ledState = digitalRead(LED_BUILTIN);
//
//  // LED à 1 eteind
//  // BUTTON à 1 eteind
//  //BTN 0 et LED 0 => eteind
//  if (sensorBtn == 0 ) {
//
//    Serial.println("SensorBTN_Ok");
//    if (ledState == 1) {
//      digitalWrite(LED_BUILTIN, HIGH);
//      Serial.println("ledState_0");
//    } else {
//      digitalWrite(LED_BUILTIN, LOW);
//      Serial.println("ledState_1");
//    }
//  }
//
//  // print out the value you read:
//  Serial.print("LED : " );
//  Serial.print(ledState);
//  Serial.println("BTN : ");
//  Serial.print(sensorBtn );
//  Serial.println("");
//  delay(500);        // delay in between reads for stability
