#include "mouse.h"
#include "utils.h"

HID_MOUSE_Info_TypeDef    	mouse_info;
uint8_t                 	mouse_report_data[8];


HID_MOUSE_Info_TypeDef *USBH_GetMouseInfo(Interface *Itf)
{
  if (USBH_MouseDecode(Itf) == USBH_OK)
  {
	  DUG_PRINTF("x:%x \r\n", mouse_info.x);
	  DUG_PRINTF("y:%x \r\n", mouse_info.y);
	  DUG_PRINTF("btn0:%x \r\n", mouse_info.buttons[0]);
	  DUG_PRINTF("btn1:%x \r\n", mouse_info.buttons[1]);
	  DUG_PRINTF("btn2:%x \r\n", mouse_info.buttons[2]);
    return &mouse_info;
  }
  else
  {
    return NULL;
  }
}




USBH_StatusTypeDef USBH_MouseDecode(Interface *Itf)
{

  if (Itf->HidRptLen == 0U)
  {
    return USBH_FAIL;
  }

  //Clear mouse_report_data

  memset(&mouse_report_data,0,sizeof(mouse_report_data));


  /*Fill report */
  if (FifoRead(&Itf->buffer, &mouse_report_data, Itf->HidRptLen) !=0)
  {

	  uint8_t btn = 0;
	  uint8_t btn_extra = 0;
	  int16_t a[2];
	  uint8_t i;



	  // skip report id if present
	  uint8_t *p = mouse_report_data + (Itf->HIDRptDesc.report_id?1:0);


	  //process axis
	  // two axes ...
	  		for(i=0;i<2;i++) {
	  			// if logical minimum is > logical maximum then logical minimum
	  			// is signed. This means that the value itself is also signed
	  			int is_signed = Itf->HIDRptDesc.joystick_mouse.axis[i].logical.min >
	  				Itf->HIDRptDesc.joystick_mouse.axis[i].logical.max;
	  			a[i] = collect_bits(p, Itf->HIDRptDesc.joystick_mouse.axis[i].offset,
	  					Itf->HIDRptDesc.joystick_mouse.axis[i].size, is_signed);
	  		}

	  //process 4 first buttons
	  for(i=0;i<4;i++)
	  	if(p[Itf->HIDRptDesc.joystick_mouse.button[i].byte_offset] &
	  			Itf->HIDRptDesc.joystick_mouse.button[i].bitmask) btn |= (1<<i);

	  // ... and the eight extra buttons
	  for(i=4;i<12;i++)
	  	if(p[Itf->HIDRptDesc.joystick_mouse.button[i].byte_offset] &
	  			Itf->HIDRptDesc.joystick_mouse.button[i].bitmask) btn_extra |= (1<<(i-4));

	  //process mouse
	  if(Itf->HIDRptDesc.type == REPORT_TYPE_MOUSE) {
	  		// iprintf("mouse %d %d %x\n", (int16_t)a[0], (int16_t)a[1], btn);
	  		// limit mouse movement to +/- 128
	  		for(i=0;i<2;i++) {
	  		if((int16_t)a[i] >  127) a[i] =  127;
	  		if((int16_t)a[i] < -128) a[i] = -128;
	  		}
	  		//btn
	  	  mouse_info.x = a[0];
	  	  mouse_info.y = a[1];
	  	  mouse_info.buttons[0] = btn&0x1;
	  	  mouse_info.buttons[1] = (btn>>1)&0x1;
	  	  mouse_info.buttons[2] = (btn>>2)&0x1;
	  	}
    return USBH_OK;
  }
  return   USBH_FAIL;
}
