#include "tests.h"

#include <stm32f7xx_hal.h>

#include "ll/debug_usart.h"
#include "hw_setup.h"
#include "wrMath.h"

#define LENGTH(X) (sizeof X / sizeof X[0])
#define POWER_STARTUP_DELAY    500
#define VOLTAGE_SETTLING_DELAY 5
#define TOLERANCE              (float)0.1 // percentage: 0.1 = 10%
#define VERBOSE 0

static uint8_t _Test_inner( const TestV_t* test );
static uint8_t Test_PowerUp( void );
static void Test_Failure( char* error );
uint8_t Test_RunSuite( void )
{
    // call this "run tests"
    if( Test_PowerUp() ){
        U_PrintLn("POWER ISSUE. SHUTTING DOWN");
        U_PrintNow();
        return 1;
    }
// RUN TESTS
    // single voltage set & read
    int failures = 0;
    int tests_run = 0;

    int len = LENGTH(vtests);
    for( int i=0; i<len; i++ ){
        if( Test_V( &vtests[i] ) ){ failures++; }
    }

    len = LENGTH(vtests_vec);
    for( int i=0; i<len; i++ ){
        if( Test_V_vec( &vtests_vec[i] ) ){ failures++; }
    }

    len = LENGTH(vtests_vec2d);
    for( int i=0; i<len; i++ ){
        if( Test_V_vec2d( &vtests_vec2d[i] ) ){ failures++; }
        U_PrintNow(); // flush uart
    }

    LL_PowerOn_Set(0);
    if( failures ){
        U_Print("Ran "); U_PrintU16n(tests_run); U_PrintLn(" tests");
        U_Print("\t"); U_PrintU16n(tests_run - failures); U_PrintLn(" successes");
        U_Print("\t"); U_PrintU16n(failures); U_PrintLn(" failures");
        U_PrintLn("\nTESTS FAILED");
        return 1;
    }
    U_PrintLn("\nAll tests passed!");
    return 0;
}

static uint8_t Test_PowerUp( void )
{
    LL_PowerOn_Set(1);

    // Wait for power to settle before reading
    HAL_Delay( POWER_STARTUP_DELAY );
    LL_Adc_Process();

// Input Rails: failure could indicate test board has failed
    float raw = GetVolts( Get_RAW );
    if( raw < 11.0 ){ Test_Failure("+12v input too low"); goto fail; }
    if( raw > 13.0 ){ Test_Failure("+12v input too high"); goto fail; }

    float rawN = GetVolts( Get_RAWn );
    if( rawN > -11.0 ){ Test_Failure("-12v input too low"); goto fail; }
    if( rawN < -13.0 ){ Test_Failure("-12v input too high"); goto fail; }

// Current Test: under could mean bad rail / missing parts
//               over could mean semi-short, chip backward
//      TODO: this really needs to be diff'd in analog
    float amp = raw - GetVolts( Get_amp_RAW );
    if( amp < 0.0 ){ Test_Failure("under-current +"); goto fail; }
    if( amp > 0.2 ){ Test_Failure("over-current +"); goto fail; }

    amp = rawN - GetVolts( Get_amp_RAWn );
    if( amp < -0.2 ){ Test_Failure("over-current -"); goto fail; }
    if( amp > -0.0 ){ Test_Failure("under-current -"); goto fail; }

// Circuit side rails: over shouldn't happen
//                     under likely means a bridge / chip on backward / bad PTC
    if( GetVolts( Get_12 ) < 11.3 ){ Test_Failure("+12v rail out"); goto fail; }
    if( GetVolts( Get_12n ) > -11.3 ){ Test_Failure("-12v rail out"); goto fail; }

// Check references
    if( GetVolts( Get_5 ) < 4.8 ){ Test_Failure("+5v ref down"); goto fail; }
    if( GetVolts( Get_5n ) > -4.8 ){ Test_Failure("-5v ref down"); goto fail; }

    U_PrintLn("Power system OK");
    return 0;
fail:
    return 1;
}

static void Test_Failure( char* error )
{
    LL_PowerOn_Set(0);
    U_Print("Test Failed: ");
    U_PrintLn(error);
    U_PrintNow();
}

uint8_t Test_V_vec( const TestV_vec_t* test )
{
    const float* expect = &(test->expect0);
    uint8_t i;
    uint8_t error = 0;
    for(i=0; i<8; i++){
        if( *expect >= -5.0 ){
            TestV_t sub_test = { .name = test->name
                               , .dest = test->dest
                               , .volts = 7-i // sweep from negative to positive
                               , .channel = test->channel
                               , .expect = *expect
                               };
            error += Test_V( &sub_test );
        }
        expect++;
    }
    return error;
}

uint8_t Test_V_vec2d( const TestV_vec2d_t* test )
{
    TestV_vec_t sub_t = { .name    = test->name
                        , .dest    = test->dest
                        , .channel = test->channel
                        , .expect0 = test->expect0
                        , .expect1 = test->expect1
                        , .expect2 = test->expect2
                        , .expect3 = test->expect3
                        , .expect4 = test->expect4
                        , .expect5 = test->expect5
                        , .expect6 = test->expect6
                        , .expect7 = test->expect7
                        };
    SelectVoltage( test->dest_static, test->volt_static );
    return Test_V_vec( &sub_t );
}

static uint8_t _Test_inner( const TestV_t* test )
{
    uint8_t error = 0;
    SelectVoltage( test->dest, test->volts );
    HAL_Delay( VOLTAGE_SETTLING_DELAY ); // wait for stabilizing
    LL_Adc_Process(); // read the ADCs
    float result = GetVolts( test->channel );
    float t_shift = _Abs(test->expect) < 1.0 ? 0.1 : 0.0;
    if( test->expect >= 0 ){
        if( result > (test->expect)*(1.0-TOLERANCE)-t_shift
         && result < (test->expect)*(1.0+TOLERANCE)+t_shift ){
            if(VERBOSE){ U_PrintLn("passed"); }
        } else { error = 1; }
    } else { // negative expectation (sign inversion)
        if( result < (test->expect)*(1.0-TOLERANCE)+t_shift
         && result > (test->expect)*(1.0+TOLERANCE)-t_shift ){
            if(VERBOSE){ U_PrintLn("passed"); }
        } else { error = 1; }
    }
    if(error){
        if(!VERBOSE){ // this is supressed above in !VERBOSE mode
            U_Print("testing ");
            U_Print(test->name);
            U_Print(": ");
        }
        U_PrintLn("FAILED");
        U_Print("\texpected "); U_PrintF(test->expect);
        U_Print("\tbut got  "); U_PrintF(result);
    }
    return error;
}

uint8_t Test_V( const TestV_t* test )
{
    if(VERBOSE){
        U_Print("testing ");
        U_Print(test->name);
        U_Print(": ");
    }
    return _Test_inner( test );
}

uint8_t Test_V_V( char* name
                )
{
    //
    return 0;
}

float GetVolts( ADC_t chan )
{
    //TODO: refine these per board (calibration stage?)
    const float volt_scalars[] = {  16.72  //Get_RAW
                                 , -14.59  //Get_RAWn
                                 ,  16.52  //Get_amp_RAW
                                 , -14.61  //Get_amp_RAWn
                                 ,  16.52  //Get_12
                                 , -14.55  //Get_12n
                                 ,  6.58   //Get_5
                                 , -6.62   //Get_5n
                                 ,  6.62   //Get_Left
                                 ,  6.61   //Get_Right
                                 ,  6.62   //Get_Crease
                                 ,  6.60   //Get_Fullwave
                                 ,  6.63   //Get_Integral
                                 ,  6.60   //Get_Follow
                                 ,  6.60   //Get_Peak
                                 ,  6.60   //Get_Trough
                                 };
    return ((float)volt_scalars[chan] * LL_Adc_Get( chan ));
}
