include <parameter.scad>
pickup();

rotate([0,0,0])coil();


module pickup(){
strings=6;
spacing=strSpanB;
echo (spacing);
diameter=10.5;
wall=2;
coilZ=12;
bottomZ=2;
t=0.3;
ySpacing=11;

bX = spacing+diameter+wall*4;
bY=ySpacing+diameter+wall*1;
bZ=coilZ+bottomZ+t;

tX = bX;
tY=bY;
hub=2.5;
magZ=1.5;
magDia=3.2;
tbottomZ=2;
tZ=hub+magZ+tbottomZ;
tDia=6.7;//6.5;
screwTdia=3.3;

screwMdia=4.3;
screwAdia=2.25;


module base(){
$fn=16;
difference(){
translate([0,0,0])rcube([bX,bY,bZ],3);

translate([-spacing/2,0,0]){
for(x=[0:strings-1]){
translate([x*(spacing/(strings-1)),ySpacing/2,0]){
translate([0,0,bottomZ])rotate([0,0,30])cylinder(d=diameter,h=100,$fn=6);
translate([0,0,-1])cylinder(d=magDia,h=100,$fn=16);
rotate([0,0,30])translate([0,-diameter/2+0.5,0])cylinder(d=screwAdia,h=100);
}}

for(x=[0:strings-1]){
translate([x*(spacing/(strings-1)),-ySpacing/2,0]){
translate([0,0,bottomZ])rotate([0,0,30])cylinder(d=diameter,h=100,$fn=6);
translate([0,0,-1])cylinder(d=magDia,h=100,$fn=16);
rotate([0,0,30])translate([0,diameter/2-0.5,0])cylinder(d=screwAdia,h=100);
}}

}
translate([-bX/2+screwMdia*0.8,0,0])cylinder(d=screwMdia-t,h=bZ);
translate([bX/2-screwMdia*0.8,0,0])cylinder(d=screwMdia-t,h=bZ);
}

}

//for(x=[0:strings-1]){
//translate([-spacing/2+x*(spacing/(strings-1)),-ySpacing/2,bottomZ]){
//rotate([0,0,0])coil();
//}}
base();
}

module coil(){
$fn=32;
magholedia=4;
wallthick=0.5;
loweridlerZ=1.8;
upperidlerZ=1.8;
rampBZ=0.1;
rampTZ=0.1;
coildia=10.25;
rampdia=coildia;
threaddepth=5;
roofZ=0;//loweridlerZ;
coilZ=12;
idlRes=6;

difference(){
union(){
cylinder(d=magholedia+wallthick*2,h=coilZ);
translate([0,0,loweridlerZ])cylinder(d1=rampdia,d2=magholedia+wallthick*2,h=rampBZ,$fn=idlRes);
translate([0,0,coilZ-upperidlerZ-rampTZ])cylinder(d2=rampdia/100*100,d1=magholedia+wallthick*2,h=rampTZ,$fn=idlRes);
translate([0,0,coilZ-upperidlerZ])cylinder(d=coildia/100*100,h=upperidlerZ,,$fn=idlRes);
//Ccube([coildia,coildia,upperidlerZ],[0,0,1]);
translate([0,0,0]){
//Ccube([coildia,coildia,loweridlerZ],[0,0,1]);
cylinder(d=coildia,h=loweridlerZ,$fn=idlRes);

}
}

translate([0,0,-roofZ])cylinder(d=magholedia,h=coilZ);
//cylinder(d=coilthreaddia,h=coilZ+1);
}}

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