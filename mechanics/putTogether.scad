include <skeleton/slotBody.scad>;
include <head/head.scad>;
//include <bridge/bridge.scad>;
include <body/body.scad>;
include <neck/neck.scad>;
include <parameter.scad>;
include <neck/neck.scad>;
include <kickup_meBow/kickup.scad>;


//color([1,0,1])fretboard(1);
//color([1,0,1])fretboard(2);
//neckBack();
translate([0,0,-20])color([0,1,0])body();
//projection(cut = false) slotBody();
slotBody();
//slotBd_spacer();
neck(0);
translate([0,neckSlotY-slotXY*2-6,0])head();
//translate([0,neckSlotY-40,0])fret();

//translate([0,-170,0])hexPickup(0);
//translate([0,-(scale-neckSlotY),0])bridge(0);
//translate([0,-120,0])kickup();


neckEnd();

echo(39);