

use <gauge.scad>;
use <arduino.scad>;
use <arduino2.scad>;

guageRim = 4;
rimHeight = 3;
screwLength = 4;
faceHeight = 53.5;
faceWidth = 90 + guageRim;
bodyThickness = rimHeight + screwLength;
faceDepth = rimHeight + screwLength;
gaugeWidth = 70.5 + guageRim;
gaugeDepth = 80;
faceAngle = 15;
$fa = 2; // Set the face angle to one face per $fa degrees.
$fs = 0.1;
standoffsHeight = 2.5;


gaugeBodyDepth = gaugeDepth - (rimHeight + screwLength);
gaugeRimDepth = gaugeDepth - gaugeBodyDepth + 0;
baseDepth = 85; // How far back to make the base
baseHeight = bodyThickness; // The height (thickness) of the base
arduinoInset = baseHeight - 2.5; // how far down to "push" the arduino
arduinoTranslateX = -53.4/2; // half the arduino width
//arduinoTranslateX = 0;

backplateThickness = 8;
backplateLength = (cos(faceAngle) * baseDepth) + (tan(faceAngle) * baseHeight);
export_mode = 0; // When export mode is on, remove the arduino and gauge.
backplate_only = 1;
meterColor = [0.4, 0.4, 0.4];

if (backplate_only) {
    difference () {
        Enclosure(1);
        Enclosure(0);
    }
} else {
    Enclosure(0);
}



module Enclosure (backplate_mode = 0) {
    union () {
        difference () {
            translate ([-faceWidth/2,0,0]) {
                translate ([faceWidth/2,baseDepth,baseHeight]) {
                    difference () {
                        rotate ([faceAngle, 0, 0]) { // The face
                            translate([0, 0, faceHeight]) {
                                rotate ([90,0,0]) {
                                    difference() { // subtract the gauge from the holder
                                        union () { // The gauge holder
                                            Faceplate(faceDepth);
                                            /*
                                              cylinder (
                                                h = gaugeRimDepth,
                                                d = faceWidth
                                              );
                                              linear_extrude(height = faceDepth) {
                                                projection () { // The gauge faceplate
                                                  rotate([90,0,0]) {
                                                    h = faceHeight + baseHeight/sin(90 - faceAngle);
                                                    cylinder (h = h, d = faceWidth);
                                                  }
                                                }
                                              }
                                            */
                                            if (backplate_mode) {
                                                difference () {
                                                    union () { // The backplate shell
                                                        cylinder (
                                                            h = backplateLength,
                                                            d = faceWidth
                                                        );
                                                        linear_extrude(height = backplateLength) {
                                                            projection () { // The gauge faceplate
                                                                rotate([90,0,0]) {
                                                                    h = faceHeight + baseHeight/sin(90 - faceAngle);
                                                                    cylinder (h = h, d = faceWidth);
                                                                }
                                                            }
                                                        }
                                                    }
                                                    translate ([0,0, bodyThickness + backplateThickness/2]) {
                                                        union () { // the backplate guts
                                                            cylinder (
                                                                h = backplateLength - 2*backplateThickness,
                                                                d = faceWidth - backplateThickness
                                                            );
                                                            linear_extrude(height = backplateLength - 2*backplateThickness) {
                                                                projection () { // The gauge faceplate
                                                                    rotate([90,0,0]) {
                                                                        h = faceHeight + baseHeight/sin(90 - faceAngle);
                                                                        cylinder (h = h, d = faceWidth - backplateThickness);
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        MeterGauge();
                                    }
                                    // Comment this out when exporting
                                    if (!export_mode) {
                                        color (meterColor) MeterGauge();
                                    }
                                }
                            }
                        }
                        // Slice block (shaves off the bottom of the cylinder)
                        sliceBlockThickness = 4;
                        translate ([-70, -90, -(sliceBlockThickness + 1)*bodyThickness]) {
                            cube ([140, 140, bodyThickness*sliceBlockThickness]);
                        }
                    }
                }
                cube ([faceWidth, baseDepth, baseHeight]);
            }
            // Arduino cutout
            translate([arduinoTranslateX, 0, baseHeight - arduinoInset]) {
                wiggleRoom = 0.5;
                //translate([wiggleRoom, 3, 0]) boundingBox();
                //translate([-wiggleRoom, 3, 0]) boundingBox();
                ArduinoUno();
            }
        }
        if (!export_mode) {
          ArduinoUno();
        }
        // Arduino Standoffs
        translate ([arduinoTranslateX, 0, baseHeight - arduinoInset]) {
            standoffs (height = standoffsHeight, mountType=0);
            if (!export_mode) {
                translate([0, 0, standoffsHeight]) {
                    ArduinoUno();
                }
            }
        }
    }
}
module Faceplate (depth) {
  cylinder (h = gaugeRimDepth, d = faceWidth);
  linear_extrude(height = depth) {
    projection () { // The gauge faceplate
      rotate([90,0,0]) {
        h = faceHeight + baseHeight/sin(90 - faceAngle);
        cylinder (h = h, d = faceWidth);
      }
    }
  }
}
module ArduinoUno() {
  translate ([2.4,15.2,0]) {
    //rotate ([0,0,90]) Arduino();
  }
  components();
}

