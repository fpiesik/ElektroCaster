include <parameter.scad>

wthick=3;
difference(){
    translate([0,0,bodyZ/2])cube([slotXY+wthick*2,slotXY+wthick*2,bodyZ],center=true);
    cylinder(d=2.9,h=bodyZ);
    translate([-slotXY/2,0,bodyZ-slotXY/2])cube([slotXY*2,slotXY,slotXY],center=true);
}
    
