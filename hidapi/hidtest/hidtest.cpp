/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include "hidapi.h"
#include "packdef.h"
#include "ghostapi.h"
// Headers needed for sleeping.
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif





#define GHOST_VID 0x2341
#define GHOST_PID 0x8036


int main(int argc, char* argv[])
{
#if 1
	OpenDevice();
	//Sleep(5000);
	//KeyPress("s", 1);
	SetLogLevel(5);
	Sleep(5000);
	//Disconnect(10);
	//MoveTo(0, 0);
	//Sleep(1000);
	//POINT pt;
	//GetCursorPos(&pt); //获取鼠标指针位置到pt
	//printf("px=%ld,py=%ld\n", pt.x, pt.y);
	//MoveTo(800, 600);
	KeyPress("a", 10);
	//MoveToR(255, 255);
	//printf("sn:%s\n",GetSN());
	//printf("ml:%s\n", GetModel());
	//printf("ver:%s\n", GetVer());
	//printf("pd:%s\n", GetProductionDate());
	//GetCursorPos(&pt); //获取鼠标指针位置到pt
	//printf("x=%ld,y=%ld\n", pt.x, pt.y);
	//Sleep(3000);
	//MoveToR(-255, -255);
	//GetCursorPos(&pt); //获取鼠标指针位置到pt
	//printf("x=%ld,y=%ld\n", pt.x, pt.y);
	Say("HEllo world adfdsfsdf");
	//KeyPress("a", 1);
	//Sleep(1000);
	//SetLogLevel(0);
	//SetCapsLock();
	//Sleep(500);
	//int ret = GetCapsLock();
	//printf("ret=%d\n",ret);
	CloseDevice();





#else

	int res;
	unsigned char buf[256];
	#define MAX_STR 255
	wchar_t wstr[MAX_STR];
	hid_device *handle;

#ifdef WIN32
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
#endif

	struct hid_device_info *devs, *cur_dev;
	
	if (hid_init())
		return -1;

	devs = hid_enumerate(0x0, 0x0);
	cur_dev = devs;	
	while (cur_dev) {
		if (cur_dev->vendor_id == 0x2341) {
			printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
			printf("\n");
			printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
			printf("  Product:      %ls\n", cur_dev->product_string);
			printf("  Release:      %hx\n", cur_dev->release_number);
			printf("  Interface:    %d\n", cur_dev->interface_number);
			printf("\n");
		}
		cur_dev = cur_dev->next;
	}
	hid_free_enumeration(devs);



	// Open the device using the VID, PID,
	// and optionally the Serial number.
	////handle = hid_open(0x4d8, 0x3f, L"12345");
	handle = hid_open(GHOST_VID, GHOST_PID, NULL);
	if (!handle) {
		printf("unable to open device\n");
 		return 1;
	}

	// Read the Manufacturer String
	wstr[0] = 0x0000;
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read manufacturer string\n");
	printf("Manufacturer String: %ls\n", wstr);

	// Read the Product String
	wstr[0] = 0x0000;
	res = hid_get_product_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read product string\n");
	printf("Product String: %ls\n", wstr);

	// Read the Serial Number String
	wstr[0] = 0x0000;
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read serial number string\n");
	printf("Serial Number String: (%d) %ls\n", wstr[0], wstr);

	// Read Indexed String 1
	wstr[0] = 0x0000;
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	if (res < 0)
		printf("Unable to read indexed string 1\n");
	printf("Indexed String 1: %ls\n", wstr);

	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(handle, 1);
	
	// Try to read from the device. There shoud be no
	// data here, but execution should not block.
	res = hid_read(handle, buf, 17);



	/* Free static HIDAPI objects. */
	hid_close(handle);
	hid_exit();

#ifdef WIN32
	system("pause");
#endif



#endif
	return 0;
}
