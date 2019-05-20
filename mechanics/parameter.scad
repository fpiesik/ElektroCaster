nStrings=6;
nFrets=24;
nLedFrets=16;
m5Dia=5.3;
m5HeadZ=3.5;
m4Dia=4.3;
m4NutDia=8.3;
m4NutZ=3.5;
m4HeadZ=2.5;
m3Dia=3.3;
m3NutDia=6.6;
m3NutZ=2.5;
m3HeadZ=2;
strDistN=7;
strDistB=10.25;
scale=645;
neckSlotY=500;

slotXY=15;

bodySlotX=115;
bodySlotY=250;

jackYA=40;
jackYB=54;
jackZA=20;
jackZB=25;

bodyX=300;

bodyZ=40;

strDia=[1.7,1.4,1.1,0.7,0.5,0.35];

fretStrPlus=4;
fretLN=strDistN*(nStrings-1)+fretStrPlus*2;
fretLB=strDistB*(nStrings-1)+fretStrPlus*2;
strSpanB=strDistB*(nStrings-1);
strSpanN=strDistN*(nStrings-1);
fretBtmZ=1;
neckSlotX=fretLN;
neckSlotDist=neckSlotX-slotXY*2;

neck_sideT=5.5; //thicknes of the neck side locks
neck_bottomT=2.5;

neck_mntPosYA=447;
neck_mntPosYB=420;
neck_mntEndT=2.5;
neck_lastFretSpc=13;

nut_slotSink=6;

//fretY=16;
fretPcbZ=1.6;
fretDia=3.5;
fretZ=1.5;
fretOvrZ=4;
fretboardZ=fretOvrZ+fretPcbZ;



L=scale;
k=1.05946;

fretPos1=scale-(scale/pow(k,4));

function fretPos(i)=L-(L/pow(k,i));
function fretSpc(i)=(L/pow(k,i))-(L/pow(k,i+1));
function strSpc(i)=i/L*strDistB+(1-i/L)*strDistN;
function fretL(i)=(i/L*strDistB+(1-i/L)*strDistN)*(nStrings-1)+fretStrPlus*2;

echo (70-(fretL(fretPos(1))));

