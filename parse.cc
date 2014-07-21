#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ext/hash_map>    
#include <cstdint>  

using std::string;  
using __gnu_cxx::hash_map;  
hash_map <unsigned long, string> map_ip2result;
hash_map <unsigned long, string>::iterator iter;

typedef struct page_head{
unsigned long long time;
unsigned long commit;
}PageHead;


typedef struct extra_head{
 unsigned long array;
 unsigned long extra;
}ExtraHead;

typedef struct ftrace_entry
{
	struct trace_entry {
		unsigned short	type;
		unsigned char	flags;
		unsigned char	preempt_cont;
		int		pid;
		int		padding;
	} entry;
	unsigned long ip;
	unsigned long parent_ip;
}FtraceEntry;

void replace(char* str) {
    if (NULL == str) {
        return;
    }
     
    while (*str != '\0') {
        if (*str == '\n') {
            *str = ' ';
        }
        str++; //去掉了*  *str++;
    }
    return;
}

void get_funcname(unsigned long ip,char* result)
{
	
	iter = map_ip2result.find(ip);
	if(iter != map_ip2result.end()) {
   		//找到
		printf("*********************zhaodaola !!********************\n");
		const char * s = iter->second.c_str();
		strcpy(result, s);
		return;
	}
    FILE *fstream=NULL;  
    char buff[60]="";      
    char cmd[60]="addr2line -e /dev/shm/vmlinux -f 0x";
    char str[20];
    
    memset(result,0,sizeof(result)); 
    sprintf(str,"%lx",ip);    
    strcat(cmd,str);
    printf("%s\n",cmd);
    if(NULL==(fstream=popen(cmd,"r")))       
    {      
        fprintf(stderr,"execute command failed: ");       
        return;       
    }      
    while(NULL!=fgets(buff, sizeof(buff), fstream))      
    {   
        strcat(result,buff);       
    }
printf("%s\n",result);
    replace(result);

	map_ip2result[ip] = result;

    //printf("%s\n",result);
    pclose(fstream);   
}

int main(int argc, char *argv[])
{
  FtraceEntry out_ent;
  PageHead pagehead;
  ExtraHead extrahead;
  unsigned long len_time;
  unsigned long long time;
  unsigned long delta_time;

  FILE *fp = NULL;
  FILE *out_fp = NULL;
  int count = 0;

  char* result_ip;
  char* result_pip;
  result_ip = (char*)malloc(sizeof(char)*255);
  result_pip = (char*)malloc(sizeof(char)*255);
  
  fp = fopen(argv[1], "rb");
  out_fp = fopen(argv[2], "wt");
  fflush(out_fp);

 fread(&pagehead,sizeof(pagehead),1,fp);
 time = pagehead.time;
  while(!feof(fp))
  {
//    count++;

    fread(&len_time,sizeof(len_time),1,fp);
//    fprintf(out_fp,"len:0x%lx,", len_time);
    if((len_time&0x8)>0)
    { 
//      fprintf(out_fp,"ExtraHead\n");
      fread(&extrahead,sizeof(extrahead),1,fp);
    }

    delta_time = (len_time >> 5) & 0x07ffffff;
    time += delta_time;
    fread(&out_ent, sizeof(out_ent), 1, fp);
	if (out_ent.ip != 0 && out_ent.parent_ip != 0) 
	{
		get_funcname(out_ent.ip,result_ip);
		get_funcname(out_ent.parent_ip,result_pip);

		fprintf(out_fp,"TIME:%lld, PID:%d, ", time, out_ent.entry.pid);
	    //printf("1\n");
	    //fprintf(stdout,"PID:0x%x,",out_ent.m_vmtrace.ent.pid);
	    //fprintf(stdout,"Flag:%d,",out_ent.m_vmtrace.call_ret_flag);
	//    fprintf(out_fp,"CALL:%0x,",out_ent.parent_ip);
	//    fprintf(out_fp,"RETURN:%0x\n,",out_ent.ip);

		fprintf(out_fp,"CALL:%s,",result_pip);
		fprintf(out_fp,"RETURN:%s\n",result_ip);
	}
//	int temp = 0xc004bd14;
//	if ( out_ent.parent_ip == temp || out_ent.ip == temp)
//	{
//		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
//		while(1){}
//		break;
//	}
//    if(count == 100)
//      break;

  }
    
  
  fclose(fp);
  fclose(out_fp);
  return 0;
}
