include <inkbody.scad>;
include <parameter.scad>;
include <neck.scad>;
include <slotBody.scad>;

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

neckCutX=strSpc(fretPos(22))*5+fretStrPlus*2+neck_sideT*2;

//projection(cut=false) 

projection(cut = false){
difference(){
//top();
//translate([-250,-350,-slotXY+0.1])cube([500,500,height]);
}
//cube([1,800,z],center=true);
}

module top(){
z=4;
cutAX=bodySlotX;
cutAY=bodySlotY-slotXY*5;
translate([0,0,slotXY]){
difference(){
import("bodyContFilled2.stl", convexity=15);
//translate([0,-cutAY/2-slotXY*3,-z])rcube([cutAX,cutAY,z*2],3);
//translate([0,0,-z])rcube([neckCutX,bodySlotY,z*2],3);
screwsTop();
screws();
}
}
}

//top();

//linear_extrude(height = 4, center = true, convexity = 10){
//projection(cut = false){
//body();
//} 
//}

//slotBody();
//bodySVG();


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

module bodySVG(){
   
 translate([-161,-313.5,0])linear_extrude(height = height, center = true, convexity = 10, twist = 0)
    import("bodyContour.svg");
    
}

module body(){
//body2Neck3();
//body2Neck();


difference(){

union(){
    translate([0,-115.3,-height/2])poly_path5353(height, width);
    translate([97.35,66,-height/2])rotate([0,0,42])scale([1,0.35,1])cylinder(d=25,h=height);
    translate([122.8,65.15,-height/2])rotate([0,0,-45])scale([1,0.3,1])cylinder(d=24,h=height);
//bodySVG();
    plateY=48;
    translate([0,plateY/2+6.5,-height/2])rcube([neckCutX,plateY,height-slotXY-neck_bottomT],4);

//translate([0,50,0])cube([neckCutX,100,slotXY],center=true);
//translate([-neckDockX/2,23,-height/2])cube([neckDockX,30,height]);
}
translate([0,50,height/2-slotXY-neck_bottomT+50])cube([neckCutX,100,100],center=true);

mntPosYA=neck_mntPosYA;
mntPosYB=neck_mntPosYB;
posYA=neckSlotY-nut_slotSink-slotXY*2-mntPosYA;
posYB=neckSlotY-nut_slotSink-slotXY*2-mntPosYB;
translate([fretL(mntPosYA)/2-slotXY/2,posYA,-height/2-0.1]){
    cylinder(d=m3Dia,h=height,$fn=12);
    cylinder(d1=7,d2=m3Dia,h=2.5,$fn=12);
}
translate([-(fretL(mntPosYA)/2-slotXY/2),posYA,-height/2-0.1]){
    cylinder(d=m3Dia,h=height,$fn=12);
    cylinder(d1=7,d2=m3Dia,h=2.5,$fn=12);
}

translate([fretL(mntPosYB)/2-slotXY/2,posYB,-height/2-0.1]){
    cylinder(d=m3Dia,h=height,$fn=12);
    cylinder(d1=7,d2=m3Dia,h=2.5,$fn=12);
}
translate([-(fretL(mntPosYB)/2-slotXY/2),posYB,-height/2-0.1]){
    cylinder(d=m3Dia,h=height,$fn=12);
    cylinder(d1=7,d2=m3Dia,h=2.5,$fn=12);
}



translate([0,0,-height/2])screws();

//translate([-neckSlotX/2,neckDockYposL,-height/2-1])cube([neckSlotX+15,100,height+2]);

//translate([-neckSlotX/2,0,0])cube([neckSlotX,100,slotXY+1]);
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

translate([0,-300,0])rotate([90,0,0])cylinder(d=4.5, h=15);
translate([-85,0,0])rotate([-90,0,0])cylinder(d=4.5, h=150);

//translate([-neckSlotX/2,slotXY/2+slotXY,slotXY/2])rotate([0,-90,0])cylinder(d=scrwDia, h=15);

translate([bodySlotX/2-slotXY/2,0,height/2-slotXY/2])rotate([-90,0,0])cylinder(d=m3Dia, h=15);
translate([bodySlotX/2-slotXY/2,8.8,height/2-slotXY/2])rotate([-90,0,0])cylinder(d1=m3Dia,d2=7, h=2.5);
translate([bodySlotX/2-slotXY/2,11,height/2-slotXY/2])rotate([-90,0,0])cylinder(d=7, h=10);

}

}

module screws(){
nScrws=9;
scrwDia=4.3;
$fn=12;
pos=[[-39,52.35],[-131.75,10],[-109.2,-65],[-136,-138],[-154.8,-217],[-114,-289],[40,-306.5]];
translate([0,0,0]){
for(i=[0:nScrws-1]){
translate([pos[i][0],pos[i][1],-slotXY])cylinder(d=scrwDia,h=height*2);
scale([-1,1,1])translate([pos[i][0],pos[i][1],-slotXY])cylinder(d=scrwDia,h=height*2);
}
translate([-100,75.7,-slotXY])cylinder(d=scrwDia,h=height*2);

translate([40,9.8,-slotXY])cylinder(d=scrwDia,h=height*2);
translate([82,29.5,-slotXY])cylinder(d=scrwDia,h=height*2);
translate([108,69.5,-slotXY])cylinder(d=scrwDia,h=height*2); 
}
}

module screwsTop(){
nScrws=4;
scrwDia=m3Dia;
$fn=12;
yDist=95;
pos=[[-(bodySlotX-slotXY)/2,-bodySlotY/2+yDist],[-(bodySlotX-slotXY)/2,-bodySlotY/2 -yDist]];
translate([0,0,0]){
for(i=[0:nScrws-1]){
translate([pos[i][0],pos[i][1]-slotXY/2,-slotXY])cylinder(d=scrwDia,h=height+5);
scale([-1,1,1])translate([pos[i][0],pos[i][1]-slotXY/2,-slotXY])cylinder(d=scrwDia,h=height+5);
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