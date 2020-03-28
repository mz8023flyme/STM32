#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define TSL2561_DATA0_LOW     0x8C  
#define TSL2561_DATA0_HIGH    0x8D  
#define TSL2561_DATA1_LOW     0x8E  
#define TSL2561_DATA1_HIGH    0x8F 

int fd;

int main(int argc ,char** argv)
{
    unsigned char buf[4];
    unsigned char i=0 ;
    fd=open("/dev/tsl2561",O_RDWR);
    if(fd<0)
    {
        printf("can't open /dev/tsl2561");
        return -1;
    }

    while(1)
    {   
        for(i=0;i<4;i++)
        {
            buf[i]=TSL2561_DATA0_LOW+i;
            read(fd,&buf[i],1);
            printf("%d\n",buf[i]);
        }
        sleep(100);
    }
    close(fd);
    
    return 0 ;
}









