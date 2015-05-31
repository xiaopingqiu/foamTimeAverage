## foamTimeAverage
foamTimeAverage is a piece of code used to calculate time average of a specified volField over a specified time range. It is programmed based on [Eelco van Vliet's code](http://www.cfd-online.com/Forums/openfoam-programming-development/70396-using-fieldaverage-library-average-postprocessing.html) and `patchAverage` which is one of the postprocessing utilities provided by OpenFOAM. In Eelco van Vliet's version, only `volScalarField` is supported, but in this version, all volFields(including `volScalarField`, `volVectorField`, `volTensorField`, `volSymmTensorField`, `volSphericalTensorField`) are supported.  

### Build
Just run `wmake`, and the executable will be generated in `$FOAM_USER_APPBIN`

### Usage
```
foamTimeAverage [fieldname] [options]
```

For example
```
foamTimeAverage p -time 0.4:0.5
```
will calculate time average of field `p` over the time range 0.4-0.5, and the time average value will be worte in time directory 0.5, named as `p_mean`.

### Warning
This program can be built successfully on OpenFOAM-2.1.1 and OpenFOAM-2.3.1, but the results are only tested on OpenFOAM-2.1.1 for `volScalarField` and `volVectorField`. In principle, it will work normally on other version of OpenFOAM, but no further test is carried out.
