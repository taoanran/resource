#define LOG_NDEBUG 0

#define LOG_TAG "CatchFB"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "android_runtime/AndroidRuntime.h"
#include "utils/Log.h"

using namespace android;


#pragma pack(1)
// 2byte   
typedef struct bmp_magic  
{  
 unsigned char magic[2];  
}magic_t;  
  
  
// 4 * 3 = 12 Byte   
typedef struct bmp_header  
{  
 unsigned int file_size;    //file size in Byte ,w * h * 3 + 54   
 unsigned short creater1;   //0   
 unsigned short creater2;   //0   
 unsigned int offset;   //offset to image data: 54D, 36H   
}header_t ;  
  
//10 * 4 =  40 Byte   
typedef struct bmp_info  
{  
 unsigned int header_size; //info size in bytes, equals 4o D, or 28H   
 unsigned int width;     //file wideth in pide   
 unsigned int height;    //file height in pide   
  
 unsigned short nplanes;   //number of clor planes , 1   
 unsigned short bitspp;    //bits per pidel, 24d, 18h   
  
 unsigned int compress_type;  //compress type,default 0   
 unsigned int image_size;   //image size in Byte.  w * h * 3   
 unsigned int hres;      //pideles per meter, 0   
 unsigned int vres;      //pideles per meter, 0   
 unsigned int ncolors;   //number of colors, 0   
 unsigned int nimpcolors; //important colors, 0   
}info_t;  
   
  
int gen_bmp_header(unsigned char *head,unsigned w, unsigned h,unsigned bytepp)  
{  
 if(head==NULL)
 	return -1;
 magic_t magic;
 info_t info;
 header_t header;
 
 magic.magic[0] = 'B';  
 magic.magic[1] = 'M';  
  
 header.file_size = (w * h * bytepp + 54);  
 header.creater1 = 0;  
 header.creater2 = 0;  
 header.offset = (54);  
   
 info.header_size = (40);  
 info.width = (w);  
 info.height = (h);  
 info.nplanes = (1);  
 info.bitspp = (bytepp * 8);  
 info.compress_type = 0;  
 info.image_size = (w * h * bytepp);  
 info.hres = 0;  
 info.vres = 0;  
 info.ncolors = 0;  
 info.nimpcolors = 0;  

 unsigned char *p=head;
 memcpy(p,&magic,sizeof(magic));
 p+=sizeof(magic);
 memcpy(p,&header,sizeof(header));
 p+=sizeof(header);
 memcpy(p,&info,sizeof(info));
 return 0; 
}  
#if 0
void saveBitMap(const int x, const int y, const int bit, const void *addr, const int size)
{
	unsigned char head[54];
	FILE *fp = NULL;
	memset(head, 0, 54);

	LOGV("x=%d, y=%d, bit=%d, sizeof=%d\n", x,y,bit,sizeof(magic_t)+sizeof(header_t)+sizeof(info_t));
	
	gen_bmp_header(head,x, y,bit);

	fp = fopen("/data/test/test.bmp", "w+");
	if (fp == NULL)
	{
			LOGE("============== fopen file error ========\n");
			return ;
	}

	fwrite(head, 54, 1, fp);
	fwrite(addr, size, 1, fp);

	fclose(fp);
	fp = NULL;
}
#endif
void saveBitMap(const int x, const int y, const int bit, void *addr, const int size)
{
	unsigned char head[54];
	int i;
	void *tmp = addr+x*y*bit-x*bit;
	FILE *fp = NULL;
	memset(head, 0, 54);

	LOGV("x=%d, y=%d, bit=%d, sizeof=%d\n", x,y,bit,sizeof(magic_t)+sizeof(header_t)+sizeof(info_t));
	
	gen_bmp_header(head,x, y,bit);

	fp = fopen("/data/test/test.bmp", "w+");
	if (fp == NULL)
	{
			LOGE("============== fopen file error ========\n");
			return ;
	}

	fwrite(head, 54, 1, fp);
	//fwrite(addr, size, 1, fp);
	for(i=0; i<y; i++)
	{
		fwrite(tmp, x*bit, 1, fp);
		tmp -= x*bit;
	}

	fclose(fp);
	fp = NULL;
}

int main()
{
		struct fb_fix_screeninfo fb_fixinfo;
		struct fb_var_screeninfo fb_varinfo;
		int fb ;
		int fb_size = -1;
		void *fb_addr = NULL;
		char *buf;


		FILE *fp = NULL;

		fb = open("/dev/graphics/fb2", O_RDWR);
		if (fb < 0)
		{
				LOGE("===============open /dev/graphics/fb2 error ==================\n");		
				return -1;		
		}
		
		if (ioctl(fb, FBIOGET_FSCREENINFO, &fb_fixinfo) == -1)
		{
						LOGE("============== ioctl FBIOGET_FSCREENINFO failed ========\n");
						return -1;
		}

		if (ioctl(fb, FBIOGET_VSCREENINFO, &fb_varinfo) < 0)
		{
						LOGE("===============ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) error ==================\n");		
						return -1;		
		}
		
		fb_size = fb_varinfo.yres*fb_varinfo.xres*fb_varinfo.bits_per_pixel / 8;

		LOGV("fb_size = %d\n", fb_size);

		LOGV("fb_varinfo.xres = %d\n", fb_varinfo.xres);
		LOGV("fb_varinfo.yres = %d\n", fb_varinfo.yres);
		LOGV("fb_varinfo.bits_per_pixel = %d\n", fb_varinfo.bits_per_pixel);

		fb_addr = (void *)mmap(NULL, fb_size, PROT_READ|PROT_WRITE,MAP_SHARED,fb, 0); //将framebuffer的物理地址映射到进程地址空间
		if (fb_addr == MAP_FAILED)
		{
				LOGE("============== mmap error (%d) ========\n", errno);
				
				return -1;
		}

		//save bitmap
		saveBitMap(fb_varinfo.xres,fb_varinfo.yres,fb_varinfo.bits_per_pixel/8, fb_addr, fb_size);
		
		munmap(fb_addr, fb_size);
		return 0;
}
