#include "../../include/driverInc.h"
#define FLOAT_VALUE_ADD(v,max,adder)  v=((v) > ( (max) - (adder)  ) ? max : ((v) + (adder)) )
#define FLOAT_VALUE_SUB(v,min,subber) v=((v) < ( (min) + (subber) ) ? min : ((v) - (subber)))
void processNormalKeyCase_GammaCorrect(unsigned char key,gammaCorrectParam * gammaParam_p)
{
    switch(key)
    {
        case 'a': FLOAT_VALUE_ADD(gammaParam_p->GammaCorrect_power_level_inv,5.0,0.1);LOG_KEYBOARD_RESPONSE("add gamma, GammaCorrect_power_level_inv=%f",gammaParam_p->GammaCorrect_power_level_inv);break;
        case 's': FLOAT_VALUE_SUB(gammaParam_p->GammaCorrect_power_level_inv,0.1,0.1);LOG_KEYBOARD_RESPONSE("sub gamma, GammaCorrect_power_level_inv=%f",gammaParam_p->GammaCorrect_power_level_inv);break;
        case 'A': FLOAT_VALUE_ADD(gammaParam_p->GammaCorrect_power_level_inv,5.0,0.1);LOG_KEYBOARD_RESPONSE("add gamma, GammaCorrect_power_level_inv=%f",gammaParam_p->GammaCorrect_power_level_inv);break;
        case 'S': FLOAT_VALUE_SUB(gammaParam_p->GammaCorrect_power_level_inv,0.1,0.1);LOG_KEYBOARD_RESPONSE("sub gamma, GammaCorrect_power_level_inv=%f",gammaParam_p->GammaCorrect_power_level_inv);break;
        default:  break;
    }
}
void processNormalKeyCase_GaussSmooth(unsigned char key,gaussianSmoothParam * gaussParam_p)
{
    switch(key)
    {
        case 'a': FLOAT_VALUE_ADD(gaussParam_p->GaussianSmooth_Kernel,21,2 ) ;LOG_KEYBOARD_RESPONSE("add Gauss, GaussianSmooth_Kernel=%d",gaussParam_p->GaussianSmooth_Kernel);break;
        case 's': FLOAT_VALUE_SUB(gaussParam_p->GaussianSmooth_Kernel,3,2)   ;LOG_KEYBOARD_RESPONSE("sub Gauss, GaussianSmooth_Kernel=%d",gaussParam_p->GaussianSmooth_Kernel);break;
        case 'q': FLOAT_VALUE_ADD(gaussParam_p->GaussianSmooth_Sigma,10,0.1) ;LOG_KEYBOARD_RESPONSE("add Gauss, GaussianSmooth_Sigma =%F",gaussParam_p->GaussianSmooth_Sigma );break;
        case 'w': FLOAT_VALUE_SUB(gaussParam_p->GaussianSmooth_Sigma,0.1,.1) ;LOG_KEYBOARD_RESPONSE("sub Gauss, GaussianSmooth_Sigma =%F",gaussParam_p->GaussianSmooth_Sigma );break;
        default:  break;
    }
}