#ifndef SOFT_H
#define SOFT_H

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "adc.h"
#include "comp.h"
#include "dac.h"
#include "hrtim.h"
#include "tim.h"
#include "gpio.h"

/*defines*/
#define I_zad 6000 // 6000
#define I_zad_min 1000 // 1000
#define I_max_dop 13000 // 13000
#define I_max 5200 // 5200
#define U_min 900 //1950
#define U_max 2500 //1950
#define I_min 1000 // 1000
#define PWM_MAX 23040 //23040
#define PWM_MIN 48 //48
#define PWM_PERIOD 57600 // 57600
#define errorSatLimit 23040 //23040
#define Pkf 1 // 1
#define Ikf 1 // 1
#define antiStickCountMax 32000 // 32000
#define curZadP 400 // 40
#define curZadM 20 // 20
#define inWorkRef 900000 // 900000
#define deltaPWM 10 // 10
#define errorSumLimit 100 // 100
#define IZadVarDelta 3000 // 3000
#define nowErrorKoefDiv 50 // 400
#define COMPARE_A2 28800 // 28800
#define dacSetValue 650 // 3333
#define inErrorMax 5 // 5
#define errPauseMax 5550 // 5550
#define deltaEnc 3 // 3
#define minCurrentCode 100 // 100
#define minVoltageAntiStick 1000 // 1000
#define encVarMin 3 // 3
#define encVarMax 33 // 33
#define minVoltageCode 32 // 32
#define deltaPulseMAX 500 // 500
#define countSurgeZadMin 15200 // 20000
#define countSurgeZadMax 800 // 20000
/********/

/**********/
extern uint32_t cnti;
extern uint32_t inWork;
extern uint16_t ADC_Data[2];
extern uint16_t uOut;
extern uint16_t iOut;
extern int32_t pulse;
extern int32_t pulseLast, deltaPulse;
extern uint16_t inError, Err_pause;
extern uint16_t Er_sost;
extern uint8_t startOS;
extern int16_t Pr, Ir, Dr, IUr;
extern uint8_t antiStick;
extern int32_t cmpA1, cmpA2, cmpA3, cmpA4;
extern int32_t lastError, nowError, errorSum, errorRate;
extern int16_t currentZad;
extern int16_t antiStickCount, antiStickCount2;
extern uint8_t encVarNow, encVarLast;
extern uint16_t IZadVar;
extern uint32_t Um, countSurge, countSurgeMin, countSurgeMax;

/**********/
extern void uOutCalc(void);
extern void iOutCalc(void);
extern void inInit(void);
extern void inMain(void);
extern void inTimH(void);
extern void inTimHR(void);
extern void inSysT(void);
extern void inSysTConfig(void);
extern void checkPulse(void);
extern void inDac(void);
extern void inComp(void);
extern void inTimH_impulse(void);
/**********/

/**********/

/**********/



/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif /* End of SOFT_H */