#define X_STEP_PIN         15
#define X_DIR_PIN          21
#define X_MIN_PIN          18
#define X_MAX_PIN           -2
#define X_ENABLE_PIN       14

void setup()
{
pinMode(X_STEP_PIN, OUTPUT);
pinMode(X_DIR_PIN, OUTPUT);
pinMode(X_ENABLE_PIN, OUTPUT);
digitalWrite(X_DIR_PIN,HIGH);
Serial.begin(9600);
}


