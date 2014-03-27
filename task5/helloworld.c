#include <linux/usb.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hid.h>

static int hello_probe(struct usb_interface *interface,
		const struct usb_device_id *id)
{
	pr_alert("USB keyboard plugged in\n");
	return 0;
}

static void hello_disconnect(struct usb_interface *interface)
{
	pr_alert("USB keyboard disconnected.\n");
}

static const struct usb_device_id id_table[] = {
	{USB_INTERFACE_INFO
	 (USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
	  USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{},
};

MODULE_DEVICE_TABLE(usb, id_table);

static struct usb_driver hello_driver = {
	.name = "hellousb",
	.probe = hello_probe,
	.disconnect = hello_disconnect,
	.id_table = id_table,
};

static int __init hello_init(void)
{
	pr_debug("Hello World!\n");
	return usb_register(&hello_driver);
}

static void __exit hello_exit(void)
{
	pr_debug("See you later.\n");
	usb_deregister(&hello_driver);
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("7c1caf2f50d1");
MODULE_DESCRIPTION("Just a module");
