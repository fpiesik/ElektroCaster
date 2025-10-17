h=5;
dia=5.5;
$fn=32;

cylinder(h=h,d=dia);
translate([0,0,h])scale([1,1,0.5])sphere(dia/2);