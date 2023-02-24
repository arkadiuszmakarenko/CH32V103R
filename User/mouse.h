#ifndef __MOUSE_H
#define __MOUSE_H

#include <stdint.h>
#include "usb_host_config.h"

typedef struct _HID_MOUSE_Info
{
  int16_t              x;
  int16_t              y;
  int8_t              buttons[3];
}
HID_MOUSE_Info_TypeDef;

HID_MOUSE_Info_TypeDef *USBH_GetMouseInfo(Interface *Itf);
USBH_StatusTypeDef USBH_MouseDecode(Interface *Itf);


#endif /*__MOUSE_H */

