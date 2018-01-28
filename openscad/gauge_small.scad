//DH-52 Variant (the tiny one)
MeterGauge();
rimDiameter = 66.4;
rimHeight = 4.8;
bodyDiameter = 70.5;
bodyHeight = 47;

module MeterGauge() {
    union() {
        // Outer Rim
        cylinder(h = rimHeight, d = rimDiameter);
        
        // Body
        translate([0, 0, rimHeight]) {
            cylinder(h = bodyHeight, d = bodyDiameter);
        }
        
        // Terminals
        hull() {
            hull() {
                terminal();
                mirror([1,0,0]) terminal();
            }
            /*
            translate ([0, -70, 0]) {
                hull() {
                    terminal();
                    mirror([1,0,0]) terminal();
                }
            }
            */
        }
        
        // Screw holes
        translate([0, 40.1, 0]) {
            m3screw();
            translate([-69.4/2, -61.2]) {
                m3screw();
            }
            translate([69.4/2, -61.2]) {
                m3screw();
            }
        }
    }
}

module terminal() {
    translate([12, 17, rimHeight + bodyHeight]) {
        terminalHeight = 16;
        terminalDiameter = 12;
        cylinder(h = terminalHeight, d = terminalDiameter);
    }
}

module m3screw () {
    screwHeight = 18;
    screwDiameter = 3;
    cylinder(h= screwHeight, d = screwDiameter);
}