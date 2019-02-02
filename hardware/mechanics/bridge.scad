include <parameter.scad>;

//bridge(0);

//bock();

//translate([0,0,0])difference(){
//bridgeMount();
//}

topZ=4;
sink=1.5;
bockSlotDia=6;//(x-slotXY)/2;
bockSlotSpc=1.2;

module bridge(part){
translate([0,0,0])bridgeMount(part);

if(part==1||part==0)translate([0,slotXY/4,slotXY])strStuff();
}
 

module strStuff(){
x= slotXY*1.5;
z= 7;
y=bodySlotX-slotXY*3;

strHDia=2.5;
strings=6;
spacing= 10;


StrMountT=7;

//bocks

translate([-y/2,0,0])rotate([0,0,-90]){
for(i=[0:strings-1]){
translate([(x-slotXY)/2,i*spacing+(y-strSpanB)/2,0]){
bock();
}}
}

translate([-y/2,0,0])rotate([0,0,-90]){
difference(){
difference(){
hull(){
$fn=32;
translate([0,z/2,z/2])rotate([0,90,0])cylinder(d=z,h=x);
translate([0,y-z/2,z/2])rotate([0,90,0])cylinder(d=z,h=x);
translate([0,0,0])cube([x,y,z/2]);
}
hull(){
translate([0,0,0]){
translate([-1,-y/2,topZ])cube([1,y*2,z]);
translate([x-3-StrMountT,-y/2,4])cube([3,y*2,x]);
}
}
//translate([-1,-y/2,topZ])cube([bockSlotDia*2+2,y*2,z]);
}

//translate([x-5,0,0])cube([5,y,z]);}

for(i=[0:strings-1]){
translate([(x-slotXY)/2,i*spacing+(y-strSpanB)/2,0]){
hull(){
translate([0,strDistB/2-bockSlotDia/2-bockSlotSpc/2,1])rotate([0,0,0])cylinder(d=bockSlotDia,h=x/2,$fn=16);
translate([0,-(strDistB/2-bockSlotDia/2-bockSlotSpc/2),1])rotate([0,0,0])cylinder(d=bockSlotDia,h=x/2,$fn=16);
}
}}

for(i=[0:strings-1]){
translate([0,i*spacing+(y-strSpanB)/2,0]){
translate([x-StrMountT,0,topZ])rotate([0,105,0])translate([0,0,-5])cylinder(d=strHDia,h=x/2,$fn=16);

}}

for(i=[0:4]){
if(i==0||i==2||i==4){
translate([0,i*spacing+(y-strSpanB)/2+strDistB/2,0]){
translate([x/2,0,0])cylinder(d=5.3,h=z,$fn=16);
translate([x/2,0,topZ-sink])cylinder(d=9.3,h=z,$fn=16); }}}
}
}
}

module bridgeMount(part){
bottomZ=4;

y=30;
z=topZ+bottomZ+slotXY;
oneSide(1);
oneSide(-1);

phSp=0.4;

module oneSide(mirror){
scale([mirror,1,1])translate([0,-slotXY/2,0])difference(){
union(){
if(part==2||part==0)translate([-bodySlotX/4,0,z/2-bottomZ-topZ/2])cube([bodySlotX/2,slotXY*1.5,z-topZ],center=true);
hull(){
translate([-bodySlotX/2+slotXY,0,z/2-bottomZ])cube([slotXY,slotXY*1.5,z],center=true);
translate([-bodySlotX/2+slotXY/2+1,0,z/2-bottomZ])cube([slotXY+2,slotXY*3,z],center=true);
}
}

translate([-bodySlotX/2+slotXY/2,0,(slotXY)/2])cube([slotXY,slotXY*4,slotXY],center=true);
translate([-bodySlotX/2+slotXY/2,0,(slotXY)/2])cube([slotXY*6,slotXY,slotXY],center=true);

translate([0,0,-bottomZ+sink+phSp])cylinder(d=m5Dia,h=slotXY*2);
translate([0,0,-bottomZ])cylinder(d=m5Dia*2,h=sink);
translate([-strDistB*2,0,-bottomZ+sink+phSp])cylinder(d=m5Dia,h=slotXY*2);
translate([-strDistB*2,0,-bottomZ])cylinder(d=m5Dia*2,h=sink);

translate([-bodySlotX/2+slotXY+m3Dia*1.5,slotXY/2+m3Dia*1.5,-bottomZ+m3NutZ+phSp])cylinder(d=m3Dia,h=slotXY*2,$fn=12);
translate([-bodySlotX/2+slotXY+m3Dia*1.5,-slotXY/2-m3Dia*1.5,-bottomZ+m3NutZ+phSp])cylinder(d=m3Dia,h=slotXY*2,$fn=12);

translate([-bodySlotX/2+slotXY+m3Dia*1.5,slotXY/2+m3Dia*1.5,-bottomZ])cylinder(d=m3NutDia,h=m3NutZ,$fn=6);
translate([-bodySlotX/2+slotXY+m3Dia*1.5,-slotXY/2-m3Dia*1.5,-bottomZ])cylinder(d=m3NutDia,h=m3NutZ,$fn=6);

translate([-bodySlotX/2+slotXY+m3Dia*1.5,slotXY/2+m3Dia*1.5,-bottomZ+z-m3HeadZ])cylinder(d1=m3Dia,d2=m3NutDia,h=m3NutZ,$fn=12);
translate([-bodySlotX/2+slotXY+m3Dia*1.5,-slotXY/2-m3Dia*1.5,-bottomZ+z-m3HeadZ])cylinder(d1=m3Dia,d2=m3NutDia,h=m3NutZ,$fn=12);

translate([-bodySlotX/2+slotXY/2,slotXY,-bottomZ+sink+phSp])cylinder(d=m5Dia,h=slotXY*2);
translate([-bodySlotX/2+slotXY/2,-slotXY,-bottomZ+sink+phSp])cylinder(d=m5Dia,h=slotXY*2);

translate([-bodySlotX/2+slotXY/2,slotXY,-bottomZ])cylinder(d=m5Dia*2,h=sink);
translate([-bodySlotX/2+slotXY/2,-slotXY,-bottomZ])cylinder(d=m5Dia*2,h=sink);

translate([-bodySlotX/2+slotXY/2,slotXY,-bottomZ+z-sink])cylinder(d=m5Dia*2,h=sink);
translate([-bodySlotX/2+slotXY/2,-slotXY,-bottomZ+z-sink])cylinder(d=m5Dia*2,h=sink);

if(part==1)translate([0,0,(z-topZ)/2-bottomZ])cube([300,300,z-topZ],center=true);
if(part==2)translate([0,0,z/2+slotXY])cube([300,300,z],center=true);
}
}
}

module bock(){
$fn=32;
z=8.5-1;
bockDia=bockSlotDia-0.4;
hull(){
translate([0,strDistB/2-bockSlotDia/2-bockSlotSpc/2,0])rotate([0,0,0])cylinder(d=bockDia,h=z-bockSlotDia/2);
translate([0,-(strDistB/2-bockSlotDia/2-bockSlotSpc/2),0])rotate([0,0,0])cylinder(d=bockDia,h=z-bockSlotDia/2);
}
translate([0,strDistB/2-bockSlotDia/2-bockSlotSpc/2,z-bockSlotDia/2])rotate([0,0,0])sphere(d=bockDia);
translate([0,-(strDistB/2-bockSlotDia/2-bockSlotSpc/2),z-bockSlotDia/2])rotate([0,0,0])sphere(d=bockDia);
}

