/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2020/04/30
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 * @Note
 * This example demonstrates the process of enumerating the keyboard and mouse
 * by a USB host and obtaining data based on the polling time of the input endpoints
 * of the keyboard and mouse.
 * The USBFS port also supports enumeration of keyboard and mouse attached at tier
 * level 2(Hub 1).
*/

/*
 * @Note
 * The USBHD module uses the system clock as the clock source, so the SystemCoreClock can
 * only be set to 72MHz or 48MHz.
 */

/*******************************************************************************/
/* Header File */
#include "usb_host_config.h"
#include "utils.h"
#include "tim.h"
#include "mouse.h"



/*********************************************************************
 * @fn      main
 *
 * @brief   Main function.
 *
 * @return  none
 */
int main( void )
{
    /* Initialize system configuration */
    Delay_Init( );
#if DEF_DEBUG_PRINTF
    USART_Printf_Init( 115200 );
    DUG_PRINTF( "SystemClk:%d\r\n", SystemCoreClock );
    DUG_PRINTF( "USBHD HOST KM Test\r\n" );
#endif

    /* Initialize timer for obtaining keyboard and mouse data at regular intervals */
    TIM3_Init( 9, SystemCoreClock / 10000 - 1 );
    DUG_PRINTF( "TIM3 Init OK!\r\n" );

    /* Configure USB clock and initialize USB host */
#if DEF_USBHD_PORT_EN
    USBHD_RCC_Init( );
    USBHD_Host_Init( ENABLE );
    memset( &RootHubDev.bStatus, 0, sizeof( ROOT_HUB_DEVICE ) );
    memset( &HostCtl[ DEF_USBHD_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL ].InterfaceNum, 0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof( HOST_CTL ) );
#endif
    
    TIM2_Init( );
    Delay_Init( );
#if DEF_DEBUG_PRINTF
    DUG_PRINTF( "TIM2 Init OK!\r\n" );
#endif
    TIM4_Init( );
    Delay_Init( );
#if DEF_DEBUG_PRINTF
    DUG_PRINTF( "TIM4 Init OK!\r\n" );
#endif


    while( 1 )
    {
        USBH_MainDeal( );

        for (int itf = 0; itf<4 ; itf++)
        {
        	if (HostCtl[0].Interface[itf].Type == DEC_MOUSE)
        	{
        		HID_MOUSE_Info_TypeDef *mousedata = 	USBH_GetMouseInfo(&HostCtl[0].Interface[itf]);
        		if (mousedata!=NULL)
        		{
        			DUG_PRINTF( "TIM4 Init OK!\r\n" );

        		}
        	}
        }






    }
}

