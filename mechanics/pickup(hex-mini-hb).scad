include <parameter.scad>;
$fn=32;
rotate([0,0,0])hexPickup(2);
pY=10;
pX=69;
pBtmZ=6;
pZA=5;
mYO=20;
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

module hexPickup(part){
if(part==1||part==0)pickup();
if(part==-1)rotate([180,0,0])pickup();

if(part==2||part==0)mount();

if(part==3||part==0)translate([0,0,-coilZ])rotate([0,0,0])coilPress();

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

module pickup(){
difference(){
union(){
translate([-pX/2,-pY/2,0])cube([pX,pY,pZ]);
translate([-(bodySlotX/2-slotXY),-pY/2,0])cube([bodySlotX-slotXY*2,pY,pBtmZ]);
}

translate([-coilsX/2,-coilsY/2,0-2])cube([coilsX,coilsY,pZ]);


translate([pScrwD/2,0,0])cylinder(d=2.8,h=mZ*2);
translate([-pScrwD/2,0,0])cylinder(d=2.8,h=mZ*2);

translate([0,pY,m3Dia*1.5])rotate([90,0,0])cylinder(d=m3Dia,h=pY*2);

for(i=[0:nStrings-1]){
translate([i*coilDist-coilX/2-coilDist*2.5,0,0]){
translate([0,-coilY/2,0])cube([coilX,coilY,pZ+2]);

}}


}



}

module coilPress(){
xA=coilsX-7;
y=coilsY-1;
cableDia=4;
difference(){
union(){
hull(){
translate([-xA/2,-y/2,pZ-2])cube([xA,y,2]);
translate([-m3Dia*1.5,-y/2,10])cube([m3Dia*3,y,2]);
}
translate([-m3Dia*1.5,-y/2,0])cube([m3Dia*3,y,10]);
}
hull(){
translate([0,pY,11])rotate([90,0,0])cylinder(d=m3Dia,h=pY*2);
translate([0,pY,0])rotate([90,0,0])cylinder(d=m3Dia,h=pY*2);
}
hull(){
translate([-pX/2,y/2,15])rotate([90,0,90])cylinder(d=cableDia,h=pX);
translate([-pX/2,y/2-cableDia/2,15])rotate([90,0,90])cylinder(d=cableDia,h=pX);
}
}
}

}

module zAdj(){
z=3;
$fn=6;
difference(){
cylinder(d=20,h=z);
cylinder(d=8.1,h=z);

}
}
//zAdj();
