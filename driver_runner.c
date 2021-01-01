#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256

static char msg[BUFFER_LENGTH];

int main(){
	int ret, fd;
	char string[BUFFER_LENGTH];
	printf("Opening device driver file...\n");
	fd = open("/dev/simple_chardev", O_RDWR); //open device driver
	if(fd < 0){
		perror("Failed to open the specified file.");
	}
	
	printf("Type a message to be sent to the kernel module:\n");
	scanf("%[^\n]%*c", string);
	printf("Writing %s to the device...\n", string);
	ret = write(fd, string, strlen(string)); //write into the device

	printf("Press ENTER to read the message sent to kernel module\n");
	getchar();

	printf("Read from device driver...\n");
	ret = read(fd, msg, BUFFER_LENGTH); //read back from device
	printf("The message is %s\n", msg);
	return 0;
}
