include <parameter.scad>

spacer(6.3); //fader:8 rotary switches:9.3
//joyButton();
//button(8); //
//dispFrame();
//sixRotEnc();

module spacer(h){
    $fn=16;
    dO=6;
    dI=3.3;
    difference(){
        cylinder(d=dO,h=h);
        cylinder(d=dI,h=h);
    }
}

module joyButton(){
 
    $fn=32;
    dA=8;
    dAi=4.2;
    dAiB=3.2;
    dB=18;
    moldScale=0.3;
    moldZOff=1;
    
    zA=6;
    zAi=3.5;
    zB=2.5;
    
    difference(){
        union(){
            cylinder(d=dA,h=zA);
            translate([0,0,zA])cylinder(d=dB,h=zB);
        }
        difference(){
            cylinder(d=dAi,h=zAi);
            translate([dAi-(dAi-dAiB)/2,0,0])cube([dAi,dAi,zAi*2],center=true);
            translate([-dAi+(dAi-dAiB)/2,0,0])cube([dAi,dAi,zAi*2],center=true);
        }    
        translate([0,0,zA+zB+moldZOff])scale([1,1,moldScale])sphere(d=dB);
    }
}

module button(space){
    //space=7;
    swH=5;
    hub=1;
    $fn=32;
    dI=7.3;
    dOA=16;
    dOB=14;
    
    zI=swH-hub;
    zA=space-hub;
    zB=4;
    
    moldScale=0.3;
    moldZOff=1;
    
    difference(){
        union(){
            cylinder(d=dOA,h=zA);
            cylinder(d=dOB,h=zA+zB);
        }
        translate([0,0,zA+zB+moldZOff])scale([1,1,moldScale])sphere(d=dOB);
        cube([dI,dI,zI*2],center=true);
    }
}

module dispFrame(){
    xO=40;
    yO=35;
    
    xI=33;
    yI=28;
    
    iOffY=2;
    
    fW=4; //width of the frame
    
    hA=1.5; //height of the frame on surface
    hB=4;
    
    difference(){
        union(){
            rcube([xO+fW*2,yO+fW*2,hA],2);
            translate([0,0,hA])rcube([xO,yO,hB],2);
        }
        hull(){
            translate([0,0,0])rcube([xO,yO,0.6],2);
            translate([0,0,hA+hB-0.7])rcube([xI,yI,0.1],2);
        }
        translate([0,0,hA+hB-0.6])rcube([xI,yI,0.6],2);
    }
}

module sixRotEnc(){
    strings=6;
    spacing=70;
    screwDia=4.4;
    centY=10;
    rotD=7.5;
    y=30;
    h=2.5;
    difference(){
        translate([0,0,0])rcube([bodySlotX,y,h],3);
        translate([-spacing/2,0,0]){
            for(x=[0:strings-1]){
                translate([x*(spacing/(strings-1)),0,0]){
                    translate([0,-y/2+centY,0])cylinder(d=rotD,h=h+1);
                    //cylinder(d=screwTdia,h=tZ);
                }
            }
        }
        translate([-bodySlotX/2+slotXY/2,0,0])cylinder(d=screwDia,h=h+1);
        translate([bodySlotX/2-slotXY/2,0,0])cylinder(d=screwDia,h=h+1);
    }
}


module spacerDisp(){
}

module rcube(size,radius){
$fn=32;
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

