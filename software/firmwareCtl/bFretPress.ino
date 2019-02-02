byte rows[] = {8,9,10,11,12,13};
const int rowCount = sizeof(rows)/sizeof(rows[0]);
byte cols[] = {53,37,27,47,35,41,51,39,31,23,29,49,25,43,45,33,46,44,50,48,42,52};
const int colCount = sizeof(cols)/sizeof(cols[0]);
byte keys[colCount][rowCount];
byte lastKeys[colCount][rowCount];
int rowUsed[]={0,0,0,0,0,0};
int ctlUsed[]={0,0,0,0,0,0};
byte ctlFrets= 6;

void readFretboard() {
  for (int colIndex=0; colIndex < colCount; colIndex++) {
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);
      for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        byte rowCol = rows[rowIndex];
        pinMode(rowCol, INPUT_PULLUP);
      }
      for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
        byte rowCol = rows[rowIndex];
        keys[colIndex][rowIndex] = !digitalRead(rowCol);
        pinMode(rowCol, INPUT);
      }
      pinMode(curCol, INPUT);
  }

  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
    lastRowUsed[rowIndex]=rowUsed[rowIndex];  
  }
  static long lastChng[nStrings];
  static int maskT=100;
  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {   
    if(millis()-lastChng[rowIndex]>maskT){
      rowUsed[rowIndex]=0;
      ctlUsed[rowIndex]=0;
      bool used=0;
      for (int colIndex=colCount-1; colIndex >= 0; colIndex--) {  
        if(keys[colIndex][rowIndex]==1&&used==0){
          if(colIndex<colCount-ctlFrets)rowUsed[rowIndex]=colIndex+1;             
          if(colIndex>=colCount-ctlFrets){
            ctlUsed[rowIndex]=ctlFrets -(colIndex-(colCount-ctlFrets));
          }    
          lastChng[rowIndex]=millis();
          used=1;
        }
        lastKeys[colIndex][rowIndex]=keys[colIndex][rowIndex];
      }
    }
  }
}
 
