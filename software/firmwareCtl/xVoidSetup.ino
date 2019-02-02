void setup() {

Serial.begin(115200);
Serial1.begin(250000);
Serial3.begin(500000);


    for(int x=0; x<rowCount; x++) {
        //Serial.print(rows[x]); Serial.println(" as input");
        pinMode(rows[x], INPUT);
    }
 
    for (int x=0; x<colCount; x++) {
        //Serial.print(cols[x]); Serial.println(" as input-pullup");
        pinMode(cols[x], INPUT_PULLUP);
    }

for(int s=0;s<nStrings;s++){ 
  pinMode(kickupPins[s],OUTPUT);
  digitalWrite(kickupPins[s], LOW);
}

for (int i=0; i<2; i++) {
        pinMode(sw1Pins[i], INPUT_PULLUP);
        digitalWrite(sw1Pins[i], HIGH);
}

redC = redC*bright;
greenC= greenC*bright;
blueC = blueC*bright;

pixels.begin();

  


for(int s=0;s<nStrings;s++){ 
  for(int f=0;f<nLedFrets;f++){ 
//stepState[s][f]=random(2);
  }}

Timer1.attachInterrupt(kickOff1);
Timer2.attachInterrupt(kickOff2);
Timer7.attachInterrupt(kickOff3);
Timer4.attachInterrupt(kickOff4);
Timer5.attachInterrupt(kickOff5);
Timer6.attachInterrupt(kickOff6);
Timer0.attachInterrupt(intClock);

delay(1000);
Timer0.start(20000);
}
