nStrings=6;
nFrets=22;
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
strDistN=8;
strDistB=10;
scale=700;
neckSlotY=500;
neckSlotX=44;
slotXY=20;
neckSlotDist=neckSlotX-slotXY*2;
bodySlotX=124;
bodySlotY=290;

jackYA=40;
jackYB=54;
jackZA=20;
jackZB=25;

bodyX=300;

strDia=[1.7,1.4,1.1,0.7,0.5,0.35];

fretLN=strDistN*(nStrings-1)+5;
fretLB=strDistB*(nStrings-1)+5;
strSpanB=strDistB*(nStrings-1);
strSpanN=strDistN*(nStrings-1);
fretBtmZ=1;

fretY=16;
fretDia=3.5;
fretZ=1.5;
fretboardZ=3.5;

L=scale;
k=1.05946;

fretPos1=scale-(scale/pow(k,4));


//echo (fretPos1);

