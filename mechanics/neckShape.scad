xI=44;
xO=55;
z=30;
yI=20;
iR=0.001;
$fn=64;

fret();

module fret(){
difference(){
scale([1,0.8,1])cylinder(d=xO,h=z);
translate([0,0,-1])rcube([xI,yI*2,z+2],iR);
translate([0,-yI,-1])rcube([xO,yI,z+2],iR);
translate([0,yI,-1])rcube([xO,yI,z+2],iR);
}

difference(){
translate([0,yI/2,0])scale([1.13,0.1,1])cylinder(d=xI,h=z);
translate([0,0,-1])rcube([xI*2,yI,z+2],iR);
translate([0,-yI,-1])rcube([xO,yI*2,z+2],iR);
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
}

module space(){
difference(){
scale([1,0.8,1])cylinder(d=xO,h=z);
translate([0,0,-1])rcube([xI,yI*2,z+2],iR);
translate([0,-yI,-1])rcube([xO,yI,z+2],iR);
translate([0,yI,-1])rcube([xO,yI,z+2],iR);
}

difference(){
translate([0,yI/2-1.15,0])scale([1.13,0.3,1])cylinder(d=xI,h=z);
translate([0,0,-1])rcube([xI*2,yI,z+2],iR);
translate([0,-yI,-1])rcube([xO,yI*2,z+2],iR);
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
}