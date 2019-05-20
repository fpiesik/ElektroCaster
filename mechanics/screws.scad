module screwA(dia,z,hDia,hZ,rY){
    rotate([0,rY,0]){
    $fn=12;
    cylinder(d1=hDia,d2=dia,h=hZ);
    cylinder(d=dia,h=z);
    }
}