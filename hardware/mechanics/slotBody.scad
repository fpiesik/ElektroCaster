include <T-Slot.scad>;
include <parameter.scad>;

module slotBody(){
translate([-slotXY/2-neckSlotDist/2,neckSlotY/2,slotXY/2])rotate([90,0,0])2020Profile(neckSlotY);
translate([slotXY/2+neckSlotDist/2,neckSlotY/2,slotXY/2])rotate([90,0,0])2020Profile(neckSlotY);

translate([0,-slotXY/2,slotXY/2])rotate([0,90,0])2020Profile(bodySlotX);
translate([0,-bodySlotY+slotXY/2,slotXY/2])rotate([0,90,0])2020Profile(bodySlotX);

translate([-bodySlotX/2+slotXY/2,-bodySlotY/2,slotXY/2])rotate([90,0,0])2020Profile(bodySlotY-slotXY*2);
translate([-(-bodySlotX/2+slotXY/2),-bodySlotY/2,slotXY/2])rotate([90,0,0])2020Profile(bodySlotY-slotXY*2);

translate([-33,-slotXY/2-15,-slotXY/2])rotate([0,90,70])2020Profile(bodySlotX);
translate([33,-slotXY/2-15,-slotXY/2])rotate([0,90,-70])2020Profile(bodySlotX);
}

slotBody();