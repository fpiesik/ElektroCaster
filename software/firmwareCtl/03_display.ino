const byte disp_cmdStr = 1;
const byte disp_cmdInt = 2;
const byte disp_cmdLng = 3;
const byte disp_cmdFrm = 4;
const byte disp_cmdBox = 5;
const byte disp_cmdRFrm = 6;
const byte disp_cmdRBox = 7;
const byte disp_cmdLine = 8;
const byte disp_cmdCircle = 9;
const byte disp_cmdDisk = 10;
const byte disp_cmdPix = 11;
const byte disp_cmdColor = 12;
const byte disp_cmdClr = 13;
const byte disp_cmdBuf = 14;
const byte disp_maxCmdsPerChunk = 3;
const int disp_maxJobCmds = 96;

struct DispCmd {
  byte kind;
  int a;
  int b;
  int c;
  int d;
  int e;
  long l;
  const char* str;
};

bool displayJobStale();
bool displayStateChanged();
void cancelDisplayJob();
void buildDisplayJob();
void sendDisplayJobChunk();
bool displayClockSafeFor(unsigned long needed);
unsigned long displayClockSlackMicros();
void queueDisplayCmd(byte kind, int a, int b, int c, int d, int e, long l, const char* str);
void sendDisplayCmd(int idx);
void disp_sendStr(int x, int y, const char* str);
void disp_sendInt(int x, int y, int val);
void disp_sendLng(int x, int y, long val);
void disp_sendFrm(int xp, int yp, int xs, int ys);
void disp_sendBox(int xp, int yp, int xs, int ys);
void disp_sendRFrm(int xp, int yp, int xs, int ys, int r);
void disp_sendRBox(int xp, int yp, int xs, int ys, int r);
void disp_sendLine(int x0, int y0, int x1, int y1);
void disp_sendCircle(int x, int y, int r);
void disp_sendDisk(int x, int y, int r);
void disp_sendPix(int x, int y);
void disp_sendColor(int c);
void disp_sendClr();
void disp_sendBuf();

DispCmd disp_job[disp_maxJobCmds];
int disp_jobLen = 0;
int disp_jobPos = 0;
bool disp_capture = false;
bool disp_jobActive = false;
byte disp_jobOpMode = 255;
byte disp_jobFbrdMode = 255;
byte disp_jobFbrdSeqVHld = 255;
unsigned long disp_jobStartMicros = 0;
unsigned long disp_jobSendMicros = 0;
unsigned long disp_lastJobDurationMicros = 0;
unsigned long disp_lastJobCommandCount = 0;
unsigned long disp_lastClockSlackMicros = 0;

void updDisplay(){
  if (displayJobStale()) cancelDisplayJob();

  if (!disp_jobActive){
    if (!displayStateChanged() && millis()-disp_frameTimer <= disp_frameInt) return;
    buildDisplayJob();
  }

  sendDisplayJobChunk();
}

bool displayJobStale(){
  return disp_jobActive && (disp_jobOpMode != opMode || disp_jobFbrdMode != fbrdMode || disp_jobFbrdSeqVHld != fbrdSeqVHld);
}

bool displayStateChanged(){
  return disp_jobOpMode != opMode || disp_jobFbrdMode != fbrdMode || disp_jobFbrdSeqVHld != fbrdSeqVHld;
}

void cancelDisplayJob(){
  disp_jobActive = false;
  disp_capture = false;
  disp_jobLen = 0;
  disp_jobPos = 0;
}

void buildDisplayJob(){
  disp_jobLen = 0;
  disp_jobPos = 0;
  disp_jobOpMode = opMode;
  disp_jobFbrdMode = fbrdMode;
  disp_jobFbrdSeqVHld = fbrdSeqVHld;
  disp_jobStartMicros = micros();
  disp_jobSendMicros = 0;
  disp_lastClockSlackMicros = 0;

  disp_capture = true;
  disp_Clr();
  switch (opMode){
    case strSetup_opMode:
      strSetup_updDisp();
      break;
    case strArp_opMode:
      if(fbrdMode==0)strArp_updDisp();
      if(fbrdMode==1)strArp_updDisp();
      break;
    case genSq_opMode:
      if(fbrdMode==0&&fbrdSeqVHld==0)scls_updDisp();
      if(fbrdMode==1||fbrdSeqVHld==1)genSq_updDisp();
      break;
    case genSq_opMode+1:
      if(fbrdMode==0&&fbrdSeqVHld==0)scls_updDisp();
      if(fbrdMode==1||fbrdSeqVHld==1)genSq_updDisp();
      break;
    case genSq_opMode+2:
      if(fbrdMode==0&&fbrdSeqVHld==0)scls_updDisp();
      if(fbrdMode==1||fbrdSeqVHld==1)genSq_updDisp();
      break;
  }
  disp_Buf();
  disp_capture = false;

  disp_jobActive = disp_jobLen > 0;
}

void sendDisplayJobChunk(){
  if (!disp_jobActive) return;

  byte sent = 0;
  while (disp_jobPos < disp_jobLen && sent < disp_maxCmdsPerChunk){
    if (!displayClockSafeFor(disp_lastDurationMicros + disp_clockGuardMicros)) return;
    unsigned long slack = displayClockSlackMicros();
    if (slack > disp_lastClockSlackMicros) disp_lastClockSlackMicros = slack;

    unsigned long cmdStartMicros = micros();
    sendDisplayCmd(disp_jobPos);
    disp_jobSendMicros += micros()-cmdStartMicros;
    disp_jobPos++;
    sent++;
  }

  if (disp_jobPos >= disp_jobLen){
    disp_lastDurationMicros = disp_jobSendMicros;
    disp_lastJobDurationMicros = micros()-disp_jobStartMicros;
    disp_lastJobCommandCount = disp_jobLen;
    disp_frameTimer = millis();
    disp_jobActive = false;
  }
}

bool displayClockSafe(){
  return displayClockSafeFor(disp_lastDurationMicros+disp_clockGuardMicros);
}

bool displayClockSafeFor(unsigned long needed){
  if (extClk == 0 && clckOn == 1){
    unsigned long elapsed = micros()-intClockTimer;
    if (elapsed >= intClockInt) return false;
    if (intClockInt-elapsed <= needed) return false;
  }
  return true;
}

unsigned long displayClockSlackMicros(){
  if (extClk == 0 && clckOn == 1){
    unsigned long elapsed = micros()-intClockTimer;
    if (elapsed >= intClockInt) return 0;
    return intClockInt-elapsed;
  }
  return 0;
}

void queueDisplayCmd(byte kind, int a, int b, int c, int d, int e, long l, const char* str){
  if (!disp_capture) return;
  if (disp_jobLen >= disp_maxJobCmds) return;
  disp_job[disp_jobLen].kind = kind;
  disp_job[disp_jobLen].a = a;
  disp_job[disp_jobLen].b = b;
  disp_job[disp_jobLen].c = c;
  disp_job[disp_jobLen].d = d;
  disp_job[disp_jobLen].e = e;
  disp_job[disp_jobLen].l = l;
  disp_job[disp_jobLen].str = str;
  disp_jobLen++;
}

void sendDisplayCmd(int idx){
  switch (disp_job[idx].kind){
    case disp_cmdStr: disp_sendStr(disp_job[idx].a, disp_job[idx].b, disp_job[idx].str); break;
    case disp_cmdInt: disp_sendInt(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c); break;
    case disp_cmdLng: disp_sendLng(disp_job[idx].a, disp_job[idx].b, disp_job[idx].l); break;
    case disp_cmdFrm: disp_sendFrm(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c, disp_job[idx].d); break;
    case disp_cmdBox: disp_sendBox(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c, disp_job[idx].d); break;
    case disp_cmdRFrm: disp_sendRFrm(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c, disp_job[idx].d, disp_job[idx].e); break;
    case disp_cmdRBox: disp_sendRBox(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c, disp_job[idx].d, disp_job[idx].e); break;
    case disp_cmdLine: disp_sendLine(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c, disp_job[idx].d); break;
    case disp_cmdCircle: disp_sendCircle(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c); break;
    case disp_cmdDisk: disp_sendDisk(disp_job[idx].a, disp_job[idx].b, disp_job[idx].c); break;
    case disp_cmdPix: disp_sendPix(disp_job[idx].a, disp_job[idx].b); break;
    case disp_cmdColor: disp_sendColor(disp_job[idx].a); break;
    case disp_cmdClr: disp_sendClr(); break;
    case disp_cmdBuf: disp_sendBuf(); break;
  }
}

//display functions
void disp_Str(int x, int y, const char* str) {
  if (disp_capture) queueDisplayCmd(disp_cmdStr, x, y, 0, 0, 0, 0, str);
  else disp_sendStr(x, y, str);
}

void disp_Int(int x, int y, int val) {
  if (disp_capture) queueDisplayCmd(disp_cmdInt, x, y, val, 0, 0, 0, 0);
  else disp_sendInt(x, y, val);
}

void disp_Lng(int x, int y, long val) {
  if (disp_capture) queueDisplayCmd(disp_cmdLng, x, y, 0, 0, 0, val, 0);
  else disp_sendLng(x, y, val);
}

void disp_Frm(int xp, int yp, int xs, int ys) {
  if (disp_capture) queueDisplayCmd(disp_cmdFrm, xp, yp, xs, ys, 0, 0, 0);
  else disp_sendFrm(xp, yp, xs, ys);
}

void disp_Box(int xp, int yp, int xs, int ys) {
  if (disp_capture) queueDisplayCmd(disp_cmdBox, xp, yp, xs, ys, 0, 0, 0);
  else disp_sendBox(xp, yp, xs, ys);
}

void disp_RFrm(int xp, int yp, int xs, int ys, int r) {
  if (disp_capture) queueDisplayCmd(disp_cmdRFrm, xp, yp, xs, ys, r, 0, 0);
  else disp_sendRFrm(xp, yp, xs, ys, r);
}

void disp_RBox(int xp, int yp, int xs, int ys, int r) {
  if (disp_capture) queueDisplayCmd(disp_cmdRBox, xp, yp, xs, ys, r, 0, 0);
  else disp_sendRBox(xp, yp, xs, ys, r);
}

void disp_Line(int x0, int y0, int x1, int y1) {
  if (disp_capture) queueDisplayCmd(disp_cmdLine, x0, y0, x1, y1, 0, 0, 0);
  else disp_sendLine(x0, y0, x1, y1);
}

void disp_Circle(int x, int y, int r) {
  if (disp_capture) queueDisplayCmd(disp_cmdCircle, x, y, r, 0, 0, 0, 0);
  else disp_sendCircle(x, y, r);
}

void disp_Disk(int x, int y, int r) {
  if (disp_capture) queueDisplayCmd(disp_cmdDisk, x, y, r, 0, 0, 0, 0);
  else disp_sendDisk(x, y, r);
}

void disp_Pix(int x, int y) {
  if (disp_capture) queueDisplayCmd(disp_cmdPix, x, y, 0, 0, 0, 0, 0);
  else disp_sendPix(x, y);
}

void disp_Color(int c) {
  if (disp_capture) queueDisplayCmd(disp_cmdColor, c, 0, 0, 0, 0, 0, 0);
  else disp_sendColor(c);
}

void disp_Clr() {
  if (disp_capture) queueDisplayCmd(disp_cmdClr, 0, 0, 0, 0, 0, 0, 0);
  else disp_sendClr();
}

void disp_Buf() {
  if (disp_capture) queueDisplayCmd(disp_cmdBuf, 0, 0, 0, 0, 0, 0, 0);
  else disp_sendBuf();
}

void disp_sendStr(int x, int y, const char* str) {
  msg_disp.beginPacket("str");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addString(str);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendInt(int x, int y, int val) {
  msg_disp.beginPacket("int");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addInt(val);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendLng(int x, int y, long val) {
  msg_disp.beginPacket("lng");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addLong(val);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendFrm(int xp, int yp, int xs, int ys) {
  msg_disp.beginPacket("frm");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendBox(int xp, int yp, int xs, int ys) {
  msg_disp.beginPacket("box");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendRFrm(int xp, int yp, int xs, int ys, int r) {
  msg_disp.beginPacket("rfr");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendRBox(int xp, int yp, int xs, int ys, int r) {
  msg_disp.beginPacket("rbx");
  msg_disp.addByte(xp);
  msg_disp.addByte(yp);
  msg_disp.addByte(xs);
  msg_disp.addByte(ys);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendLine(int x0, int y0, int x1, int y1) {
  msg_disp.beginPacket("lin");
  msg_disp.addByte(x0);
  msg_disp.addByte(y0);
  msg_disp.addByte(x1);
  msg_disp.addByte(y1);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendCircle(int x, int y, int r) {
  msg_disp.beginPacket("cir");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendDisk(int x, int y, int r) {
  msg_disp.beginPacket("dis");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.addByte(r);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendPix(int x, int y) {
  msg_disp.beginPacket("pix");
  msg_disp.addByte(x);
  msg_disp.addByte(y);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendColor(int c) {
  msg_disp.beginPacket("col");
  msg_disp.addByte(c);
  msg_disp.streamPacket(&Serial7);
}

void disp_sendClr() {
  msg_disp.beginPacket("clr");
  msg_disp.streamPacket(&Serial7);
}

void disp_sendBuf() {
  msg_disp.beginPacket("buf");
  msg_disp.streamPacket(&Serial7);
}

void disp_debug(){
  disp_Color(1);
  disp_Line(5,5,100,20);
  disp_Circle(50, 40, 20);
  disp_Disk(50, 20, 20);
  disp_Box(100,10,10,20);
  disp_RBox(70,30,20,30, 7);
  disp_RFrm(70,30,20,30, 7);
  disp_Pix(60,10);
}
