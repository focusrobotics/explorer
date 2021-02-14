// Model for the body of the robot

module first_level() {
  cylinder(r=150, h=5, center=true);
  // Add gearmotors and wheels
  // Add motor controller
  // Add battery
}

module second_level() {
  cylinder(r=150, h=5, center=true);
  // Second level supports, make these with a real 2020 extrusion profile
  translate([100, 100, -50]) cube([20, 20, 100], center=true);
  translate([-100, 100, -50]) cube([20, 20, 100], center=true);
  translate([100, -100, -50]) cube([20, 20, 100], center=true);
  translate([-100, -100, -50]) cube([20, 20, 100], center=true);
}

module monitor_mount() {

}

module gear_motors() {

}

module motor_controller() {

}

module battery() {

}

module raspberry_pi() {

}

module monitor_stand() {

}

first_level();
translate([0, 0, 100]) second_level();

