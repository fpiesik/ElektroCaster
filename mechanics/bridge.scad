include <parameter.scad>;

//bridge(1);

//bockCut();
//scrw(3);
bock();

//translate([0,0,0])difference(){
//bridgeMount();
//}

topZ=4;
bottomZ=4;
z=topZ+bottomZ+slotXY;
sink=1.5;
bockSlotDia=6;//(x-slotXY)/2;
bockSlotSpc=1.2;
yA=36;
x=bodySlotX-slotXY*2;
strStrt=5;
scrwD=3.3;
scrwTD=2.8;
scrwHeadD=6;
scrwHeadZ=2;
StrMountT=16;
scrwOff=4;
mntY=yA;

module bridge(part){
translate([0,0,0])bridgeMount(part);

if(part==1||part==0)translate([0,yA/2,slotXY])strStuff(part);
}
 

module scrw(dia){
$fn=16;
headD=scrwHeadD;
headZ=scrwHeadZ;
cylinder(d=dia,h=z);
translate([0,0,z-headZ])cylinder(d1=dia,d2=headD,h=headZ);
}

module bockCut(){
$fn=16;
t=0.4;
rotate([0,0,0])
for(i=[0:nStrings-1]){
translate([strStrt,i*strDistB-strSpanB/2,0]){
hull(){
translate([0,strDistB/2-bockSlotDia/2-bockSlotSpc/2,0])rotate([0,0,0])cylinder(d=bockSlotDia+t,h=z);
translate([0,-(strDistB/2-bockSlotDia/2-bockSlotSpc/2),0])rotate([0,0,0])cylinder(d=bockSlotDia+t,h=z);   
}
translate([0,0,-z])cylinder(d=scrwTD,h=z);
}}
}

module strStuff(part){
zStrM=7;


strHDia=2.5;
strings=nStrings;
spacing=strDistB;


translate([-x/2,0,0])rotate([0,0,-90]){
for(i=[0:strings-1]){
translate([strStrt,i*spacing+(x-strSpanB)/2,-slotXY/2+scrwHeadZ]){
if(part==0)bock();
}}
}

translate([-x/2,0,0])rotate([0,0,-90]){
difference(){
difference(){
hull(){
$fn=32;
translate([0,slotXY/2,zStrM/2])rotate([0,90,0])cylinder(d=zStrM,h=yA);
translate([0,x-slotXY/2,zStrM/2])rotate([0,90,0])cylinder(d=zStrM,h=yA);
}
hull(){
translate([0,0,0]){
translate([-1,-x/2,topZ])cube([1,x*2,zStrM]);
translate([yA-3-StrMountT,-x/2,4])cube([3,x*2,yA]);
}
}
//translate([-1,-y/2,topZ])cube([bockSlotDia*2+2,y*2,z]);
}

for(i=[0:strings-1]){
translate([0,i*spacing+(x-strSpanB)/2,0]){
translate([yA-StrMountT+2,0,topZ])rotate([0,99,0])translate([0,0,-slotXY])cylinder(d=strHDia,h=yA,$fn=16);

}}

translate([-scrwOff,0,0]){
translate([yA/2,x/2,-z+topZ+0.001])scrw(scrwD);
translate([yA/2,x/2+strSpanB/2+strDistB/2,-z+topZ+0.001])scrw(scrwD); 
translate([yA/2,x/2-(strSpanB/2+strDistB/2),-z+topZ+0.001])scrw(scrwD);
}

translate([mntY/2-StrMountT/2,0,zStrM-topZ]){
translate([yA/2,x/2,-z+topZ+0.001])scrw(scrwD);
translate([yA/2,x/2+strSpanB/2+strDistB/2,-z+topZ+0.001])scrw(scrwD); 
translate([yA/2,x/2-(strSpanB/2+strDistB/2),-z+topZ+0.001])scrw(scrwD);
}

translate([0,x/2,-5])bockCut();

}
}
}

module bridgeMount(part){
z=topZ+bottomZ+slotXY;

difference(){
union(){
oneSide(1);
oneSide(-1);
}
translate([0,yA/2,slotXY/2])rotate([0,0,-90])bockCut();
}

phSp=0.4;

module oneSide(mirror){
scale([mirror,1,1])translate([0,0,0])difference(){
union(){
if(part==2||part==0)translate([-bodySlotX/4,0,z/2-bottomZ-topZ/2])cube([bodySlotX/2,yA,z-topZ],center=true);
hull(){
translate([-bodySlotX/2+slotXY,0,z/2-bottomZ])cube([slotXY,yA,z],center=true);
translate([-bodySlotX/2+slotXY/2+1,0,z/2-bottomZ])cube([slotXY+2,mntY,z],center=true);
}
}

translate([-bodySlotX/2+slotXY/2,0,(slotXY)/2])cube([slotXY,slotXY*4,slotXY],center=true);
//translate([-bodySlotX/2+slotXY/2,0,(slotXY)/2])cube([slotXY*6,slotXY,slotXY],center=true);


translate([-bodySlotX/2+slotXY/2,slotXY-scrwD*1.5,-bottomZ+0.001])scrw(scrwD);
translate([-bodySlotX/2+slotXY/2,-slotXY+scrwD*1.5,-bottomZ+0.001])scrw(scrwD);

translate([-bodySlotX/2+slotXY/2,slotXY-scrwD*1.5,-bottomZ+z-0.001])rotate([180,0,0])scrw(scrwD);
translate([-bodySlotX/2+slotXY/2,-slotXY+scrwD*1.5,-bottomZ+z-0.001])rotate([180,0,0])scrw(scrwD);

translate([0,scrwOff,0]){
translate([0,0,-bottomZ-0.001])scrw(scrwTD);
translate([-(strSpanB/2+strDistB/2),0,-bottomZ-0.001])scrw(scrwTD);
}
translate([0,-(mntY/2-StrMountT/2),0]){
translate([0,0,-bottomZ-0.001])scrw(scrwTD);
translate([-(strSpanB/2+strDistB/2),0,-bottomZ-0.001])scrw(scrwTD);
}

if(part==1)translate([0,0,(z-topZ)/2-bottomZ])cube([300,300,z-topZ],center=true);
if(part==2)translate([0,0,z/2+slotXY])cube([300,300,z],center=true);
}
}
}

module bock(){
$fn=16;
bockDia=bockSlotDia-0.4;
z=z/2+bockDia/2-scrwHeadZ-1;
hull(){
translate([0,strDistB/2-bockSlotDia/2-bockSlotSpc/2,0])rotate([0,0,0])cylinder(d=bockDia,h=z-bockSlotDia/2);
translate([0,-(strDistB/2-bockSlotDia/2-bockSlotSpc/2),0])rotate([0,0,0])cylinder(d=bockDia,h=z-bockSlotDia/2);
}
translate([0,strDistB/2-bockSlotDia/2-bockSlotSpc/2,z-bockSlotDia/2])rotate([0,0,0])sphere(d=bockDia);
translate([0,-(strDistB/2-bockSlotDia/2-bockSlotSpc/2),z-bockSlotDia/2])rotate([0,0,0])sphere(d=bockDia);
}

