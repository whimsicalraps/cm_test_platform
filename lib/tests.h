#pragma once

#include "ll/state.h"
#include "ll/adc.h"

typedef struct TestV{
    char*           name;
    Destination_t   dest;
    Volts_t         volts;
    ADC_t           channel;
    float           expect;
} TestV_t;

typedef struct TestVV{
    char*           name;
    Destination_t   dest;
    Volts_t         volts;
    Destination_t   dest2;
    Volts_t         volts2;
    ADC_t           channel;
    float           expect;
} TestVV_t;

static const TestV_t vtests[] =
{ { "+RAW"      , Set_NA        , Volt_0
                , Get_RAW       , 12.0
                }
, { "-RAW"      , Set_NA        , Volt_0
                , Get_RAWn      , -12.0
                }

, { "CREASE"    , Set_Crease    , Volt_5
                , Get_Crease    , 0.32
                }
, { "CREASE"    , Set_Crease    , Volt_4
                , Get_Crease    , 0.0
                }
, { "CREASE"    , Set_Crease    , Volt_1
                , Get_Crease    , 0.0
                }
, { "CREASE"    , Set_Crease    , Volt_n2v5
                , Get_Crease    , 1.17
                }
, { "CREASE"    , Set_Crease    , Volt_n5
                , Get_Crease    , 0.0
                }
};

typedef struct TestT{
    char*           name;
    Destination_t   dest;
    Volts_t         rest_volts;
    int             rest_delay;
    Volts_t         go_volts;
    int             read_delay;
    ADC_t           channel;
    float           expect;
} TestT_t;

// These likely need a wider tolerance window
static const TestT_t ttests[] =
{ { "FOLLOW"    , Set_Fold      , Volt_0    , 100
                                , Volt_5    , 100
                , Get_Follow    , 2.91
                }
, { "LOCATION"  , Set_Crease    , Volt_0    , 100
                                , Volt_5    , 500
                , Get_Location  , 1.21
                }
};

typedef struct TestV_vec{
    char*           name;
    Destination_t   dest;
    ADC_t           channel;
    float           expect0;
    float           expect1;
    float           expect2;
    float           expect3;
    float           expect4;
    float           expect5;
    float           expect6;
    float           expect7;
} TestV_vec_t;

// nb: negative values <-5 are 'don't care' and test is skipped
static const TestV_vec_t vtests_vec[] =
//  .name         .dest           .channel
//               -5    -2.5    0       1       2       3       4       5
{ { "CREASE"   , Set_Crease    , Get_Crease
                            // 0v is non-determinate due to hysteresis
               , 0.0  ,1.17   ,-999   ,0.0    ,0.0    ,0.0    ,0.0    ,0.32
               }
, { "Fullwave" , Set_Fold , Get_Fullwave
                            // 0v is non-determinate due to hysteresis
               //, 9.0  ,9.0   ,9.0   ,9.0    ,9.0    ,9.0    ,9.0    ,9.0
               , 3.31 ,1.58   ,0.01   ,0.65   ,1.27   ,1.89   ,2.57   ,3.32
               }
};

typedef struct TestV_vec2d{
    char*           name;
    Destination_t   dest;
    ADC_t           channel;
    Destination_t   dest_static;
    Volts_t         volt_static;
    float           expect0;
    float           expect1;
    float           expect2;
    float           expect3;
    float           expect4;
    float           expect5;
    float           expect6;
    float           expect7;
} TestV_vec2d_t;

static const TestV_vec2d_t vtests_vec2d[] =
//  .name         .dest(sweep)    .channel
//                .dest(static)   .volts(static)
//               -5    -2.5    0       1       2       3       4       5
{ { "LEFT -5"  , Set_Left    , Get_Left
               , Set_XVCAOut , Volt_n5
               , 0.0  ,1.75  ,3.34  ,4.01    ,4.66   ,5.30   ,5.96   ,6.2
               }
, { "LEFT 0"   , Set_Left    , Get_Left
               , Set_XVCAOut , Volt_0
               , 0.0  ,0.0   ,0.0   ,0.65    ,1.29   ,1.93   ,2.60   ,3.32
               }
, { "LEFT +5"  , Set_Left    , Get_Left
               , Set_XVCAOut , Volt_5
               , 0.0  ,0.0   ,0.0    ,0.0    ,0.0    ,0.0    ,0.0    ,0.0
               }

, { "RIGHT -5" , Set_Right   , Get_Right
               , Set_XVCAOut , Volt_n5
               , 0.0  ,0.0   ,0.0    ,0.0    ,0.0    ,0.0    ,0.0    ,0.0
               }
, { "RIGHT 0"  , Set_Right   , Get_Right
               , Set_XVCAOut , Volt_0
               , 0.0  ,0.0   ,0.0   ,0.67    ,1.30   ,1.95   ,2.62   ,3.32
               }
, { "RIGHT +5" , Set_Right   , Get_Right
               , Set_XVCAOut , Volt_5
               , 0.04  ,1.73 ,3.35  ,4.00    ,4.64   ,5.28   ,5.95   ,6.61
               }

, { "PEAK 0 "  , Set_Peak1      , Get_Peak
               , Set_Peak2    , Volt_0
               , 0.0  ,0.0   ,0.0   ,0.59   ,1.20   ,1.85   ,2.54   ,3.31
               }
, { "PEAK 2 "  , Set_Peak1      , Get_Peak
               , Set_Peak2    , Volt_2
               , 1.20, 1.20, 1.20, 1.20, 1.21, 1.85, 2.54, 3.31
               }
, { "PEAK 5 "  , Set_Peak1      , Get_Peak
               , Set_Peak2    , Volt_5
               , 3.30, 3.30, 3.30, 3.30, 3.30, 3.30, 3.30, 3.32
               }

, { "TROUGH 1" , Set_Trough1    , Get_Trough
               , Set_Trough2    , Volt_1
               , 0.0, 0.0, 0.02, 0.71, 0.73, 0.73, 0.73, 0.73
               }
, { "TROUGH 3" , Set_Trough1    , Get_Trough
               , Set_Trough2    , Volt_3
               , 0.0, 0.0, 0.02, 0.73, 1.40, 2.03, 2.05, 2.05
               }
, { "TROUGH 5" , Set_Trough1    , Get_Trough
               , Set_Trough2    , Volt_5
               , 0.0, 0.0, 0.02, 0.73, 1.40, 2.04, 2.68, 3.33
               }
//, { "LEFT "    , Set_Left       , Get_Left
//               , Set_XVCAOut    , Volt_0
//               , 9.0  ,9.0   ,9.0   ,9.0    ,9.0    ,9.0    ,9.0    ,9.0
//               }
};


//static const TestVV_t vvtests[] =
//{ { "+RAW"      , Set_NA        , Volt_0
//                , Set_NA2       , Volt_1
//                , Get_RAW       , 12.0
//                }
//, { "-RAW"      , Set_NA        , Volt_0
//                , Set_NA2       , Volt_1
//                , Get_RAWn      , -12.0
//                }
//};

uint8_t Test_RunSuite( void );
uint8_t Test_V( const TestV_t* test );
uint8_t Test_T( const TestT_t* test );
uint8_t Test_V_vec( const TestV_vec_t* test );
uint8_t Test_V_vec2d( const TestV_vec2d_t* test );
float GetVolts( ADC_t chan );
