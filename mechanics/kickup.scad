include <parameter.scad>
include <screws.scad>
//kickup();
mcMag();

strings=6;
spacing=strSpanB;//52;
echo (spacing);
diameter=15;
wall=2;
coilZ=15;
bottomZ=1.5;
t=0.3;
ySpacing=12;

bX = spacing+diameter+wall*3;
bY=ySpacing+diameter+wall*1;
bZ=coilZ+bottomZ+t;

tX = bX;
tY=bY;
hub=2.5;
magZ=1.5;
tbottomZ=2;
tZ=hub+magZ+tbottomZ;
tDia=7;//6.5;
screwTdia=3.3;


screwAdia=3;
topZ=2.5;


module base(bZ,screwMdia){
$fn=16;
difference(){
translate([0,0,0])rcube([bX,bY,bZ],3);

translate([-spacing/2,0,0]){
for(x=[0:strings-1]){
translate([x*(spacing/(strings-1)),(x%2-0.5)*ySpacing,0]){
translate([0,0,bottomZ])cylinder(d=diameter,h=100,$fn=6);
translate([0,(x%2-0.5)*-(diameter/100*95-screwAdia/2),0])cylinder(d=screwAdia,h=100);
}}

}
translate([-bX/2+screwMdia*1,bY/2-screwMdia*2.5,0])cylinder(d=screwMdia,h=bZ);
translate([bX/2-screwMdia*1,-bY/2+screwMdia*2.5,0])cylinder(d=screwMdia,h=bZ);
}
}

module top(){
$fn=16;
difference(){
translate([0,0,0])rcube([tX,tY,tZ],3);

translate([-spacing/2,0,0]){
for(x=[0:strings-1]){
translate([x*(spacing/(strings-1)),(x%2-0.5)*ySpacing,0]){
translate([0,0,-tbottomZ])cylinder(d=tDia,h=tZ);
cylinder(d=screwTdia,h=tZ);
}}

}
translate([-bX/2+screwMdia*1,bY/2-screwMdia*2.5,0])cylinder(d=screwMdia,h=tZ);
translate([bX/2-screwMdia*1,-bY/2+screwMdia*2.5,0])cylinder(d=screwMdia,h=tZ);
}
}
module topMacMag(screwMdia){
$fn=16;
difference(){
translate([0,0,0])rcube([bodySlotX,tY,2.5],3);

translate([-spacing/2,0,0]){
for(x=[0:strings-1]){
translate([x*(spacing/(strings-1)),(x%2-0.5)*ySpacing,0]){
translate([0,0,-tbottomZ])cylinder(d=tDia,h=tZ);
cylinder(d=screwTdia,h=tZ);
}}

}
translate([-bX/2+screwMdia*1,bY/2-screwMdia*2.5,topZ])screwA(m3Dia,10,m3Dia*2,2,180);
translate([bX/2-screwMdia*1,-bY/2+screwMdia*2.5,topZ])screwA(m3Dia,10,m3Dia*2,2,180);


translate([-bodySlotX/2+slotXY/2,0,topZ])screwA(m3Dia,10,m3Dia*2,2,180);
translate([-(-bodySlotX/2+slotXY/2),0,topZ])screwA(m3Dia,10,m3Dia*2,2,180);
}
}

module topKickUp(){
$fn=16;
difference(){
translate([0,0,0])rcube([bodySlotX,tY,2.5],3);

translate([-spacing/2,0,0]){
for(x=[0:strings-1]){
translate([x*(spacing/(strings-1)),(x%2-0.5)*ySpacing,0]){
cylinder(d=screwTdia+t,h=tZ);
}}

}
//translate([-bX/2+screwMdia*1,bY/2-screwMdia*2.5,0])cylinder(d=screwMdia-t,h=tZ);
//translate([bX/2-screwMdia*1,-bY/2+screwMdia*2.5,0])cylinder(d=screwMdia-t,h=tZ);
translate([-bX/2+screwMdia*1,bY/2-screwMdia*2.5,0])cylinder(d=screwMdia,h=bZ);
translate([bX/2-screwMdia*1,-bY/2+screwMdia*2.5,0])cylinder(d=screwMdia,h=bZ);

translate([-bodySlotX/2+slotXY/2,0,topZ])screwA(m3Dia,10,m3Dia*2,2,180);
translate([bodySlotX/2-slotXY/2,0,topZ])screwA(m3Dia,10,m3Dia*2,2,180);
}
}
module toplift(){
$fn=16;
lift=1.5;
difference(){
translate([0,0,0])rcube([slotXY,tY,lift],3);

hull(){
translate([0,0,0])cylinder(d=screwMdia+t,h=tZ);
translate([0,-tY,0])cylinder(d=screwMdia+t,h=tZ);
}
}
}

module blende(){
$fn=16;
x=95;
y=50;
z=1.5;
m=20;
scrwD=2.5;
sdX=85;
sdY=38;
ovlp=3;
offX=0;
offY=-4;
spcX=2.5;
spcY=4;
difference(){
union(){
difference(){
translate([offX,offY,0])rcube([x,y,z],2);
rcube([tX-ovlp*2,tY-ovlp*2,tZ],2);#
translate([offX,offY,0]){
hull(){
translate([sdX/2,sdY/2+spcY/2,0])cylinder(d=scrwD,h=z);
translate([sdX/2,sdY/2-spcY/2,0])cylinder(d=scrwD,h=z);}
hull(){
translate([-sdX/2,sdY/2+spcY/2,0])cylinder(d=scrwD,h=z);
translate([-sdX/2,sdY/2-spcY/2,0])cylinder(d=scrwD,h=z);}
hull(){
translate([-sdX/2,-sdY/2+spcY/2,0])cylinder(d=scrwD,h=z);
translate([-sdX/2,-sdY/2-spcY/2,0])cylinder(d=scrwD,h=z);}
hull(){
translate([sdX/2,-sdY/2+spcY/2,0])cylinder(d=scrwD,h=z);
translate([sdX/2,-sdY/2-spcY/2,0])cylinder(d=scrwD,h=z);}
}}
scale([-1,1,1]){
translate([-tX/2,-tY/2,0]){
difference(){
cube([m,m,z]);
translate([m,0,0])rotate([0,0,45])cube([100,100,z+1]);
}}
}

scale([1,-1,1]){
translate([-tX/2,-tY/2,0]){
difference(){
cube([m,m,z]);
translate([m,0,0])rotate([0,0,45])cube([100,100,z+1]);
translate([screwMdia*1.5,screwMdia*1.5,0])cylinder(d=screwMdia,h=tZ);
}}
}
}
hull(){
translate([-bX/2+screwMdia*1.5-spcX/2,bY/2-screwMdia*1.5,0])cylinder(d=screwMdia,h=tZ);
translate([-bX/2+screwMdia*1.5+spcX/2,bY/2-screwMdia*1.5,0])cylinder(d=screwMdia,h=tZ);}
hull(){
translate([bX/2-screwMdia*1.5-spcX/2,-bY/2+screwMdia*1.5,0])cylinder(d=screwMdia,h=tZ);
translate([bX/2-screwMdia*1.5+spcX/2,-bY/2+screwMdia*1.5,0])cylinder(d=screwMdia,h=tZ);}}

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




module coil(){
$fn=32;
magholedia=4;
wallthick=0.5;
loweridlerZ=1.8;
upperidlerZ=1.8;
rampBZ=0.2;
rampTZ=0.2;
coildia=14;
rampdia=coildia;
threaddepth=5;
roofZ=loweridlerZ;

difference(){
union(){
cylinder(d=magholedia+wallthick*2,h=coilZ);
translate([0,0,loweridlerZ])cylinder(d1=rampdia,d2=magholedia+wallthick*2,h=rampBZ,$fn=6);
translate([0,0,coilZ-upperidlerZ-rampTZ])cylinder(d2=rampdia/100*100,d1=magholedia+wallthick*2,h=rampTZ,$fn=6);
translate([0,0,coilZ-upperidlerZ])cylinder(d=coildia/100*100,h=upperidlerZ,,$fn=6);
//Ccube([coildia,coildia,upperidlerZ],[0,0,1]);
translate([0,0,0]){
//Ccube([coildia,coildia,loweridlerZ],[0,0,1]);
cylinder(d=coildia,h=loweridlerZ,$fn=6);

}
}

translate([0,0,-roofZ])cylinder(d=magholedia,h=coilZ);
//cylinder(d=coilthreaddia,h=coilZ+1);
}}

module kickup(){
base();
translate([0,0,bZ+tZ])rotate([0,0,0])top();
translate([0,-33,bZ])rotate([0,0,0])topKickUp();
//translate([0,0,bZ])rotate([0,0,0])toplift();
//translate([0,0,bZ+tZ+19])rotate([0,0,0])blende();
//translate([0,tY+28,tZ])rotate([180,0,0])top();
//translate([0,tY+0,0])rotate([0,90,0])coil();
}

module mcMag(){
coilZ=20;
bZ=coilZ+bottomZ+t;
screwMdia=3.3;   
base(bZ,screwMdia);
//translate([0,0,bZ+tZ])rotate([0,0,0])top();
translate([0,0,bZ+2])rotate([0,0,0])topMacMag(screwMdia);
//translate([0,-33,bZ])rotate([0,0,0])topKickUp();
//topKickUp();
//translate([0,0,bZ])rotate([0,0,0])toplift();
//translate([0,0,bZ+tZ+19])rotate([0,0,0])blende();
//translate([0,tY+28,tZ])rotate([180,0,0])top();
//translate([0,tY+0,0])rotate([0,90,0])coil();
}