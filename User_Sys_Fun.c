/*******************************************************************
	@file		User_Sys_Fun.c
	@author 	Jiahao.lin
	@date 		2022.05.30
	@version	V1.0.1
	@brief	   
**********************************************************************/
#include "User_Base.h"


unsigned char CycleExitFlag;
/***************************************************************
函数原型：void System_Off_Process(void)
功能描述：
输入参数：无
输出参数：无
说    明：关机
***************************************************************/
void System_Off_Process(void)
{
	CycleExitFlag = false;
	Sys_Config_Init();
	User_Enter_Sleep();
	User_Timer2_Init();
	Delay_ms(5);
	EnWDT();
	RB2 = 1;
	PowerKeyUpCnt = 0;
	PowerKeyDownCnt = 0;
	MainExitFlag = false;
	while(!MainExitFlag)
	{
		asm("clrwdt");
		PowerKeyDetect();
	}
}

/***************************************************************
函数原型：void System_On_Process(void)
功能描述：
输入参数：无
输出参数：无
说    明：工作进程
***************************************************************/
void System_On_Process(void)
{
	Sys_Config_Init();
	User_Init_Var();
	User_Timer0_Init();
	User_Timer2_Init();
	User_ADC_Init();
	User_PWM_Init();
	SetHotTempPara();
	SetMotorPara();
	RB1 = 1;
	EnWDT();
	MainExitFlag = false;
	while(!MainExitFlag)
	{	
		asm("clrwdt");
		if(ScanKb()){ 
			switch(CurKbCode){
				case PowerKeyCode:
					PowerKeyProcess();  // 开关机
					break;                                                    	
				default:
					break;
			}
		}
		User_Get_TempAdc();
		User_Play_Temp();
		User_Sys_AutoOff();
		User_Laohua_Mode();
		User_Shake_Motor();
		User_Have_ModeShake();
		User_Shake_Work();
	}
}

/********************************* (C) COPYRIGHT IPOLARSTAR *************************************/