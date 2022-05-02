include <../parameter.scad>
strContact();
module strContact(){
strings=nStrings;
strToStr=strSpanB;
z=4;

spacing=strToStr/(strings-1);
strDia=3;
strSpD=4.5;
strSpZ=5;
oeseDia=7;
oeseShft=4.5;
x=strToStr+oeseDia+1;
y=oeseDia*2;


difference(){
translate([0,y/2-oeseDia/2-0.5,z/2])cube([x,y,z],center=true);

for(x=[0:strings-1]){
translate([x*(spacing)-spacing*strings/2+spacing/2,0,0]){
cylinder(h=z, d=strDia, $fn=12);
translate([0,0,z/2])cylinder(h=z/2, d=oeseDia, $fn=12);
translate([-oeseShft/2,0,z/2])cube([oeseShft,y,z/2]);
}
}

}
/*
for(x=[0:strings-1]){
translate([x*(spacing)-spacing*strings/2+spacing/2,0,-strSpZ]){
difference(){
cylinder(h=strSpZ, d=strSpD, $fn=12);
cylinder(h=strSpZ, d=strDia, $fn=12);}
}
}
*/
}