include <parameter.scad>;
$fn=32;
pY=20;
pX=69;
pBtmZ=6;
pZA=5;
mYO=pY+10;
mX=bodySlotX;
mXI=bodySlotX-slotXY*2;
mZ=4;
pScrwD=mXI-m3Dia-2.4;
pZ=slotXY+mZ;
pMDist=0.5;
coilZ=4.5;
coilX=7;
coilsX=67;
coilsY=7;
coilY=6.5;
coilDist=10.4;
coilSpan=nStrings*(coilDist-1);

mount();

module mount(){
difference(){
translate([-mX/2,-mYO/2,slotXY])cube([mX,mYO,mZ]);
translate([-pX/2-pMDist,-pY/2-pMDist,slotXY])cube([pX+pMDist*2,pY+pMDist*2,mZ+1]);

translate([bodySlotX/2-slotXY/2,0,slotXY])cylinder(d=m3Dia,h=mZ*2);
translate([bodySlotX/2-slotXY/2,0,slotXY+mZ-m3HeadZ])cylinder(d1=m3Dia,d2=m3Dia*2,h=m3HeadZ+0.1);

translate([-(bodySlotX/2-slotXY/2),0,slotXY])cylinder(d=m3Dia,h=mZ*2);
translate([-(bodySlotX/2-slotXY/2),0,slotXY+mZ-m3HeadZ])cylinder(d1=m3Dia,d2=m3Dia*2,h=m3HeadZ+0.1);

translate([pScrwD/2,0,slotXY])cylinder(d=m3Dia,h=mZ*2);
translate([pScrwD/2,0,slotXY+mZ-m3HeadZ])cylinder(d1=m3Dia,d2=m3Dia*2,h=m3HeadZ+0.1);

translate([-pScrwD/2,0,slotXY])cylinder(d=m3Dia,h=mZ*2);
translate([-pScrwD/2,0,slotXY+mZ-m3HeadZ])cylinder(d1=m3Dia,d2=m3Dia*2,h=m3HeadZ+0.1);
}
}




