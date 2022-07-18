#include "DataPLC.h"

volatile static uint8_t I[2][8] = {};
volatile static uint8_t Q[2][8] = {};
volatile static uint8_t M[10][2] = {};
volatile static uint8_t T37 = 0;
volatile static uint32_t countT37 = 0;
volatile static uint32_t T37reset = 0;
volatile static uint8_t C1 = 0;
volatile static uint32_t countC1 = 0;
volatile static uint8_t C2 = 0;
volatile static uint32_t countC2 = 0;

void read_Pin_Input()
{
	I0_0 = HAL_GPIO_ReadPin(I0_0_PORT, I0_0_PIN);
}
void write_Pin_Output()
{
	if (Q0_0 >= 1)
	{
		HAL_GPIO_WritePin(Q0_0_PORT, Q0_0_PIN, ENABLE);
	}
	else
	{
		HAL_GPIO_WritePin(Q0_0_PORT, Q0_0_PIN, DISABLE);
	}
	if (Q0_1 >= 1)
	{
		HAL_GPIO_WritePin(Q0_1_PORT, Q0_1_PIN, ENABLE);
	}
	else
	{
		HAL_GPIO_WritePin(Q0_1_PORT, Q0_1_PIN, DISABLE);
	}
	if (Q0_2 >= 1)
	{
		HAL_GPIO_WritePin(Q0_2_PORT, Q0_2_PIN, ENABLE);
	}
	else
	{
		HAL_GPIO_WritePin(Q0_2_PORT, Q0_2_PIN, DISABLE);
	}
}
void Main_task(void *param)
{
	volatile static uint8_t vaoT37 = 0;
	volatile static const uint32_t datT37 = 10;
	volatile static uint8_t checkT37 = 0;
	volatile static uint8_t vaoC1 = 0;
	volatile static uint8_t resetC1 = 0;
	volatile static const uint32_t datC1 = 50;
	volatile static uint8_t startC1 = 0;
	volatile static uint8_t checkC1 = 0;
	volatile static uint8_t vaoC2 = 0;
	volatile static uint8_t resetC2 = 0;
	volatile static const uint32_t datC2 = 2;
	volatile static uint8_t startC2 = 0;
	volatile static uint8_t checkC2 = 0;
	while (1)
	{
		read_Pin_Input();
		/*--------------NetWork 1 -----------*/

		volatile uint8_t tempC2_0 = 0;
		if (countC2 < 2)
		{
			tempC2_0 = 1;
		}
		M0_0 = ((I0_0 + M0_0) * tempC2_0);
		if (M0_0 > 0)
		{
			M0_0 = 1;
		}
		else
		{
			M0_0 = 0;
		}
		/*--------------NetWork 2 -----------*/

		if (!T37reset)
		{
			vaoT37 = (M0_0);
			T37;
			if (vaoT37)
			{
				if (countT37 >= datT37)
					T37 = 1;

				if ((T37 == 0) && (checkT37 == 0))
				{
					xTimerStart(handle_timerPLC[0], portMAX_DELAY);
					checkT37 = 1;
				}
			}
			else
			{
				if (checkT37 == 1)
				{
					xTimerStop(handle_timerPLC[0], portMAX_DELAY);
					checkT37 = 0;
				}
				T37 = 0;
				countT37 = 0;
			}
		}
		else
		{
			xTimerStop(handle_timerPLC[0], portMAX_DELAY);
			countT37 = 0;
			T37 = 0;
			checkT37 = 0;
		}
		/*--------------NetWork 3 -----------*/

		volatile uint8_t T37sl0 = T37;
		volatile static uint8_t checkEU0 = 0;
		if (!(T37))
		{
			checkEU0 = 0;
		}
		if ((!checkEU0) && (T37))
		{
			checkEU0 = 0;
			T37sl0 = 1;
		}
		volatile uint8_t tempC1_1 = 0;
		if (countC1 >= 50)
		{
			tempC1_1 = 1;
		}
		vaoC1 = (T37sl0);
		resetC1 = (tempC1_1 + (!M0_0));
		if (resetC1)
		{
			countC1 = 0;
			C1 = 0;
			startC1 = 1;
		}
		else
		{
			if (vaoC1)
			{
				if (((checkC1 == 1) || (startC1 == 1)) && (countC1 <= 4294967295))
				{
					countC1++;
					startC1 = 0;
				}
				if (countC1 >= datC1)
				{
					C1 = 1;
				}
			}
			else
			{
				checkC1 = 1;
			}
		}
		/*--------------NetWork 4 -----------*/

		volatile uint8_t tempC1_2 = 0;
		if (countC1 <= 20)
		{
			tempC1_2 = 1;
		}
		Q0_0 = (tempC1_2 * M0_0);
		if (Q0_0 > 0)
		{
			Q0_0 = 1;
		}
		else
		{
			Q0_0 = 0;
		}
		/*--------------NetWork 5 -----------*/

		volatile uint8_t tempC1_3 = 0;
		if (countC1 > 20)
		{
			tempC1_3 = 1;
		}
		volatile uint8_t tempC1_4 = 0;
		if (countC1 <= 25)
		{
			tempC1_4 = 1;
		}
		Q0_2 = (tempC1_3 * tempC1_4 * M0_0);
		if (Q0_2 > 0)
		{
			Q0_2 = 1;
		}
		else
		{
			Q0_2 = 0;
		}
		/*--------------NetWork 6 -----------*/

		volatile uint8_t tempC1_5 = 0;
		if (countC1 > 25)
		{
			tempC1_5 = 1;
		}
		Q0_1 = (tempC1_5 * M0_0);
		if (Q0_1 > 0)
		{
			Q0_1 = 1;
		}
		else
		{
			Q0_1 = 0;
		}
		/*--------------NetWork 7 -----------*/

		T37reset = (T37);
		if (T37reset > 0)
		{
			T37reset = 1;
		}
		else
		{
			T37reset = 0;
		}
		/*--------------NetWork 8 -----------*/

		vaoC2 = (I0_0);
		resetC2 = (C2);
		if (resetC2)
		{
			countC2 = 0;
			C2 = 0;
			startC2 = 1;
		}
		else
		{
			if (vaoC2)
			{
				if (((checkC2 == 1) || (startC2 == 1)) && (countC2 <= 4294967295))
				{
					countC2++;
					startC2 = 0;
				}
				if (countC2 >= datC2)
				{
					C2 = 1;
				}
			}
			else
			{
				checkC2 = 1;
			}
		}
		write_Pin_Output();
	}
}
void TimerCallBack(TimerHandle_t xTimer)
{
	int id;
	id = (uint32_t)pvTimerGetTimerID(xTimer);
	switch (id)
	{
	case 1:
		countT37++;
		break;
	}
}
void initTimer(void)
{
	handle_timerPLC[0] = xTimerCreate("timerTONT37", pdMS_TO_TICKS(100), pdTRUE, (void *)(0 + 1), TimerCallBack);
}
