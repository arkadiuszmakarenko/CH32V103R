#ifndef __MOUSE_H
#define __MOUSE_H

#include "stdint.h"
#include "usb_mouse.h"
#include "utils.h"




#define MOUSEX	0
#define MOUSEY	1
#define Q_RATELIMIT 500
#define Q_BUFFERLIMIT 300
#define DPI_DIVIDER 2



void ProcessMouse(HID_MOUSE_Info_TypeDef *mousemap);
void ProcessX_IRQ();
void ProcessY_IRQ();

#endif
