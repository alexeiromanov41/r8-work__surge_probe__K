#include "soft.h"

/**********/
uint32_t cnti;
uint32_t inWork;
uint16_t ADC_Data[2];
uint16_t uOut;
uint16_t iOut;
int32_t pulse;
int32_t pulseLast, deltaPulse;
uint16_t inError, Err_pause;
uint16_t Er_sost;
uint8_t startOS;
int16_t Pr, Ir, Dr, IUr;
uint8_t antiStick;
int32_t cmpA1, cmpA2, cmpA3, cmpA4;
int32_t lastError, nowError, errorSum, errorRate; 
int16_t currentZad;
int16_t antiStickCount, antiStickCount2;
uint8_t encVarNow, encVarLast;
uint16_t IZadVar;
uint32_t Um, countSurge, countSurgeMin, countSurgeMax;
/**********/

/**********/
void uOutCalc(void);
void iOutCalc(void);
void inInit(void);
void inMain(void);
void inTimH(void);
void inTimHR(void);
void inSysT(void);
void inSysTConfig(void);
void checkPulse(void);
void inDac(void);
void inComp(void);
void inTimH_impulse(void);
/**********/

//Prototypes
void uOutCalc(void) {
	ADC_Data[0] = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
  if (ADC_Data[0] > 32)
  {
    uOut = ( ADC_Data[0] * 24 ) / 10;
  } else {
    uOut = 0;
  }
}

void iOutCalc(void) {
	ADC_Data[1] = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
  if (ADC_Data[1] > minCurrentCode)
  {
    iOut = ( ADC_Data[1] * 47 ) / 10;
  } else {
    iOut = 0;
  }
}

void inInit(void) {
	for(cnti = 0; cnti <= 9000000; cnti++){}
	cnti = 0;	
	HAL_GPIO_WritePin(soft_start_GPIO_Port, soft_start_Pin, GPIO_PIN_SET);	
	startOS = 1;
	antiStick = 0;
	antiStickCount = 0;
	antiStickCount2 = 0;
	IZadVar = I_zad;
	currentZad = IZadVar;
	Um = U_min;
	countSurge = 0;
	countSurgeMin = 0;	
	countSurgeMax = 0;

	__HAL_TIM_SET_COUNTER(&htim2, I_zad / (deltaEnc * 100));
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);		

  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
  HAL_ADCEx_InjectedStart(&hadc1);

	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2);
	HAL_HRTIM_WaveformCountStart_IT(&hhrtim1, HRTIM_TIMERID_TIMER_A);

	HAL_GPIO_WritePin(test_led_GPIO_Port, test_led_Pin, GPIO_PIN_SET);
}

void inMain(void) {
	encVarNow = __HAL_TIM_GET_COUNTER(&htim2);
	if(encVarNow < encVarMin) {
		encVarNow = encVarMin;
		__HAL_TIM_SET_COUNTER(&htim2, encVarNow);
	}
	if(encVarNow > encVarMax) {
		encVarNow = encVarMax;
		__HAL_TIM_SET_COUNTER(&htim2, encVarNow);
	}
	IZadVar = encVarNow * deltaEnc * 100;
}

void inTimH(void) {
	iOutCalc();
	uOutCalc();
	
	if ( (startOS == 1) && (inWork < inWorkRef) ) {
		
			if(antiStick == 0) {
				if ( (pulse > PWM_MIN + deltaPWM) && (uOut <= U_min) ) {
					currentZad += curZadP;
				}
				else {
					currentZad -= curZadM;
				}
			}
			else if (antiStick == 1) {
				currentZad = I_min;
				if(errorSum > errorSumLimit) {
					errorSum = errorSumLimit;
				}
				if(errorSum < -errorSumLimit) {
					errorSum = -errorSumLimit;
				}
				
				if(uOut >= U_min) {
					antiStickCount2 ++;
					if(antiStickCount2 > antiStickCountMax) {
						antiStickCount2 = 0;
						antiStickCount = 0;
						antiStick = 0;
						currentZad = IZadVar;
					}
				}
			}

			if((iOut >= IZadVar + IZadVarDelta) && (uOut < minVoltageAntiStick)) {
				antiStickCount ++;
				if(antiStickCount > antiStickCountMax) {
					antiStickCount = antiStickCountMax;
					antiStick = 1;
					currentZad = I_min;
				}
			} 
			else {
				antiStickCount -= 1;
				if(antiStickCount < -antiStickCountMax) {
					antiStickCount = -antiStickCountMax;
				}
			}
		
		nowError = currentZad - iOut;
		errorSum += nowError / nowErrorKoefDiv;
		
		if(errorSum > errorSatLimit) {
			errorSum = errorSatLimit;
		}
		if(errorSum < -errorSatLimit) {
			errorSum = -errorSatLimit;
		}			

		pulse = nowError + errorSum;			

	} 
	else {
		pulse = PWM_MIN;
	} 

	if(currentZad > I_max_dop) {
		currentZad = I_max_dop;
	}
	if(currentZad < IZadVar) {
		if(antiStick == 1) {
			currentZad = I_min;
		}
		else {
			currentZad = IZadVar;
		}
		
	}

	deltaPulse = pulse - pulseLast;
	if (deltaPulse > deltaPulseMAX)	{
		pulse = deltaPulseMAX + pulseLast;
	}

	checkPulse();

	pulseLast = pulse;		
		
}

void inTimHR(void) {
	iOutCalc();
	uOutCalc();
	
	if ( (startOS == 1) && (inWork < inWorkRef) ) {
		
			if(antiStick == 0) {
				if ( (pulse > PWM_MIN + deltaPWM) && (uOut <= U_min) ) {
					currentZad += curZadP;
				}
				else {
					currentZad -= curZadM;
				}
			}
			else if (antiStick == 1) {
				currentZad = I_min;
				if(errorSum > errorSumLimit) {
					errorSum = errorSumLimit;
				}
				if(errorSum < -errorSumLimit) {
					errorSum = -errorSumLimit;
				}
				
				if(uOut >= U_min) {
					antiStickCount2 ++;
					if(antiStickCount2 > antiStickCountMax) {
						antiStickCount2 = 0;
						antiStickCount = 0;
						antiStick = 0;
						currentZad = IZadVar;
					}
				}
			}

			if((iOut >= IZadVar + IZadVarDelta) && (uOut < minVoltageAntiStick)) {
				antiStickCount ++;
				if(antiStickCount > antiStickCountMax) {
					antiStickCount = antiStickCountMax;
					antiStick = 1;
					currentZad = I_min;
				}
			} 
			else {
				antiStickCount -= 1;
				if(antiStickCount < -antiStickCountMax) {
					antiStickCount = -antiStickCountMax;
				}
			}
		
		nowError = currentZad - iOut;
		errorSum += nowError / nowErrorKoefDiv;
		
		if(errorSum > errorSatLimit) {
			errorSum = errorSatLimit;
		}
		if(errorSum < -errorSatLimit) {
			errorSum = -errorSatLimit;
		}			

		pulse += ((nowError - lastError) / 2) + (nowError / 400);			

	} 
	else {
		pulse = PWM_MIN;
	} 

	if(currentZad > I_max_dop) {
		currentZad = I_max_dop;
	}
	if(currentZad < IZadVar) {
		if(antiStick == 1) {
			currentZad = I_min;
		}
		else {
			currentZad = IZadVar;
		}
		
	}

	deltaPulse = pulse - pulseLast;
	if (deltaPulse > deltaPulseMAX)	{
		pulse = deltaPulseMAX + pulseLast;
	}

	checkPulse();

	pulseLast = pulse;
	lastError = nowError;		
		
}

void inSysT(void) {
	inWork ++;
	if (inWork >= inWorkRef) {
		inWork = inWorkRef;
		startOS = 0;
		currentZad = IZadVar;
		HAL_GPIO_WritePin(test_led_GPIO_Port, test_led_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(soft_start_GPIO_Port, soft_start_Pin, GPIO_PIN_RESET);
		pulse = PWM_MIN;
		checkPulse();
		HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2);
		HAL_HRTIM_WaveformCountStop_IT(&hhrtim1, HRTIM_TIMERID_TIMER_A);
	}
	
	if (inWork < inWorkRef) {
		Er_sost = HAL_GPIO_ReadPin(driver_error_GPIO_Port, driver_error_Pin);
		if (startOS == 1) {
			if (Er_sost == 0) {
			 inError ++;
			} else {
				inError = 0;
			}
		}
		
		if ( (inError >= inErrorMax) && (Err_pause == 0)) {
			inError = inErrorMax;
			Err_pause = 1;
			pulse = PWM_MIN;
			startOS = 0;
			currentZad = IZadVar;
			checkPulse();
			HAL_GPIO_WritePin(test_led_GPIO_Port, test_led_Pin, GPIO_PIN_RESET);
		}
		
		if (Err_pause > 0) {
			Err_pause ++;
		} else {
				Err_pause = 0;
		}
		
		if (Err_pause >= errPauseMax) {
			Err_pause = 0;
			startOS = 1;
			pulse = PWM_MIN;
			checkPulse();
			HAL_GPIO_WritePin(test_led_GPIO_Port, test_led_Pin, GPIO_PIN_SET);
		}
	}	
}

void inSysTConfig(void) {
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 10000);
	/**Configure the Systick 
	 */
	HAL_SYSTICK_CLKSourceConfig (SYSTICK_CLKSOURCE_HCLK);
	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);	
}

void checkPulse(void) {
	if(pulse < PWM_MIN) {
		pulse = PWM_MIN;
	} else if(pulse > PWM_MAX) {
			pulse = PWM_MAX;
		}
	
	cmpA1 = pulse;
	cmpA2 = COMPARE_A2;
	cmpA3 = cmpA2 + pulse;
	cmpA4 = cmpA1 / 2;

	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, cmpA1);
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_2, cmpA2);
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_3, cmpA3);
	__HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, cmpA4);		
}

void inDac(void) {
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dacSetValue);
}

void inComp(void) {
	HAL_COMP_Start(&hcomp2);
}

void inTimH_impulse(void) {
	iOutCalc();
	uOutCalc();
	
	if(antiStick == 0) {
		if((countSurgeMin < countSurgeZadMin) && (countSurgeMax == 0)) {
			countSurgeMin ++;
			if(countSurgeMin >= countSurgeZadMin) {
				if(Um == U_min) {
					Um = U_max;
					currentZad = I_max_dop;
					countSurgeMin = countSurgeZadMin;
				} 
			}
		}
		if((countSurgeMax < countSurgeZadMax) && (countSurgeMin == countSurgeZadMin)) {
			countSurgeMax ++;
			if(countSurgeMax >= countSurgeZadMax) {
				if(Um == U_max) {
					Um = U_min;
					countSurgeMax = 0;
					countSurgeMin = 0;
				} 
			}
		}
	}
	
	if ( (startOS == 1) && (inWork < inWorkRef) ) {
		
			if(antiStick == 0) {
				if ( (pulse > PWM_MIN + deltaPWM) && (uOut <= Um) ) {
					currentZad += curZadP;
				}
				else {
					currentZad -= curZadM;
				}
			}
			else if (antiStick == 1) {
				currentZad = I_min;
				if(errorSum > errorSumLimit) {
					errorSum = errorSumLimit;
				}
				if(errorSum < -errorSumLimit) {
					errorSum = -errorSumLimit;
				}
				
				if(uOut >= U_min) {
					antiStickCount2 ++;
					if(antiStickCount2 > antiStickCountMax) {
						antiStickCount2 = 0;
						antiStickCount = 0;
						antiStick = 0;
						currentZad = IZadVar;
					}
				}
			}

		if(Um == U_min) {
			if((iOut >= IZadVar + IZadVarDelta) && (uOut < minVoltageAntiStick)) {
				antiStickCount ++;
				if(antiStickCount > antiStickCountMax) {
					antiStickCount = antiStickCountMax;
					antiStick = 1;
					currentZad = I_min;
				}
			} 
			else {
				antiStickCount -= 1;
				if(antiStickCount < -antiStickCountMax) {
					antiStickCount = -antiStickCountMax;
				}
			}
		}
		
		nowError = currentZad - iOut;
		errorSum += nowError / nowErrorKoefDiv;
		
		if(errorSum > errorSatLimit) {
			errorSum = errorSatLimit;
		}
		if(errorSum < -errorSatLimit) {
			errorSum = -errorSatLimit;
		}			

		pulse = nowError + errorSum;			

	} 
	else {
		pulse = PWM_MIN;
	} 

	if(currentZad > I_max_dop) {
		currentZad = I_max_dop;
	}
	if(currentZad < IZadVar) {
		if(antiStick == 1) {
			currentZad = I_min;
		}
		else {
			currentZad = IZadVar;
		}
		
	}

	deltaPulse = pulse - pulseLast;
	if (deltaPulse > deltaPulseMAX)	{
		pulse = deltaPulseMAX + pulseLast;
	}

	checkPulse();

	pulseLast = pulse;		
}