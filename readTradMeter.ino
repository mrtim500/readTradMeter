
int sensorEenLaag = 0;
int sensorEenHoog = 1024;
bool sensorEenZwart = false;
bool sensorEenRondjeGeteld = false;
int sensorTweeLaag = 0;
int sensorTweeHoog = 1024;
bool sensorTweeZwart = false;
bool sensorTweeRondjeGeteld = false;
int rondjes;
int Wh = 0;
unsigned long tijd = 0;
unsigned long vorigeTijd = 0;
unsigned long vorigeTijdE = 0;
int watt = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorEenLaag  = analogRead(A1); //Uitlezen sensor 1 (links)
  sensorTweeLaag = analogRead(A0); //Uitlezen sensor 2 (rechts)

  if (sensorEenLaag > sensorEenHoog) { //Zwart vlak gezien
    sensorEenZwart = true;
  }

  if (sensorEenZwart == true && sensorEenLaag < sensorEenHoog ) { //Wit vlak na zwart
    sensorEenZwart = false;

    sensorEenRondjeGeteld = true;

    if (sensorTweeRondjeGeteld == true) { //we draaien terug?
      rondjes--;

      sensorEenRondjeGeteld = false;
      sensorTweeRondjeGeteld = false;

      tijd = (millis() - vorigeTijdE);
      vorigeTijdE = millis();
      watt = (( 3600000 / 375 ) * 1000 ) / tijd;
      Wh = ( rondjes * 1000 ) / 375;
    }
  }

  if (sensorTweeLaag > sensorTweeHoog) { //Zwart vlak gezien
    sensorTweeZwart = true;
  }

  if (sensorTweeZwart == true && sensorTweeLaag < sensorTweeHoog ) { //Wit vlak na zwart
    sensorTweeZwart = false;

    sensorTweeRondjeGeteld = true;

    if (sensorEenRondjeGeteld == true) { //we draaien vooruit?
      rondjes++;

      sensorEenRondjeGeteld = false;
      sensorTweeRondjeGeteld = false;

      tijd = (millis() - vorigeTijdE);
      vorigeTijdE = millis();
      watt = (( 3600000 / 375 ) * 1000 ) / tijd;
      Wh = ( rondjes * 12000 ) / 375; //rondjes * 12(5min x 12 = 1 uur) * 1000 / 375
    }
  }

  sensorEenHoog  = ( sensorEenLaag + 4 ); // +4 om verschil tussen wit en zwart te bepalen
  sensorTweeHoog = ( sensorTweeLaag + 4 ); // +4 om verschil tussen wit en zwart te bepalen

  if ((millis() - vorigeTijd) >= 300000) {
    vorigeTijd = millis();

    //serial output in json format
    String json = "{\"ronde\":\"";
    json += rondjes;
    json += "\",\"watt\":\"";
    json += watt;
    json += "\",\"Wh\":\"";
    json += String(Wh);
    json += "\"}";
    Serial.println(json);

    delay(100);

    rondjes = 0;
    watt    = 0; //anders zelfde waarde indien geen rondje binnen 5 min
    Wh      = 0; //anders zelfde waarde indien geen rondje binnen 5 min
  }

  delay(100);

}

