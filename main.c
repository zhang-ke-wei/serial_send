#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>


int set_opt(int,int,int,char,int);
int main()
{
    char *serial = "/dev/ttySAC3";
    char *buffer = "hello world!\n";
    int fd = 0;
    int i = 100;
    int write_sta;
    char str[10];

    if((fd = open(serial, O_RDWR|O_NOCTTY|O_NDELAY))<0)
    {
        printf("open %s failed!",serial);
    }
    else
    {
        printf("open %s success!",serial);
        set_opt(fd,9600,8,'N',1);
        while (i--)
        {
            write_sta = write(fd,buffer, strlen(buffer));
            sprintf(str,"%d\n",i);
            write_sta = write(fd,str, strlen(str));
            if (write_sta < 0)
            {
                printf("write failed!");
            }
            else
            {
                printf("write success!");
            }
            if (i==2)
                i = 100;
            sleep(1);
        }
    }
    close(fd);
    return 0;
}

/*串口参数设置*/
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
    struct termios newtio,oldtio;  //串口参数结构体
    if  ( tcgetattr( fd,&oldtio)  !=  0) { //读取串口参数
        perror("SetupSerial 1");
        return -1;
    }
    bzero( &newtio, sizeof( newtio ) );//类似memset
    newtio.c_cflag  |=  CLOCAL | CREAD;  /* control modes */
    newtio.c_cflag &= ~CSIZE;

    switch( nBits )
    {
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }

    switch( nEvent )
    {
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK | ISTRIP);
            break;
        case 'E':
            newtio.c_iflag |= (INPCK | ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;
    }

    switch( nSpeed )
    {
        case 2400:
            cfsetispeed(&newtio, B2400);
            cfsetospeed(&newtio, B2400);
            break;
        case 4800:
            cfsetispeed(&newtio, B4800);
            cfsetospeed(&newtio, B4800);
            break;
        case 9600:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
        case 115200:
            cfsetispeed(&newtio, B115200);
            cfsetospeed(&newtio, B115200);
            break;
        case 460800:
            cfsetispeed(&newtio, B460800);
            cfsetospeed(&newtio, B460800);
            break;
        default:
            cfsetispeed(&newtio, B9600);
            cfsetospeed(&newtio, B9600);
            break;
    }
    if( nStop == 1 )
        newtio.c_cflag &=  ~CSTOPB;
    else if ( nStop == 2 )
        newtio.c_cflag |=  CSTOPB;
    newtio.c_cc[VTIME]  = 0;
    newtio.c_cc[VMIN] = 0;
    tcflush(fd,TCIFLUSH);   //清空串口BUFFER中的数据
    if((tcsetattr(fd,TCSANOW,&newtio))!=0) //设置串口参数函
    {
        perror("com set error");
        return -1;
    }
//	printf("set done!\n\r");
    return 0;
}


