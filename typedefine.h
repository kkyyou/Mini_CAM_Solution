#ifndef TYPEDEFINE_H
#define TYPEDEFINE_H

#define _MILLION    1000000

enum FEATURE_TYPE
{
    _FEATURE_PAD,
    _FEATURE_LINE
};

enum SHAPE_TYPE
{
    _SHAPE_ROUND,
    _SHAPE_RECT
};

#define _ROUND       "Round"
#define _RECTANGLE   "Rectangle"
#define _CENTER_PT   "CenterPt"
#define _START_PT    "StartPt"
#define _END_PT      "EndPt"
#define _RADIUS      "Radius"
#define _WIDTH       "Width"
#define _HEIGHT      "Height"
#define _STEP_0      0
#define _STEP_1      1
#define _STEP_2      2

// Command Define.
#define _ADD_PAD     "Add Pad"
#define _ADD_LINE    "Add Line"

#define _NO_COMMAND  "No Command"

// XML ELEMENT.
#define _LAYERS         "LAYERS"
#define _LAYER          "LAYER"
#define _LAYER_NAME     "NAME"
#define _FEATURE        "FEATURE"
#define _PAD            "PAD"
#define _LINE           "LINE"
#define _FEATURE_TYPE   "FEATURE_TYPE"
#define _SHAPE_TYPE     "SHAPE_TYPE"
#define _CENTER_X       "CENTER_X"
#define _CENTER_Y       "CENTER_Y"
#define _START_X        "START_X"
#define _START_Y        "START_Y"
#define _END_X          "END_X"
#define _END_Y          "END_Y"

#endif // TYPEDEFINE_H
