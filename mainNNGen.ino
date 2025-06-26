const int BUTTON_PIN = 2;
const int detailing = 100;
const int FREQ_HZ = 1000;
const int HZ_DELAY = 1000000 / (FREQ_HZ * detailing);
const float Period = (2 * PI) / detailing;

volatile int sig = 0;

int sig_main[3][detailing];

void IRAM_ATTR onButtonPress() {
    sig = (sig + 1) % 3;
}

void sig_gen(){
  for (int Angle = 0; Angle < detailing; Angle++) {
    sig_main[0][Angle] = (sin(Angle * Period) * 127) + 128;
    sig_main[1][Angle] = Angle * (255 / detailing);
    if (Angle < detailing / 2)
      sig_main[2][Angle] = Angle * 255 / (detailing / 2);
    else
      sig_main[2][Angle] = 255 - ((Angle - detailing / 2) * 255 / (detailing / 2));
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPress, FALLING);
  Serial.begin(115200);
  sig_gen();
}

void loop() {
  if (Serial.available() > 0) {
    char ch = Serial.read();
    if (ch >= '0' && ch <= '2') {
      sig = ch - '0';
    }
  }
  for (int i = 0; i < detailing; i++) {
    dacWrite(25, sig_main[sig][i]);
    dacWrite(26, sig_main[1][i]);
    delayMicroseconds(HZ_DELAY);
  }
}
