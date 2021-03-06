#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>


int main()
{
	int fb = -1;
	char *fb_addr = NULL;
	unsigned  int fb_size = 0;
	struct fb_var_screeninfo fb_var;
	struct fb_fix_screeninfo fb_fix;
	FILE *fp = NULL;
	
	fb = open("/dev/fb0", O_RDONLY);
	if (fb < 0)
	{
		printf("open a fb error ---------------------------[%d]\n", __LINE__);
		return -1;	
	}
	
	ioctl(fb, FBIOGET_VSCREENINFO, &fb_var);
	printf("fb_var.xres = %d, fb_var.yres = %d\n", fb_var.xres,fb_var.yres);
	
	ioctl(fb, FBIOGET_FSCREENINFO, &fb_fix);
	printf("fb_fix.line_length=%d\n",fb_fix.line_length);
	
	fb_size = fb_var.xres * fb_var.yres * fb_var.bits_per_pixel / 8;
	printf("size = %d\n", fb_size);
	fb_addr = (char *)mmap(0, fb_size, PROT_READ|PROT_WRITE, MAP_SHARED, fb, 0);
	if (fb_addr == (void *)-1)
	{
		perror("");
		printf("mmap error ---------------------------[%d]\n", __LINE__);
		return -1;
	}
	
	printf("fb_addr = 0x%x\n", *fb_addr);
	
	fp = fopen("./test", "ab+");
	if(fp == NULL)
	{
		printf("fopen test error ---------------------------[%d]\n", __LINE__);
		return -1;
	}
	printf("fwrite = %d\n", fwrite(fb_addr, 1, fb_size, fp));
	fclose(fp);
	fp = NULL;
	
	munmap(fb_addr, fb_size);
	
	close(fb);
	fb = -1;
	
	return 0;	
}
