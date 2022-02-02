include <parameter.scad>;
$fn=32;
pY=40;
pX=70;
wT=1.5;
mYO=pY+wT*2;

mX=bodySlotX;
mXI=bodySlotX-slotXY*2;
mZ=4;
encZ=30;
pmCutY=15;
pmCutZOff=12;

pScrwD=77.5;
pZ=slotXY+mZ;

lidT=0.3; //lid tolerance: the lid is that much smaller than the mount
lidZB=4; // how much contact with the mount
lidZA=1.5;


rotate([180,0,0])mount();
translate([0,50,0])lid();

module mount(){
difference(){
union(){
    translate([-mX/2,-mYO/2,slotXY])cube([mX,mYO,mZ]);
    translate([0,0,slotXY-encZ])rcube([mXI,mYO,encZ],3);  
}
translate([0,0,slotXY-encZ-1])rcube([mXI-wT*2,mYO-wT*2,encZ+1],2);
translate([0,0,slotXY-encZ-1-pmCutZOff])rcube([mXI*2,pmCutY,encZ+1],2);
translate([0,0,slotXY-1])rcube([pX,pY,mZ+2],3);


translate([bodySlotX/2-slotXY/2,0,slotXY])cylinder(d=m3Dia,h=mZ*2);
translate([bodySlotX/2-slotXY/2,0,slotXY+mZ-m3HeadZ])cylinder(d1=m3Dia,d2=m3Dia*2,h=m3HeadZ+0.1);

translate([-(bodySlotX/2-slotXY/2),0,slotXY])cylinder(d=m3Dia,h=mZ*2);
translate([-(bodySlotX/2-slotXY/2),0,slotXY+mZ-m3HeadZ])cylinder(d1=m3Dia,d2=m3Dia*2,h=m3HeadZ+0.1);

translate([pScrwD/2,0,slotXY])cylinder(d=m2Dia,h=mZ*2);
translate([pScrwD/2,0,slotXY+mZ-m3HeadZ])cylinder(d=m2Dia,h=m2HeadZ+0.1);

translate([-pScrwD/2,0,slotXY])cylinder(d=m2Dia,h=mZ*2);
translate([-pScrwD/2,0,slotXY+mZ-m3HeadZ])cylinder(d=m2Dia,h=m2HeadZ+0.1);
}
}

module lid(){
difference(){
union(){
    translate([0,0,slotXY])rcube([mXI,mYO,lidZA],3);  
    translate([0,0,slotXY])rcube([mXI-wT*2-lidT,mYO-wT*2-lidT,lidZA+lidZB],2);
}
//translate([0,0,slotXY-encZ-1])rcube([mXI-wT*2,mYO-wT*2,encZ+1],2);
//translate([0,0,slotXY-encZ-1-pmCutZOff])rcube([mXI*2,pmCutY,encZ+1],2);
//translate([0,0,slotXY-1])rcube([pX,pY,mZ+2],3);
}
}

module rcube(size,radius){
linear_extrude(height=size[2])
if(radius>0){
hull()
{
    // place 4 circles in the corners, with the given radius
    translate([(-size[0]/2)+(radius), (-size[1]/2)+(radius), 0])
    circle(r=radius);

    translate([(size[0]/2)-(radius), (-size[1]/2)+(radius), 0])
    circle(r=radius);

    translate([(-size[0]/2)+(radius), (size[1]/2)-(radius), 0])
    circle(r=radius);

    translate([(size[0]/2)-(radius), (size[1]/2)-(radius), 0])
    circle(r=radius);
}
}
if(radius==0){
translate([0,0,size[2]/2])cube(size,center=true);
}
}


