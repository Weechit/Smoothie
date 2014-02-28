/*
	file added by ZMorph
*/

#include "USBMessageStream.h"

#include "libs/Kernel.h"
#include "libs/SerialMessage.h"

USBMessageStream::USBMessageStream() :
	USBHID(
		USB_OUTPUT_MESSAGE_LENGTH, 
		USB_INPUT_MESSAGE_LENGTH, 
		USB_VENDOR_ID,
		USB_PRODUCT_ID,
		USB_PRODUCT_RELEASE,
		true) {}

void USBMessageStream::on_module_loaded()
{
    this->register_for_event(ON_MAIN_LOOP);
}

void USBMessageStream::on_main_loop(void *argument)
{
	// execute all pending messages
	HID_REPORT recv_report;
	while (readNB(&recv_report)) 
	{
        struct SerialMessage message;
        // TODO the following conversion is dangerous - unsigned to signed
        message.message = string((const char *)recv_report.data);
        message.stream = this;
        THEKERNEL->call_event(ON_CONSOLE_LINE_RECEIVED, &message);
	}
	puts("sabaka\n");
}

int USBMessageStream::puts(const char *str)
{
	// TODO check for strings longer then 64 bytes

	int length = strlen(str);

	HID_REPORT send_report;
	send_report.length = length;
	memcpy(send_report.data, str, length);

	// TODO consider if it should be blocking or non-blocking
	send(&send_report);
	return length;
}