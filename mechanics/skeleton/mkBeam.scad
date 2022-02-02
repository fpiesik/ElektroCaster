module mkBeam(height){
    size=15;
    translate([0,0,-height/2])difference(){
        rcube([size,size,height],0.5);
        for(i=[0:3]){
            rotate([0,0,i*90])translate([size/2+3,0,0])cube([size,3,height*3],center=true);
        }
    }
}

//mkBeam(50);

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