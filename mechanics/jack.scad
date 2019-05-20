include <parameter.scad>




module mount(){
y=20;
z=jackZB/2+m3Dia*2;
x=m3Dia*2;
t=0.4;
$fn=16;

difference(){
cube([x,y,z]);
translate([x/2,m3Dia,0])cylinder(d=m3Dia,h=z);
translate([x/2,m3Dia*2,m3Dia/2])rotate([-90,0,0])cylinder(d=m3Dia-t,h=z);
translate([x/2,m3Dia*2,m3Dia/2+jackZB/2])rotate([-90,0,0])cylinder(d=m3Dia-t,h=z);
}
}

module plate(){
height=5;
x=2.5;
rj45Y=16;
rj45Z=14;
rj45YPos=11.2;

jDia=11.5;
jrDist=16.6;
jYPos=12.4;
jZPos=8.1;
pcbY=jackYA;
pcbZ=1.4;

$fn=32;
t=0.3;

difference(){
//translate([15,(jackYB-jackYA)/2,-jackZA/2])cube([neckSlotX,jackYA,jackZA]);
//translate([-9,-2.5,0])cube([neckSlotX,55,slotXY+1]);
translate([0,0,0])rotate([0,90,0])rcube([jackZB-t,jackYB-t,x],3);
translate([0,pcbY/2-rj45YPos,-jackZA/2+pcbZ])rcube([x*3,rj45Y,rj45Z],1);
translate([-1,-pcbY/2+jYPos,-jackZA/2+jZPos])rotate([0,90,0])cylinder(d=jDia,h=20,$fn=20);

translate([-1,-jackYB/2+(jackYB-jackYA)/4,jackZB/4])rotate([0,90,0])cylinder(d=m3Dia,h=20,$fn=12);
translate([0,-jackYB/2+(jackYB-jackYA)/4,jackZB/4])rotate([0,90,0])cylinder(d2=m3Dia,d1=m3Dia*1.7,h=m3HeadZ,$fn=12);

translate([-1,jackYB/2-(jackYB-jackYA)/4,jackZB/4])rotate([0,90,0])cylinder(d=m3Dia,h=20,$fn=12);
translate([0,jackYB/2-(jackYB-jackYA)/4,jackZB/4])rotate([0,90,0])cylinder(d2=m3Dia,d1=m3Dia*1.7,h=m3HeadZ,$fn=12);

translate([-1,-jackYB/2+(jackYB-jackYA)/4,-jackZB/4])rotate([0,90,0])cylinder(d=m3Dia,h=20,$fn=12);
translate([0,-jackYB/2+(jackYB-jackYA)/4,-jackZB/4])rotate([0,90,0])cylinder(d2=m3Dia,d1=m3Dia*1.7,h=m3HeadZ,$fn=12);

translate([-1,jackYB/2-(jackYB-jackYA)/4,-jackZB/4])rotate([0,90,0])cylinder(d=m3Dia,h=20,$fn=12);
translate([0,jackYB/2-(jackYB-jackYA)/4,-jackZB/4])rotate([0,90,0])cylinder(d2=m3Dia,d1=m3Dia*1.7,h=m3HeadZ,$fn=12);
}
}

rotate([0,90,0])plate();


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