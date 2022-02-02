include <mkBeam.scad>;
include <../parameter.scad>;
include <../screws.scad>;

//slotBody();

module slotBody(){
    hull(){
        translate([fretL(0)/2-slotXY/2,neckSlotY-0.05-slotXY*2,-slotXY/2])rotate([90,0,0])mkBeam(0.1);
        translate([fretL(neckSlotY-nut_slotSink)/2-slotXY/2,0.05-slotXY*2,-slotXY/2])rotate([90,0,0])mkBeam(0.1);
    }
    hull(){
        translate([-(fretL(0)/2-slotXY/2),neckSlotY-0.05-slotXY*2,-slotXY/2])rotate([90,0,0])mkBeam(0.1);
        translate([-(fretL(neckSlotY-nut_slotSink)/2-slotXY/2),0.05-slotXY*2,-slotXY/2])rotate([90,0,0])mkBeam(0.1);
    }
//translate([-slotXY/2-neckSlotDist/2,neckSlotY/2-slotXY*2,-slotXY/2])rotate([90,0,0])mkBeam(neckSlotY);
//translate([slotXY/2+neckSlotDist/2,neckSlotY/2-slotXY*2,-slotXY/2])rotate([90,0,0])mkBeam(neckSlotY);

translate([0,-slotXY/2,-slotXY/2-slotXY])rotate([0,90,0])mkBeam(bodySlotX);
translate([0,-slotXY/2-slotXY,-slotXY/2-slotXY])rotate([0,90,0])mkBeam(bodySlotX);

translate([0,-bodySlotY+slotXY/2,-slotXY/2-slotXY])rotate([0,90,0])mkBeam(bodySlotX);

translate([-bodySlotX/2+slotXY/2,-bodySlotY/2,-slotXY/2])rotate([90,0,0])mkBeam(bodySlotY);
translate([-(-bodySlotX/2+slotXY/2),-bodySlotY/2,-slotXY/2])rotate([90,0,0])mkBeam(bodySlotY);

}

module slotBd_spacer(){
difference(){
    x=bodySlotX-slotXY*2;
    y=fretPos(24)-fretPos(22)+neck_lastFretSpc+neck_mntEndT;//slotXY*2;//+neck_mntEndT;
    z=6.5;
    scrwD=5;
    sink=slotXY;
translate([-x/2,-y,-sink])cube([x,y,z]);
translate([-x/4,-y/2,-sink-1])cube([x/2,y,z+2]);

translate([x/2-scrwD,-slotXY/2,-sink+z+0.1])screwA(3.3,20,7,2.5,180);
translate([x/2-scrwD,-slotXY*1.5,-sink+z+0.1])screwA(3.3,20,7,2.5,180);
    
translate([-(x/2-scrwD),-slotXY/2,-sink+z+0.1])screwA(3.3,20,7,2.5,180);
translate([-(x/2-scrwD),-slotXY*1.5,-sink+z+0.1])screwA(3.3,20,7,2.5,180);

translate([0,0,-1])slotBody();

}
}
    
//slotBd_spacer();