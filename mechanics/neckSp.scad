$fn=32;
y=44;
xA=20;
xB=40;
x=xA+xB;
zA=10;
zB=15;
z=zA+zB;

m4Dia=4.3;
rotate([90,0,0]){
difference(){
hull(){
translate([0,-y/2,zB-3])rotate([0,0,0])cube([xA,y,zA]);
translate([-xB,-y/2,0])cube([xA,y,z]);
}

translate([-xB+xA/2,-y/2+xA/2,0])cylinder(d=m4Dia,h=z);
translate([-xB+xA/2,-y/2+xA/2,0])cylinder(d=m4Dia*2,h=zA/2);

translate([-xB+xA/2,y/2-xA/2,0])cylinder(d=m4Dia,h=z);
translate([-xB+xA/2,y/2-xA/2,0])cylinder(d=m4Dia*2,h=zA/2);

translate([xA/2,-y/2+xA/2,0])cylinder(d=m4Dia,h=z);
translate([xA/2,-y/2+xA/2,0])cylinder(d=m4Dia*2,h=zB-3);

translate([xA/2,y/2-xA/2,0])cylinder(d=m4Dia,h=z);
translate([xA/2,y/2-xA/2,0])cylinder(d=m4Dia*2,h=zB-3);

}
}