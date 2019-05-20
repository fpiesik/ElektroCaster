include <slotBody.scad>;
include <head.scad>;
include <bridge.scad>;
//include <body.scad>;
include <body.scad>;
//include <parameter.scad>;
//include <fret.scad>;
include <neck.scad>;
include <hexPickup.scad>; 
include <kickup.scad>;


color([1,0,1])fretboard(1);
//color([1,0,1])fretboard(2);
//neckBack();
color([0,1,0])body();
slotBody();
translate([0,neckSlotY,0])head();
//translate([0,-170,0])hexPickup(0);
translate([0,-(scale-neckSlotY),0])bridge(0);
//translate([0,-120,0])kickup();

