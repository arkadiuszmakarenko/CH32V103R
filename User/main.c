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
#include <usb_gamepad.h>
#include <usb_mouse.h>
#include <usb_keyboard.h>
#include "usb_host_config.h"
#include "utils.h"
#include "tim.h"
#include "mouse.h"
#include "gpio.h"
#include "keyboard.h"
#include "gamepad.h"

/*********************************************************************
 * @fn      main
 *
 * @brief   Main function.
 *
 * @return  none
 */
int main(void) {
	/* Initialize system configuration */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    /* Initialize timer for obtaining keyboard and mouse data at regular intervals */
    TIM3_Init(9, SystemCoreClock / 10000 - 1);

    /* Configure USB clock and initialize USB host */
#if DEF_USBHD_PORT_EN
    USBHD_RCC_Init();
    USBHD_Host_Init(ENABLE);
    memset(&RootHubDev.bStatus, 0, sizeof(ROOT_HUB_DEVICE));
    memset(
            &HostCtl[ DEF_USBHD_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL].InterfaceNum,
            0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof(HOST_CTL));
#endif

    TIM2_Init();
    TIM4_Init();
    Delay_Init();
    GPIO_Config();
    InitMouse();


    while (1) {
            USBH_MainDeal();

            //Handle HID Device
            if (RootHubDev.bType == USB_DEV_CLASS_HID) {

                for (int itf = 0; itf < DEF_INTERFACE_NUM_MAX; itf++) {
                    //Handle mouse
                    if (HostCtl[0].Interface[itf].HIDRptDesc.type
                            == REPORT_TYPE_MOUSE) {
                        HID_MOUSE_Info_TypeDef *mousemap = USBH_GetMouseInfo(
                                &HostCtl[0].Interface[itf]);
                        ProcessMouse(mousemap);
                    }

                    //Handle gamepad
                    if (HostCtl[0].Interface[itf].HIDRptDesc.type
                            == REPORT_TYPE_JOYSTICK) {

            HID_gamepad_Info_TypeDef *gamepad = GetGamepadInfo(
                                &HostCtl[0].Interface[itf]);
                            ProcessGamepad(gamepad);
                    }

                    // Handle Keyboard
                    if (HostCtl[0].Interface[itf].HIDRptDesc.type
                            == REPORT_TYPE_KEYBOARD) {
                        //HID_KEYBD_Info_TypeDef *USBH_HID_GetKeybdInfo(Interface *Itf)
                        HID_KEYBD_Info_TypeDef *kbd = USBH_HID_GetKeybdInfo(
                                &HostCtl[0].Interface[itf]);

                        amikb_process(kbd);

                    }

                }
            }

            //Handle HUB Device

            if (RootHubDev.bType == USB_DEV_CLASS_HUB) {

                //Iterate over all devices
                for (uint8_t device = 1; device < 5; device++)
                {
                    //Iterate over all interfaces
                    for (int itf = 0; itf < DEF_INTERFACE_NUM_MAX; itf++) {
                        //Handle mouse
                        if (HostCtl[device].Interface[itf].HIDRptDesc.type
                                == REPORT_TYPE_MOUSE) {
                            HID_MOUSE_Info_TypeDef *mousemap = USBH_GetMouseInfo(
                                    &HostCtl[device].Interface[itf]);
                                ProcessMouse(mousemap);

                        }

                        //Handle gamepad
                        if (HostCtl[device].Interface[itf].HIDRptDesc.type
                                == REPORT_TYPE_JOYSTICK) {

                HID_gamepad_Info_TypeDef *gamepad = GetGamepadInfo(
                                    &HostCtl[device].Interface[itf]);
                                ProcessGamepad(gamepad);
                        }

                        // Handle Keyboard
                        if (HostCtl[device].Interface[itf].HIDRptDesc.type
                                == REPORT_TYPE_KEYBOARD) {
                            HID_KEYBD_Info_TypeDef *kbd = USBH_HID_GetKeybdInfo(
                                    &HostCtl[device].Interface[itf]);
                                amikb_process(kbd);

                        }

                    }
                }



                }

            }

        }





