# Screen-to-World-D

Generates an extrinsics matrix, using an intrinsics matrix and a set of person detection boxes as required inputs.

## Overview

Generating a set of screen-to-world points correspondences could be a tedious process, usually it implies identifying points on a map tile service (e.g. OSM, Google Maps or Geoportail.fr). The application https://github.com/rodolfoap/points-picker could help, but the process is costly anyway. Mainly when there's a hundred cameras to calibrate.

So, for that, if we have the camera pole height, the camera bearing (towards north, in degrees), and some person detection boxes, distances from the camera can be calculated. With that, screen-to-world correspondences can be generated, and all the necessary elements to generate the extrinsics matrix are ready. This is, in some way, an automatic camera calibration.

See the [draft here](doc/CalibrationByMovingDetection.pdf) with a complete description of the process.

The actual relevant code (`autocalib.cpp`) takes 20 lines, standardizing the detection behavior with linear regressions, generating new sets from the original and generating the matrices...

## Build

```
mkdir -p build && pushd $_;
cmake ..
make
popd
```

### Usage Sample

##### 1. Put detections in CSV format

```
$ head test/detections.csv

        1,-1,1016.00,308.00,1042.00,356.00,0.747717543546,-1,-1,-1
        2,-1,781.00,415.00,820.00,488.00,0.514173738261,-1,-1,-1
        3,-1,519.00,546.00,573.00,651.00,0.722693597027,-1,-1,-1
        4,-1,274.00,503.00,324.00,603.00,0.515294991517,-1,-1,-1
        5,-1,649.00,270.00,671.00,310.00,0.608299698565,-1,-1,-1
        6,-1,34.00,404.00,70.00,477.00,0.747906120691,-1,-1,-1
        7,-1,277.00,340.00,305.00,397.00,0.644890807295,-1,-1,-1
        8,-1,1005.00,268.00,1025.00,309.00,0.592953132947,-1,-1,-1
        9,-1,1031.00,597.00,1090.00,716.00,0.860742203792,-1,-1,-1
        10,-1,1123.00,500.00,1172.00,598.00,0.835152783207,-1,-1,-1
        ...
```

##### 2. Get the intrinsics matrix

This can be obtained using https://github.com/rodolfoap/cameracalibration

```
$ cat test/camera_matrix_1280x720.yaml

        %YAML:1.0
        ---
        camera_matrix: !!opencv-matrix
           rows: 3
           cols: 3
           dt: d
           data: [ 9.8658267922418236e+02, 0., 6.5644063029267375e+02, 0.,
               9.2143502631862248e+02, 3.5621735537789453e+02, 0., 0., 1. ]
        distortion_coefficients: !!opencv-matrix
           rows: 5
           cols: 1
           dt: d
           data: [ 2.6029460512088087e-01, -1.8101009855718500e+00,
               -3.8176576250647945e-04, 1.4483555874521570e-02,
               5.9783794886029265e+00 ]
```

##### 3. Run the Calibration

```
$ ./gen-points test/detections.csv test/camera_matrix_1280x720.yaml

        Processing test/detections.csv...
        Done. Generating matrix...
        Done.
```

##### 4. Check result

The generated matrix is called `rotation_translation_matrix.yaml`. Test it with some points.

Incidentally, the application has generated a `points_matrix.yaml`, which you can also test with some https://geoportail.fr points.

```
$ cat rotation_translation_matrix.yaml

        %YAML:1.0
        ---
        rotation_matrix: !!opencv-matrix
           rows: 3
           cols: 3
           dt: d
           data: [ -4.8359770760309317e-01, 8.7471580995178844e-01,
               3.1709762241613038e-02, -2.2331512014778204e-01,
               -1.5832934118414987e-01, 9.6180152673697417e-01,
               8.4632358723741985e-01, 4.5804374413431237e-01,
               2.7190497263751068e-01 ]
        rotation_vector: !!opencv-matrix
           rows: 3
           cols: 1
           dt: d
           data: [ -8.0397627454137988e-01, -1.3000894692412277e+00,
               -1.7524112718452212e+00 ]
        translation_vector: !!opencv-matrix
           rows: 3
           cols: 1
           dt: d
           data: [ 1.9824397141331261e+01, 9.9559567883918714e+00,
               -3.7528364626343134e+01 ]
        camera_position: !!opencv-matrix
           rows: 3
           cols: 1
           dt: d
           data: [ 4.3571488872267587e+01, 1.4252391215663509e+00,
               -1.3240228121778462e-04 ]
```
