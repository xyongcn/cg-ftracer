#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <assert.h>
#define Size 4096*10
#define PORT 8000
#define true 1
#define false 0
void intToByte(int i,unsigned char *bytes,int size);
int bytesToInt(unsigned char* bytes,int size);

int main(int argc, char **argv)
{
    int sockfd, connfd;
    FILE *fp;
//    char *path = "/home/wu/Desktop/parse/test/";
    if (argc!=2) {
	printf("Wrong parameter!\n");
	return -1;
    }
	
    char *path = argv[1];
    char *filename = argv[1];//"/dev/shm/test/ftrace.data";
    long count = 0;
    int len;
    struct sockaddr_in servaddr;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    if( listen(sockfd, 10) == -1){  
    //listen 监听  
        printf("listen socket error\n");  
        exit(0);  
    } 
	
	int n;
	unsigned char * recvbuf = NULL;
    	unsigned char * block = NULL;
	unsigned char * next_temp_block = NULL;
	recvbuf = (unsigned char *)malloc(Size * sizeof(char));
	block = (unsigned char *)malloc(Size * sizeof(char));
	next_temp_block = (unsigned char *)malloc(Size * sizeof(char));
	memset(recvbuf, 0 , Size);
	memset(block,0,Size);
    	memset(next_temp_block,0,Size);

	if( (connfd = accept(sockfd, (struct sockaddr*)NULL, NULL)) == -1){  
            printf("accept socket error\n");  
            exit (0);  
        } else {
	    printf("accept!\n");
	}
	
	int offset_next_block = 0;
	int offset_block = 0;
	int offset_recv = 0;
	int recv_remain = 0;
	int block_remain = 0;
	int block_done_flag = true;
	int blocklen;

    while(1)//one circle, one block, one file
    {
	if(offset_recv >= Size) {
		printf("offset_recv >= Size!!\n");
		goto out;
	}

        len = recv(connfd, recvbuf + recv_remain, Size - recv_remain, 0);
	if ( len <= 0)
		goto out;	
#ifdef PRINT
	printf("rev Len = %d, recv buf at %d\n", len, len + recv_remain);
#endif
	if (block_remain == 0) {
		//first int(4 byte) means block len
		blocklen = bytesToInt(recvbuf, 4);
		offset_recv += 4;
#ifdef PRINT
		printf("block Len = %d\n", blocklen);
#endif
		block_remain = blocklen;		
		//block_done_flag = false;
		
		//next_temp_block ==> block
//		if ( offset_next_block != 0 )
//			memcpy(block, next_temp_block, offset_next_block);
//		offset_block = offset_next_block;
	}

	//recvbuf ==> block
	while (offset_block < blocklen && offset_recv < (recv_remain + len) ) {
		//remain buf => block
		//int j;		
		//for (j=0; remain>0; remain--){
		//	block[offset_block++] = recvbuf[j];
		//}
		
		block[offset_block++] = recvbuf[offset_recv++];
		block_remain--;
	}
#ifdef PRINT
	printf("offset_block: %d, offset_recv: %d, block_remain: %d\n", offset_block, offset_recv, block_remain);
#endif
	assert(block_remain >= 0);
	if (block_remain == 0) {
		//this block is done
		block_done_flag = true;
		offset_block = 0;
//		sprintf(filename,"%s%ld",path,count);
//		printf("%s\n",filename);
		fp=fopen(filename,"ab");
		fwrite(block,sizeof( unsigned char ),blocklen,fp);
		fclose(fp);
		count++;
		//remain recvbuf ==> next_temp_block ==>recvbuf

///!!!!!!!!!!!!!!!!!
		recv_remain = recv_remain + len - offset_recv;
#ifdef PRINT
		printf("recv_reamin : %d\n", recv_remain);
#endif
		if ( recv_remain > 0) {
			memcpy( next_temp_block, recvbuf + offset_recv, recv_remain);
			offset_next_block = recv_remain;
			
			memcpy( recvbuf, next_temp_block, recv_remain); 
			offset_recv = 0;
		} else {
			//assert(offset_recv == (recv_remain + len));
			assert(recv_remain == 0);			
			offset_next_block = 0;
			offset_recv = 0;
		}
		
	} else {
		//block not done
		assert(offset_recv == recv_remain + len);
		//assert(!block_done_flag);
		offset_recv = 0;
		recv_remain = 0;
		continue;
	}

    }
out:
    close(sockfd);
    printf("CLOSE!!\n");
}

void intToByte(int i,unsigned char *bytes,int size)
{
     //byte[] bytes = new byte[4];
//    memset(bytes,0,sizeof(byte) *  size);
    bytes[0] = (unsigned char) (0xff & i);
    bytes[1] = (unsigned char) ((0xff00 & i) >> 8);
    bytes[2] = (unsigned char) ((0xff0000 & i) >> 16);
    bytes[3] = (unsigned char) ((0xff000000 & i) >> 24);
    return ;
 }

//byte转int
 int bytesToInt(unsigned char* bytes,int size) 
{
    int addr = bytes[0] & 0xFF;
    addr |= ((bytes[1] << 8) & 0xFF00);
    addr |= ((bytes[2] << 16) & 0xFF0000);
    addr |= ((bytes[3] << 24) & 0xFF000000);
    return addr;
 }
