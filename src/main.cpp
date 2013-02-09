#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>
#include <algorithm>

int main(int argc, char *argv[])
{
	--argc;
	if(argc != 2) {
		printf("%s [device] [image]\n", argv[0]);
		return EXIT_FAILURE;
	}
	int fd = open(argv[1], O_RDWR);
	if(fd < 0) {
		perror("open");
		return EXIT_FAILURE;
	}
	
	fb_var_screeninfo vinfo;
	fb_fix_screeninfo finfo;
	
	if(ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("FBIOGET_FSCREENINFO");
		return EXIT_FAILURE;
	}

	// Get variable screen information
	if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("FBIOGET_VSCREENINFO");
		return EXIT_FAILURE;
	}
	
	FILE *image = fopen(argv[2], "r");
	if(!image) {
		perror("fopen");
		return EXIT_FAILURE;
	}
	fseek(image, 0, SEEK_END);
	const unsigned long imagesize = ftell(image);
	unsigned char *buffer = new unsigned char[imagesize];
	
	fclose(image);
	
	const unsigned long screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	unsigned char *display = (unsigned char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	for(;;) {
		memcpy(display, buffer, std::min(screensize, imagesize));
		usleep(50000);
	}
	
	munmap(display, screensize);
	close(fd);
	delete[] buffer;
	
	return EXIT_SUCCESS;
}
