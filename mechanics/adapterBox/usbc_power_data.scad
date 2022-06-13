wC=30;  // width oft the combined connectors
dC=23.5; //depth of the combined connectors
hC=22; //height of the cobined connectors
oC=2; //offset off the connectors

wE=16; //width of the rj45 connector
hE=14; //height of the rj45 connector

lsT=1.5; //lid left right support
wT=1.5;  //wall thickness
wI=wC+lsT*2; //width inside
wO=wI+wT*2; //width outside

dI=dC;  //depth inside
dO=dI+wT; //depth outside

hI=hC;  //height inside
hO=hI+wT*2; //height outside

wU=10;  //width of the usb-c connector
hU=3.5;  //height of the usc-c connector

eR=2; //edge radius
t=0.2; //tolerance of the lid
lD=5; //depth of the lid

sW=9.5;

base();
translate([0,30,wT+dO])rotate([0,180,0])lid();

module lid(){
  translate([0,0,dO]){  
    difference(){
      translate([oC,0,0]){
        rcube([wO,hO,wT],eR);
        translate([0,0,-lD])rcube([wI-t,hI-t,lD],0.1);
      }
      translate([0,hI/2-hE/2-hU-0.6,-6])rcube([wE,hE,10],0.1);
    }
  }
}

module base(){
  difference(){
    
    translate([oC,0,0])rcube([wO,hO,dO],eR);
    translate([oC,0,wT])rcube([wI,hI,dI+1],0.1);
    translate([0,hI/2-hU/2,-1])rcube([wU,hU,wT+2],0.1);
    translate([-(wI/2-hU/2-oC-lsT),0,-1])rcube([hU,wU,wT+2],0.1);
    translate([(wE+sW)/2,hI/2-8.75,-1])cylinder(d=5.3,h=10,$fn=12);
    translate([(wE+sW)/2,hI/2-15,-1])cylinder(d=5.3,h=10,$fn=12);
  }
}




module rcube(size,radius){
  $fn=16;
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