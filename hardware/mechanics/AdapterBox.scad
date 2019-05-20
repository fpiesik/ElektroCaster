xI=101;
xISp=3;
yI=33;
zA=8;
zI=26;

ethCY=16;
ethCZ=14;

usbCY=10;
usbCYOff=3.5;
usbCZ=9;

pwrCY=9;
pwrCZ=12.5;

usbICX=32;
usbICYOff=-24;


wthick=2.5;

lid();

module base(){
difference(){
    rcube([xI+wthick*2,yI+wthick*2,zI+wthick],3);
    translate([0,0,wthick])rcube([xI-xISp*2,yI,zA],0);
    translate([usbICYOff,yI/2,wthick])rcube([usbICX,yI,zA],0);
    translate([0,0,wthick+zA])rcube([xI,yI,zI],0);
    translate([xI/2,0,wthick+zA])rcube([xI,ethCY,ethCZ],0);
    translate([-xI/2,usbCYOff,wthick+zA])rcube([xI,usbCY,usbCZ],0);
    translate([-xI/2,-yI/2+pwrCY/2,wthick+zA])rcube([xI,pwrCY,pwrCZ],0);
}
}

module lid(){
    rcube([xI+wthick*2,yI+wthick*2,2],3);
    difference(){
    rcube([xI,yI,2+3.5],3);
    rcube([xI*2,yI-wthick*2,2+3.5],3);
    }  
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