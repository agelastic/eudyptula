#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define IOC_MAGIC 'r'
#define FAT_IOCTL_SET_VOLUME_LABEL _IOW(IOC_MAGIC, 0x14, char *)

int main(int argc, char *argv[])
{
	int result;
	unsigned int fd;
	char newlabel[20];


	printf("New label? \n");
	fgets(newlabel,20,stdin);
	printf("Will assign new label: %s\n",newlabel);

	fd = open("/mnt/myfloppy/", 0);
	if (!fd)
		printf("failed to open dir\n");

	if (ioctl(fd, FAT_IOCTL_SET_VOLUME_LABEL, newlabel))
		printf("set volume failed\n");
	else
		printf("successfully changed label\n");

}
