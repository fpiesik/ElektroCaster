include <../parameter.scad>;
include <../neck/neck.scad>;

zA=17;

xB=45;
xN=strSpanN+9;
y=90;
edgeDia=10;

tunShftDia=5.5;
xA=strSpanN+tunShftDia+fretStrPlus+6;
tunerX=20;


tunerDia=10.4; //(8.6)


tunerYDistA=30;
tunerYDistB=30;

tunerYOff=16.1;

angle=10;

scrwDia=3.3;
scrwDist=fretLN-slotXY;
slotSink=8;

strsink=1;


mountT=10; //mount thickness

strHDia=[2,1.8,1.6,1.4,1.2,1];
strSp=1.5;
strEtoE=(nStrings-1)*strDistN;
fret2Str=0.6; //distance between Strings and frets on the nut position

nutZA=fretboardZ+fretZ+strHDia[5];
nutZB=6;
nutY=12;
nutZ=nutZA+nutZB;

strZPos=fretboardZ+fret2Str+fretZ+strsink;

lockZ=zA/2.2;
lockY=12;
lockX=xA;
lockScr=3;
lockNut=6.3;
lockNutZ=2.5;
lockHeadD=5.5;
lockHeadZ=2;
lockScrSpc=scrwDist/2+scrwDia+1;

xAngle=12;

printSp=0.3;

//head();
//scrw(lockScr,10);
//strSpacer();
rotate([90,0,0])nut();
//translate([0,nutY,0])rotate([xAngle,0,0])tuners();

module scrw(dia, h){
$fn=16;
headD=lockHeadD;
headZ=lockHeadZ;
translate([0,0,-h-headZ+0.001])cylinder(d=dia,h=h);
translate([0,0,-headZ-0.001])cylinder(d1=dia,d2=headD,h=headZ+0.001);
translate([0,0,-0.002])cylinder(d=headD,h=20);
}

module head(){
translate([0,nutY,-slotXY])tuners();
translate([0,0,-slotXY])rotate([0,0,0])nut();
}

module tuners(){

tYDist=23;
zAngle=19.7;

y=tYDist*5+17;
tunerYOff=9;
t=0.3;


translate([0,0,(slotXY-zA)/2])rotate([-xAngle,0,0])translate([0,tunerX/2,0])difference(){
union(){
hull(){
translate([-(xA+tunerX)/2,-tunerX/2,0])cube([xA+tunerX-6,0.1,zA]);
translate([-(xA+tunerX)/2,-tunerX/2,0])rotate([xAngle,0,0])cube([xA+tunerX-6,0.1,zA]);
}

hull(){
translate([-(xA+tunerX)/2,-tunerX/2,0])cube([xA+tunerX-6,1,zA]);
translate([xA/2,0,0]){

//cylinder(d=tunerX,h=zA);
//rotate([0,0,0])translate([-4,y-8,0])cylinder(d=tunerX,h=zA);
}
translate([-xA/2,0,0]){
cylinder(d=tunerX,h=zA);
rotate([0,0,-zAngle])translate([0,y,0])cylinder(d=tunerX,h=zA);
}
}
}
for(i=[0:5]){
translate([-xA/2,0,0])rotate([0,0,-zAngle])tuner(0,i*tYDist+tunerYOff,0);}

zOff=-2;
//translate([0,-tunerX/2,0])rotate([xAngle,0,0])translate([scrwDist/2,0,zA/2])rotate([-90,0,0])cylinder(d=scrwDia,h=y,$fn=16);
//translate([0,-tunerX/2,0])rotate([xAngle,0,0])translate([-scrwDist/2,0,zA/2])rotate([-90,0,0])cylinder(d=scrwDia,h=y,$fn=16);

//translate([0,-tunerX/2,0])rotate([xAngle,0,0])translate([-(scrwDist+scrwDia)/2,mountT,-zA])rotate([0,0,0])cube([scrwDist+scrwDia,10,zA*2]);
translate([-lockX/2-t,-tunerX/2,0])rotate([xAngle,0,0])translate([0,-1,(zA-lockZ)/2])cube([lockX+t*2,lockY+1.5,lockZ]);


for(i=[0:2]){
space=lockScrSpc;
translate([i*space-space,-tunerX/2,0])rotate([xAngle,0,0])translate([0,lockY/2,m4NutZ+zOff+printSp])cylinder(d=lockScr,h=zA-m4HeadZ-4,$fn=16);
//translate([i*space-space,-tunerX/2,0])rotate([xAngle,0,0])translate([0,lockY/2,zA+zOff-2.2])cylinder(d=lockScr,h=zA-m4HeadZ-4,$fn=16);
translate([i*space-space,-tunerX/2,0])rotate([xAngle,0,0])translate([0,lockY/2,zA-1])rotate([0,0,0])scrw(lockScr,(zA-lockZ)/2-lockHeadZ-1-printSp);
translate([i*space-space,-tunerX/2,0])rotate([xAngle,0,0])translate([0,lockY/2,zOff])cylinder(d=lockNut,h=lockNutZ,$fn=6);
}


}

module tuner(posX,posY,rot){
$fn=32;
slotDia=3.3;
slotToC=8.5;
slotDist=10;
slotZ=5;
translate([posX,posY,0]){
translate([0,0,-1])cylinder(d=tunerDia,h=zA+2);
translate([slotDist/2,slotToC,-1])cylinder(d=slotDia,h=slotZ+1);
translate([-slotDist/2,slotToC,-1])cylinder(d=slotDia,h=slotZ+1);
}
}

}



module nut(){
spacing=strDistN;
t=0.75;
translate([0,nutY,0]){
difference(){
union(){
hull(){
translate([0,-nutY,0])rotate([180,0,0])neckShp();
translate([-(xA+tunerX)/2,0-2,(slotXY-zA)/2])cube([xA+tunerX-6,2,zA]);
translate([-xN/2,-nutY,slotXY+strZPos+strHDia[0]/2])cube([xN,strSp,0.001]);
}
lockX=lockX-t;
lockZ=lockZ-0.25;
translate([-lockX/2,0,(slotXY-zA)/2+(zA-lockZ)/2])cube([lockX,lockY-t,lockZ]);
}

translate([-(scrwDist+slotXY)/2,-nutY-1,0])cube([scrwDist+slotXY,nut_slotSink+1,slotXY]);

for(i=[0:nStrings-1]){
translate([i*spacing,0,0]){
hull(){
translate([-(spacing*(nStrings-1))/2,0,slotXY+strZPos+strHDia[i]/2-strsink])rotate([90,0,0])cylinder(d=strHDia[i],h=20,$fn=16);
translate([-(spacing*(nStrings-1))/2,0,slotXY+strZPos+strHDia[i]*2])rotate([90,0,0])cylinder(d=strHDia[i]+1.5,h=20,$fn=16);
}
}}

translate([scrwDist/2,lockY-1-t,slotXY/2])rotate([-90,0,0])scrw(lockScr,lockY+nutY/2-3-printSp-t);
translate([-scrwDist/2,lockY-1-t,slotXY/2])rotate([-90,0,0])scrw(lockScr,lockY+nutY/2-3-printSp-t);


for(i=[0:2]){
space=lockScrSpc;
translate([i*space-space,lockY/2,0])rotate([0,0,0])cylinder(d=lockScr,h=y/3,$fn=16);
translate([i*space-space,lockY/2,0])rotate([0,0,0])cylinder(d=lockScr,h=y/3,$fn=16);
translate([i*space-space,lockY/2,0])rotate([0,0,0])cylinder(d=lockScr,h=y/3,$fn=16);
}
}
}
}

module strSpacer(){
spacing=strDistN;
rotate([90,0,0]){
difference(){
translate([-(strEtoE+8)/2,0,0])cube([strEtoE+8,3,10]);
for(i=[0:strings-1]){
translate([i*spacing,0,0]){
hull(){
translate([-(spacing*(nStrings-1))/2,0,0])rotate([-90,0,0])cylinder(d=1.5,h=20,$fn=16);
translate([-(spacing*(nStrings-1))/2,0,5])rotate([-90,0,0])cylinder(d=1.5,h=20,$fn=16);}}}
}
}
}