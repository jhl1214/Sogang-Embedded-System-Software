#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

unsigned char ct_number[10][10] = {
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 0
	{0x0c,0x1c,0x1c,0x0c,0x0c,0x0c,0x0c,0x0c,0x0c,0x1e}, // 1
	{0x7e,0x7f,0x03,0x03,0x3f,0x7e,0x60,0x60,0x7f,0x7f}, // 2
	{0xfe,0x7f,0x03,0x03,0x7f,0x7f,0x03,0x03,0x7f,0x7e}, // 3
	{0x66,0x66,0x66,0x66,0x66,0x66,0x7f,0x7f,0x06,0x06}, // 4
	{0x7f,0x7f,0x60,0x60,0x7e,0x7f,0x03,0x03,0x7f,0x7e}, // 5
	{0x60,0x60,0x60,0x60,0x7e,0x7f,0x63,0x63,0x7f,0x3e}, // 6
	{0x7f,0x7f,0x63,0x63,0x03,0x03,0x03,0x03,0x03,0x03}, // 7
	{0x3e,0x7f,0x63,0x63,0x7f,0x7f,0x63,0x63,0x7f,0x3e}, // 8
	{0x3e,0x7f,0x63,0x63,0x7f,0x3f,0x03,0x03,0x03,0x03}, // 9
};

jstring Java_com_example_androidex_ModeActivity_btnSwitch (JNIEnv *env, jobject thiz){
	int switch_dev, i;
	unsigned char push_sw[9];
	unsigned char temp[10];

	// Open fpga push switch driver
	if((switch_dev = open("/dev/fpga_push_switch", O_RDWR)) < 0)
			perror("/dev/fpga_push_switch open error");

	// Read switch input
	read(switch_dev, &push_sw, 9);

	// Copy to char array
	for (i = 0; i < 9; i++) {
		if (push_sw[i] == 1)
			temp[i] = '1';
		else
			temp[i] = '0';
	}
	temp[9] = '\0';

	close(switch_dev);

	return (*env)->NewStringUTF(env, temp);
}

void Java_com_example_androidex_ModeActivity_printNumber (JNIEnv *env, jobject thiz, jint count){
	int fpga_dot, dot_num, dot_size;

	// Open fpga dot driver
	if((fpga_dot = open("/dev/fpga_dot", O_WRONLY)) < 0)
		perror("/dev/fpga_dot open error");
	dot_size = sizeof(ct_number[10]);

	dot_num = count;

	write(fpga_dot, ct_number[dot_num], dot_size);

	close(fpga_dot);
}
