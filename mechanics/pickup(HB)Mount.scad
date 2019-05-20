pickscrwDist=78;
scrwDist=104;
y=124;
x=15;
z=2;

m5Dia=5.3;
m2Dia=2.3;


difference(){
translate([-x/2,-y/2,0])cube([x,y,z]);

translate([0,scrwDist/2,0])cylinder(d=m5Dia,h=z);
translate([0,-scrwDist/2,0])cylinder(d=m5Dia,h=z);

translate([0,pickscrwDist/2,0])cylinder(d=m2Dia,h=z);
translate([0,-pickscrwDist/2,0])cylinder(d=m2Dia,h=z);
}