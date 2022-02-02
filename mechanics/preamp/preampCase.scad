include <parameter.scad>
t=0.3;
btmZ=1;
pcbBtm=2;
pcbX=86;
pcbY=32;
pcbZ=14;
pcbT=2;
z=pcbZ+btmZ*2+pcbBtm+pcbT;
baseZ=pcbZ+btmZ+pcbBtm+pcbT;
x=bodySlotX;
echo(z);
jackDia=7;

cCutY=8;
cCutZ=10;

wthick=1.5;

lidDepth=5;

$fn=64;
module base(){
    difference(){
        rcube([x,pcbY+wthick*2,baseZ],1.5);
        translate([0,0,btmZ+pcbBtm])rcube([pcbX,pcbY,z],1);
        translate([0,0,btmZ])rcube([pcbX-wthick*2,pcbY-wthick*2,z],1);
        translate([pcbX/2,0,z-cCutZ])rcube([pcbX+wthick*3,cCutY,cCutZ],1);
      hull(){  
      translate([0,pcbY/2,z-lidDepth-jackDia/2])rotate([90,0,0])cylinder(h=20,d=jackDia,center=true);
      translate([0,pcbY/2,100])rotate([90,0,0])cylinder(h=20,d=jackDia,center=true);
      }
      translate([-(pcbX+slotXY)/2,pcbY/2-m3HeadDia/2,0])screw();
      translate([(pcbX+slotXY)/2,-(pcbY/2-m3HeadDia/2),0])screw();
      
    }
}

module lid(){
  rcube([pcbX+wthick*2,pcbY+wthick*2,btmZ],1.5);
  difference(){
    translate([0,0,btmZ])rcube([pcbX-t*2,pcbY-t*2,pcbZ],1);
    translate([0,0,btmZ])rcube([pcbX-wthick*2-t*2,pcbY-wthick*2-t*2,baseZ],1);
    translate([0,0,lidDepth+btmZ]){
      rcube([pcbX*2,pcbY*0.5,baseZ],1);
      rcube([pcbX*0.7,pcbY*2,baseZ],1);
    }
  }

}

translate([0,40,0])rotate([0,0,0])lid();
//translate([0,0,z+btmZ])rotate([180,0,0])lid();
base();

module screw(){
  cylinder(d=m3Dia,h=z*2);
  translate([0,0,baseZ-m3HeadZ])cylinder(d1=m3Dia,d2=m3HeadDia,h=m3HeadZ+0.01);
}


module rcube(size,radius){
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