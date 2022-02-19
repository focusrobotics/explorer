// Arduino camera mount for my robot Explorer

module cam_base() {
  difference() {
    union() {
      // base plate
      cube([25, 18, 2], center=true);

      // 4 stand-offs
      translate([-10.5, 6.25, 0]) cylinder(d=3.2, h=4);
      translate([-10.5, -6.25, 0]) cylinder(d=3.2, h=4);
      translate([10.5, 6.25, 0]) cylinder(d=3.2, h=4);
      translate([10.5, -6.25, 0]) cylinder(d=3.2, h=4);

      // connect the stand-offs up the sides for strength
      // these would conflict with board components
      //translate([-10.5, 0, 1.5]) cube([3.2, 12.5, 4], center=true);
      //translate([10.5, 0, 1.5]) cube([3.2, 12.5, 4], center=true);

      // tab for connecting to the base
      translate([-2.5, -13, 2]) rotate([0, 90, 0]) cylinder(d=6, h=5);
      translate([0, -12, 0]) cube([5, 7, 2], center=true);
      }
    union() {
      translate([-10.5, 6.25, 0]) cylinder(d=1.6, h=12, center=true);
      translate([-10.5, -6.25, 0]) cylinder(d=1.6, h=12, center=true);
      translate([10.5, 6.25, 0]) cylinder(d=1.6, h=12, center=true);
      translate([10.5, -6.25, 0]) cylinder(d=1.6, h=12, center=true);
      translate([-4, -13, 2]) rotate([0, 90, 0]) cylinder(d=3, h=8);
    }
  }
}

module robot_mount() {
  difference() {
    union() {
      cube([25, 25, 2], center=true);
      translate([-8, -18, 2]) rotate([0, 90, 0]) cylinder(d=6, h=16);
      translate([0, -16, 0]) cube([16, 9, 2], center=true);
    }
    union() {
      translate([-40, -18, 2]) rotate([0, 90, 0]) cylinder(d=3, h=80);
      translate([0, -18.5, 0]) cube([6, 12, 12], center=true);
      cylinder(d=3, h=20, center=true);
    }
  }
}

module extension_arm() {
}

$fn=12;
cam_base();
translate([30, 0, 0]) robot_mount();