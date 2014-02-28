/*
	file added by ZMorph
*/

#ifndef USB_MESSAGESTREAM_H
#define USB_MESSAGESTREAM_H

#include "USBHID.h"

#include "Module.h"
#include "StreamOutput.h"

#define USB_OUTPUT_MESSAGE_LENGTH 64
#define USB_INPUT_MESSAGE_LENGTH 64
#define USB_VENDOR_ID 0x1234
#define USB_PRODUCT_ID 0x0006
#define USB_PRODUCT_RELEASE 0x0001


class USBMessageStream : public USBHID, public Module, public StreamOutput
{
public:
	USBMessageStream();

    int puts(const char *);
	
    void on_module_loaded(void);
    void on_main_loop(void *);
};

#endif
