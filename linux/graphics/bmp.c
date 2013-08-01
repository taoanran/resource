#include <stdio.h>

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

//if the outImage->bits() is the source of the RGB(ARGB 32)
int main()
{
    static int index=0;
    index++;
    char filename[16];
    memset(filename,0,sizeof(filename));
    sprintf(filename,"data_%d.bmp",index);


    unsigned char head[54];
    gen_bmp_header(head,width,height,4);


    FILE * fp=fopen(filename,"w+");
    if(fp)
    {
        fwrite(head,54,1,fp);
        fwrite(outImage->bits(),outImage->height()*bpl,1,fp);
        fclose(fp);
    }
}
