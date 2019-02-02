include <inkbody.scad>;
include <parameter.scad>;
include <neck.scad>;

profile_scale = 25.4/100; //made in inkscape in mm
height = 40;
width = 12/profile_scale;

scrwDia=4.3;
scrw2Dia=5.3;

fillX=[255,230,225,250,290,300,300,264,100,100,100,100];
fillY=273;
fillThick=10;

neckDockX=56.4;
neckDockYposL=31.95;//38.6;
neckDockYposU=53.6;//38.6;

//projection(cut=false) 

projection(cut = false){
difference(){
top();
translate([-250,-350,-slotXY+0.1])cube([500,500,height]);
}
}
module top(){
z=4;
cutAX=bodySlotX;
cutAY=bodySlotY-slotXY*2;
translate([0,0,slotXY]){
difference(){
import("bodyContFilled.stl", convexity=15);
translate([0,-cutAY/2-slotXY,-z])rcube([cutAX,cutAY,z*2],3);
translate([0,0,-z])rcube([neckDockX,bodySlotY,z*2],3);
screwsTop();
}
}
}

//top();
//body();

//rotate([90,0,0])body2Neck();
//rotate([90,0,0]){
//neckBack();
//body2Neck();
//}


//for(i=[0:7]){
//translate([-fillX[i]/2,0-fillThick*i*4,-height/2])cube([fillX[i],fillThick,height]);
//}
//
//for(i=[0:5]){
//translate([95-fillThick*i*4,-fillY,-height/2])cube([fillThick,fillY,height]);
//}



//body2Neck3();

module body(){
body2Neck3();
body2Neck();

difference(){
union(){
translate([0,-115.3,-height/2])poly_path5353(height, width);
translate([97.35,66,-height/2])rotate([0,0,42])scale([1,0.35,1])cylinder(d=25,h=height);
translate([122.8,65.15,-height/2])rotate([0,0,-45])scale([1,0.3,1])cylinder(d=24,h=height);
//translate([-slotXY/2-neckSlotX/2,0,0])cube([slotXY/2,slotXY*2+6,slotXY]);
//translate([-neckDockX/2,23,-height/2])cube([neckDockX,30,height]);
}
screws();

translate([-neckSlotX/2,neckDockYposL,-height/2-1])cube([neckSlotX+15,100,height+2]);

translate([-neckSlotX/2,0,0])cube([neckSlotX,100,slotXY+1]);
//translate([-neckSlotX/2,0,-slotXY])cube([neckSlotX,100,slotXY+1]);



translate([92,-260,0])rotate([0,0,-30])translate([0,2.1,0]){
translate([15,(jackYB-jackYA)/2,-jackZA/2])cube([neckSlotX,jackYA,jackZA]);
//translate([-9,-2.5,0])cube([neckSlotX,55,slotXY+1]);
translate([41.5,jackYB/2,0])rotate([0,90,0])rcube([jackZB,jackYB,neckSlotX],3);
translate([44,-5,-height/2-1])cube([neckSlotX*2,100,height+2]);

translate([45,(jackYB-jackYA)/4,jackZB/4])rotate([0,-90,0])cylinder(d=m3Dia,h=20,$fn=12);
translate([45,jackYB-(jackYB-jackYA)/4,jackZB/4])rotate([0,-90,0])cylinder(d=m3Dia,h=20,$fn=12);
translate([45,(jackYB-jackYA)/4,-jackZB/4])rotate([0,-90,0])cylinder(d=m3Dia,h=20,$fn=12);
translate([45,jackYB-(jackYB-jackYA)/4,-jackZB/4])rotate([0,-90,0])cylinder(d=m3Dia,h=20,$fn=12);
}
//translate([130,-265,-slotXY/2])rotate([0,0,-20])cube([neckSlotX,50,slotXY+1]);

translate([0,-300,0])rotate([90,0,0])cylinder(d=m5Dia, h=15);
translate([-85,0,0])rotate([-90,0,0])cylinder(d=m5Dia, h=150);

translate([-neckSlotX/2,slotXY/2+slotXY,slotXY/2])rotate([0,-90,0])cylinder(d=scrwDia, h=15);

translate([bodySlotX/2-slotXY/2,0,slotXY/2])rotate([-90,0,0])cylinder(d=scrw2Dia, h=15);
translate([bodySlotX/2-slotXY/2,6,slotXY/2])rotate([-90,0,0])cylinder(d=scrw2Dia*2, h=15);

translate([bodySlotX/3-slotXY/2,-bodySlotY,slotXY/2])rotate([90,0,0])cylinder(d=scrwDia, h=100);
translate([bodySlotX/3-slotXY/2,-bodySlotY-20,slotXY/2])rotate([90,0,0])cylinder(d=scrwDia*2, h=100);

translate([-bodySlotX/3+slotXY/2,-bodySlotY,slotXY/2])rotate([90,0,0])cylinder(d=scrwDia, h=100);
translate([-bodySlotX/3+slotXY/2,-bodySlotY-20,slotXY/2])rotate([90,0,0])cylinder(d=scrwDia*2, h=100);
}

}

module screws(){
nScrws=9;
scrwDia=m4Dia;
$fn=12;
pos=[[-35,50.35],[-131.75,10],[-109.2,-65],[-136,-138],[-154.8,-217],[-114,-289],[40,-306.5]];
translate([0,0,0]){
for(i=[0:nScrws-1]){
translate([pos[i][0],pos[i][1],-slotXY])cylinder(d=scrwDia,h=height+5);
scale([-1,1,1])translate([pos[i][0],pos[i][1],-slotXY])cylinder(d=scrwDia,h=height+5);
}
translate([-100,75.7,-slotXY])cylinder(d=scrwDia,h=height+5);

translate([40,9.8,-slotXY])cylinder(d=scrwDia,h=height+5);
translate([82,29.5,-slotXY])cylinder(d=scrwDia,h=height+5);
translate([108,69.5,-slotXY])cylinder(d=scrwDia,h=height+5); 
}
}

module screwsTop(){
nScrws=9;
scrwDia=m4Dia;
$fn=12;
yDist=95;
pos=[[-(bodySlotX+slotXY)/2,-bodySlotY/2],[-(bodySlotX+slotXY)/2,-bodySlotY/2+yDist],[-(bodySlotX+slotXY)/2,-bodySlotY/2 -yDist],[neckDockX/2+slotXY/2,-slotXY/2],[bodySlotX/4,-bodySlotY+slotXY/2]];
translate([0,0,0]){
for(i=[0:nScrws-1]){
translate([pos[i][0],pos[i][1],-slotXY])cylinder(d=scrwDia,h=height+5);
scale([-1,1,1])translate([pos[i][0],pos[i][1],-slotXY])cylinder(d=scrwDia,h=height+5);
}
}
}

module body2Neck(){
difference(){
hull(){
translate([0,0,0])body2NeckShape();
translate([0,-(scale-neckSlotY)+3,0])rotate([-90,0,0])neckShape(12,0);
}
translate([-neckSlotX/2,29,0])cube([neckSlotX,slotXY*4,slotXY+1]);
}
}

module body2Neck3(){
scrwDia=m5Dia;
difference(){
hull(){
translate([-neckDockX/2,neckDockYposL,-slotXY])cube([1,0.1,slotXY*2]);
translate([neckDockX/2-10,neckDockYposL,-slotXY])cube([10,0.11,slotXY*2]);
translate([0,0,0])body2NeckShape();
}
translate([-neckSlotX/2,neckDockYposL-1,0])cube([neckSlotX,slotXY*4,slotXY+1]);
//translate([-neckDockX/2-(neckDockX-neckSlotX)/2,35,-slotXY])scale([1,-1,1])cube([neckDockX,slotXY*4,slotXY*2+1]);

translate([neckSlotX/2-slotXY/2,neckDockYposL+scrwDia*1.2,-slotXY])rotate([0,0,0])cylinder(d=scrwDia, h=height);
translate([neckSlotX/2-slotXY/2,neckDockYposL+scrwDia*1.2,-slotXY])rotate([0,0,0])cylinder(d=scrwDia*1.8, h=scrwDia);

translate([-(neckSlotX/2-slotXY/2),neckDockYposL+scrwDia*1.2,-slotXY])rotate([0,0,0])cylinder(d=scrwDia, h=height);
translate([-(neckSlotX/2-slotXY/2),neckDockYposL+scrwDia*1.2,-slotXY])rotate([0,0,0])cylinder(d=scrwDia*1.8, h=scrwDia);
}
translate([neckSlotX/2-slotXY/2,neckDockYposL+scrwDia*1.2,-slotXY])rotate([0,0,0])cylinder(d=scrwDia*1.6, h=scrwDia-0.25);
translate([-(neckSlotX/2-slotXY/2),neckDockYposL+scrwDia*1.2,-slotXY])rotate([0,0,0])cylinder(d=scrwDia*1.6, h=scrwDia-0.25);
}

module body2NeckShape(){
hull(){
translate([-neckDockX/2,neckDockYposU,-slotXY])rotate([0,0,0])cube([slotXY,1,slotXY*2]);
translate([neckDockX/2-slotXY,neckDockYposU,0])rotate([0,0,0])cube([slotXY,1,slotXY]);
//translate([neckDockX/2-slotXY,52,-7.5])rotate([0,0,0])cube([slotXY,1,slotXY]);
//translate([-(neckSlotX)/2-6.2,52,-slotXY])rotate([0,0,0])cube([neckSlotX/2,1,slotXY*2]);
translate([0,0,neckDockX/2-slotXY])difference(){
translate([0,neckDockYposU,0])rotate([-90,0,0])scale([1,1,1])cylinder(d=neckDockX,h=1,$fn=64);
translate([-neckDockX/2,neckDockYposU-0.5,0])rotate([0,0,0])cube([neckDockX,2,neckDockX/2]);
}
}
}

module rcube(size,radius){
$fn=32;
linear_extrude(height=size[2])
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