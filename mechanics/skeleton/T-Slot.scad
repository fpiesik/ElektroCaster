/** 
 * @file
 * Generic library for fractional T-Slot extrusions.
 *
 * Creative Commons Share Alike 3.0
 * Copyright (c) 2014 Manuel García
 *
 * Original library (David Lee Miller)
 * https://www.thingiverse.com/thing:136430
 */ 

ProfileCore = 4.3;     // Profile core Ø (Default M5).
minkR_TS = 0.04 * 20;  // Minkowski radius for the T-Slot.
minkR_IC = 0.075 * 20; // Minkowski radius for the inner cutout.
minkR_PF = 0.05 * 20;  // Minkowski radius for the profile corners.

//1515Profile(100);
//2020Profile(100);
//2040Profile(100);
//2060Profile(100);
//2080Profile(100);
//4040Profile(100);
//4060Profile(100);
//4080Profile(100);

module fillet(rad) {
  translate([-rad, -rad, 0])
  difference() {
    translate([0, 0, 0]) square([rad + 0.01, rad + 0.01]);
    circle(r = rad, center = true, $fn = 32);
  }
}

module insideCutout() {
  minkowski() {
    translate([0, 0, 0]) circle(r = minkR_IC, center = true, $fn = 32);
    hull() {
      square([0.2 * 20 - minkR_IC, 0.645 * 20 - 2 * minkR_IC], center = true);
      square([0.8 * 20 - 2 * minkR_IC, 0.001 * 20], center = true);
    }
  }
}

module doubleCutout() {
  union() {
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_IC, center = true, $fn = 32);
      union() {
        rotate([0, 0, 0]) hull() {
          translate([-0.5 * 20, 0, 0]) hull() {
            square([0.2 * 20 - minkR_IC, 0.645 * 20 - 2 * minkR_IC], center = true);
            square([0.8 * 20 - 2 * minkR_IC, 0.001 * 20], center = true);
          }
          translate([0.5 * 20, 0, 0]) hull() {
            square([0.2 * 20 - minkR_IC, 0.645 * 20 - 2 * minkR_IC], center = true);
            square([0.8 * 20 - 2 * minkR_IC, 0.001 * 20], center = true);
          }
        }
        rotate([0, 0, 90]) hull() {
          translate([-0.5 * 20, 0, 0]) hull() {
            square([0.2 * 20 - minkR_IC, 0.645 * 20 - 2 * minkR_IC], center = true);
            square([0.8 * 20 - 2 * minkR_IC, 0.001 * 20], center = true);
          }
          translate([0.5 * 20, 0, 0]) hull() {
            square([0.2 * 20 - minkR_IC, 0.645 * 20 - 2 * minkR_IC], center = true);
            square([0.8 * 20 - 2 * minkR_IC, 0.001 * 20], center = true);
          }
        }
      }
    }
    rotate([0, 0, 0]) translate([-0.645 * 20 / 2, -0.645 * 20 / 2, 0]) fillet(minkR_IC);
    rotate([0, 0, 180]) translate([-0.645 * 20 / 2, -0.645 * 20 / 2, 0]) fillet(minkR_IC);
    rotate([0, 0, 90]) translate([-0.645 * 20 / 2, -0.645 * 20 / 2, 0]) fillet(minkR_IC);
    rotate([0, 0, -90]) translate([-0.645 * 20 / 2, -0.645 * 20 / 2, 0]) fillet(minkR_IC);
  }
}

module tSlot() {
  union() {
    translate([minkR_TS, 0, 0])
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_TS, center = true, $fn = 32);
      hull() {
        square([0.001 * 20, 0.585 * 20 - 2 * minkR_TS], center = true);
        translate([(0.233 * 20 - 2 * minkR_TS) / 2, 0, 0]) square([0.233 * 20 - 2 * minkR_TS, 0.2 * 20], center = true);
      }
    }
    translate([-0.255 * 20 / 2 + 0.01, 0, 0]) square(0.255 * 20, center = true);
    translate([-0.35 * 20 / 2 - 0.087 * 20 + 0.01, 0, 0]) square(0.35 * 20, center = true);
    translate([0, -0.255 * 20 / 2, 0]) fillet(minkR_TS / 2);
    translate([-0.087 * 20, -0.255 * 20 / 2, 0]) rotate([0, 0, 90]) fillet(minkR_TS / 2);
    scale([1, -1, 1]) translate([0, -0.255 * 20 / 2, 0]) fillet(minkR_TS / 2);
    scale([1, -1, 1]) translate([-0.087 * 20, -0.255 * 20 / 2, 0]) rotate([0, 0, 90]) fillet(minkR_TS / 2);
  }
}

module 2020Profile(height, core = ProfileCore) {
  linear_extrude(height = height, center = true)
  union() {
    difference() {
      minkowski() {
        translate([0, 0, 0]) circle(r = minkR_PF, center = true, $fn = 32);
        square([1 * 20 - 2 * minkR_PF, 1 * 20 - 2 * minkR_PF], center = true);
      }
      translate([0, 0, 0]) circle(r = core / 2, $fn = 24);
      translate([-0.5 * 20 + 0.087 * 20, 0, 0]) tSlot();
      rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, 0, 0]) tSlot();
      translate([0, -0.5 * 20 + 0.087 * 20, 0]) rotate([0, 0, 90]) tSlot();
      translate([0, 0.5 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    }
  }
}

module 1515Profile(height, core = ProfileCore) {
ProfileCore = 3.1;     // Profile core Ø (Default M5).
minkR_TS = 0.04 * 15;  // Minkowski radius for the T-Slot.
minkR_IC = 0.075 * 15; // Minkowski radius for the inner cutout.
minkR_PF = 0.05 * 15;  // Minkowski radius for the profile corners.
  linear_extrude(height = height, center = true)
  union() {
    difference() {
      minkowski() {
        translate([0, 0, 0]) circle(r = minkR_PF, center=true, $fn = 32);
        square([1 * 15 - 2 * minkR_PF, 1 * 15 - 2 * minkR_PF], center = true);
      }
      translate([0, 0, 0]) circle(r = core / 2, $fn = 24);
      translate([-0.5 * 15 + 0.087 * 15, 0, 0]) tSlot();
      rotate([0, 0, 180]) translate([-0.5 * 15 + 0.087 * 15, 0, 0]) tSlot();
      translate([0, -0.5 * 15 + 0.087 * 15, 0]) rotate([0, 0, 90]) tSlot();
      translate([0, 0.5 * 15 - 0.087 * 15, 0]) rotate([0, 0, -90]) tSlot();
    }
  }
}

module 2040Profile(height, core = ProfileCore) {
  linear_extrude(height = height, center = true)
  difference() {
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_PF, center = true, $fn = 32);
      square([1 * 20 - 2 * minkR_PF, 2 * 20 - 2 * minkR_PF], center = true);
    }
    translate([0, 0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, -0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 0, 0]) insideCutout();
    translate([-0.5 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    translate([-0.5 * 20 + 0.087 * 20, -0.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, -0.5 * 20, 0]) tSlot();
    translate([0, -1 * 20 + 0.087 * 20, 0]) rotate([0, 0, 90]) tSlot();
    translate([0, 1 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
  }
}

module 2060Profile(height, core = ProfileCore) {
  linear_extrude(height = height, center = true)
  difference() {
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_PF, center = true, $fn = 32);
      square([1 * 20 - 2 * minkR_PF, 3 * 20 - 2 * minkR_PF], center = true);
    }
    translate([0, 0, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 1 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, -1 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, -0.5 * 20, 0]) insideCutout();
    translate([0, 0.5 * 20, 0]) insideCutout();
    translate([-0.5 * 20 + 0.087 * 20, 0, 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, 0, 0]) tSlot();
    translate([-0.5 * 20 + 0.087 * 20, 1 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, 1 * 20, 0]) tSlot();
    translate([-0.5 * 20 + 0.087 * 20, -1 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, -1 * 20, 0]) tSlot();
    translate([0, -1.5 * 20 + 0.087 * 20, 0]) rotate([0, 0, 90]) tSlot();
    translate([0, 1.5 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
  }
}

module 2080Profile(height, core = ProfileCore) {
  linear_extrude(height = height, center = true)
  difference() {
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_PF, center = true, $fn = 32);
      square([1 * 20 - 2 * minkR_PF, 4 * 20 - 2 * minkR_PF], center = true);
    }
    translate([0, 0.5 * 20 + 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 0.5 * 20 + 20 - 1 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 0.5 * 20 + 20 - 2 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 0.5 * 20 + 20 - 3 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 0.5 * 20 + 20 - 0.5 * 20, 0]) insideCutout();
    translate([0, 0, 0]) insideCutout();
    translate([0, -(0.5 * 20 + 20) + 0.5 * 20, 0]) insideCutout();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, -(0.5 * 20 + 20), 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, -(0.5 * 20), 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20 + 0.087 * 20, 0.5 * 20 + 20, 0]) tSlot();
    translate([-0.5 * 20 + 0.087 * 20, 0.5 * 20 + 20, 0]) tSlot();
    translate([-0.5 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    translate([-0.5 * 20 + 0.087 * 20, -(0.5 * 20), 0]) tSlot();
    translate([-0.5 * 20 + 0.087 * 20, -(0.5 * 20 + 20), 0]) tSlot();
    translate([0, -2 * 20 + 0.087 * 20, 0]) rotate([0, 0, 90]) tSlot();
    translate([0, 2 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
  }
}

module 4040Profile(height, core = ProfileCore) {
  linear_extrude(height = height, center = true)
  difference() {
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_PF, center = true, $fn = 32);
      square([2 * 20 - 2 * minkR_PF, 2 * 20 - 2 * minkR_PF], center = true);
    }
    translate([0.5 * 20, 0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, 0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0.5 * 20, -0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, -0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 0, 0]) doubleCutout();
    translate([-1 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    translate([-1 * 20 + 0.087 * 20, -0.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, -0.5 * 20, 0]) tSlot();
    translate([-0.5 * 20, 1 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20, 1 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    translate([0.5 * 20, 1 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    rotate([0, 0, 180]) translate([0.5 * 20, 1 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
  }
}

module 4060Profile(height, core = ProfileCore) {
  linear_extrude(height = height, center = true)
  difference() {
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_PF, center = true, $fn = 32);
      square([2 * 20 - 2 * minkR_PF, 2 * 30 - 2 * minkR_PF], center = true);
    }
    translate([0.5 * 20, 1 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, 1 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0.5 * 20, 0, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, 0, 0]) circle(r = core / 2, $fn = 24);
    translate([0.5 * 20, -1 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, -1 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-1 * 20 / 2, 1 * 20 / 2, 0]) insideCutout();
    translate([0, 2 * 20 / 2, 0]) rotate([0, 0, -90]) insideCutout();
    translate([1 * 20 / 2, 1 * 20 / 2, 0]) insideCutout();
    translate([0, -20 / 2, 0]) doubleCutout();
    translate([-1 * 20 + 0.087 * 20, 1 * 20, 0]) tSlot();
    translate([-1 * 20 + 0.087 * 20, 0, 0]) tSlot();
    translate([-1 * 20 + 0.087 * 20, -1 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, 1 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, 0, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, -1 * 20, 0]) tSlot();
    translate([-1 * 20 / 2, -1.5 * 20 + 0.087 * 20, 0]) rotate([0, 0, 90]) tSlot();
    translate([1 * 20 / 2, -1.5 * 20 + 0.087 * 20, 0]) rotate([0, 0, 90]) tSlot();
    translate([-1 * 20 / 2, 1.5 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    translate([1 * 20 / 2, 1.5 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
  }
}

module 4080Profile(height, core = ProfileCore) {
  linear_extrude(height = height, center = true)
  difference() {
    minkowski() {
      translate([0, 0, 0]) circle(r = minkR_PF, center = true, $fn = 32);
      square([2 * 20 - 2 * minkR_PF, 4 * 20 - 2 * minkR_PF], center = true);
    }
    translate([0.5 * 20, 1.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, 1.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0.5 * 20, 0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, 0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0.5 * 20, -1.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, -1.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0.5 * 20, -0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([-0.5 * 20, -0.5 * 20, 0]) circle(r = core / 2, $fn = 24);
    translate([0, 1 * 20, 0]) doubleCutout();
    translate([0, -1 * 20, 0]) doubleCutout();
    translate([0.5 * 20, 0, 0]) insideCutout();
    translate([-0.5 * 20, 0, 0]) insideCutout();
    translate([-1 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, 0.5 * 20, 0]) tSlot();
    translate([-1 * 20 + 0.087 * 20, -0.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, -0.5 * 20, 0]) tSlot();
    translate([-1 * 20 + 0.087 * 20, 1.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, 1.5 * 20, 0]) tSlot();
    translate([-1 * 20 + 0.087 * 20, -1.5 * 20, 0]) tSlot();
    rotate([0, 0, 180]) translate([-1 * 20 + 0.087 * 20, -1.5 * 20, 0]) tSlot();
    translate([-0.5 * 20, 2 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    rotate([0, 0, 180]) translate([-0.5 * 20, 2 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    translate([0.5 * 20, 2 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
    rotate([0, 0, 180]) translate([0.5 * 20, 2 * 20 - 0.087 * 20, 0]) rotate([0, 0, -90]) tSlot();
  }
}
