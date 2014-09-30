#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ext/hash_map>
#include <stack>  
#include <cstdint>  
#include <assert.h>

#define PAGESIZE 4096

using std::string;  
using __gnu_cxx::hash_map;
using std::stack;
hash_map <unsigned long, string> map_ip2result;
hash_map <unsigned long, string>::iterator iter;



enum trace_type {
	__TRACE_FIRST_TYPE = 0,

	TRACE_FN,
	TRACE_CTX,
	TRACE_WAKE,
	TRACE_STACK,
	TRACE_PRINT,
	TRACE_BPRINT,
	TRACE_MMIO_RW,
	TRACE_MMIO_MAP,
	TRACE_BRANCH,
	TRACE_GRAPH_RET,
	TRACE_GRAPH_ENT,
	TRACE_USER_STACK,
	TRACE_BLK,

	__TRACE_LAST_TYPE,
};

typedef struct page_head{
	unsigned long long time;
	unsigned long commit;
}PageHead;


typedef struct extra_head{
	unsigned long array;
	unsigned long extra;
}ExtraHead;

struct trace_entry {
	unsigned short	type;
	unsigned char	flags;
	unsigned char	preempt_cont;
	int		tid;
	int		pid;
};

struct function_tracer_entry {
	unsigned long ip;
	unsigned long parent_ip;
};

struct ftrace_graph_ent {
	unsigned long func; /* Current function */
	int depth;
};
struct ftrace_graph_ret {
	unsigned long unkwon_padding1;
	unsigned long func; /* Current function */
	unsigned long unkwon_padding2;
	unsigned long long calltime;
	unsigned long long rettime;
	/* Number of functions that overran the depth limit for current task */
	unsigned long overrun;
	int depth;
};


void replace(char* str) {
	if (NULL == str) {
		return;
	}
	int count = 0;
	while (*str != '\0') {
		if (*str == '\n') {
			count++;
			if (count==1)
				*str = ':';
			else
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
		//		printf("*********************zhaodaola !!********************\n");
		const char * s = iter->second.c_str();
		strcpy(result, s);
		return;
	}
	FILE *fstream=NULL;  
	char buff[60]="";      
	char cmd[60]="addr2line -e /dev/shm/vmlinux -i -f 0x";
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

	printf("%s\n",result);
	pclose(fstream);   
}

stack <ftrace_graph_ent> func_stack;
stack <ftrace_graph_ent> call_func_stack;
stack <ftrace_graph_ent> tmp_stack;
int call_dept = 0;

int main(int argc, char *argv[])
{
	struct trace_entry ftrace_entry;
	struct function_tracer_entry fn_entry;
	struct ftrace_graph_ent fg_entry;
	struct ftrace_graph_ret fg_return;

	struct ftrace_graph_ent tmp_ent;
	struct ftrace_graph_ret tmp_ret;	

	unsigned long call_func_ip = 0;
	tmp_ent.func = 0;
	tmp_ent.depth = -100;
	call_func_stack.push(tmp_ent);
	//call_func_stack.push(tmp_ent);

	PageHead pagehead;// = (PageHead*)malloc(sizeof(PageHead));
	ExtraHead extrahead;// = (ExtraHead*)malloc(sizeof(ExtraHead));
	unsigned char* page_buf = (unsigned char*)malloc(PAGESIZE+100);
	assert(page_buf!=NULL);
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

	while(!feof(fp)){//a page

		//read a page
		fread(page_buf,PAGESIZE,1,fp);
		int offset = 0;
		//printf("offset: %d\n", offset);
		//fread(&pagehead,sizeof(pagehead),1,fp);
		memcpy(&pagehead, page_buf+offset, sizeof(pagehead));
		offset += sizeof(pagehead);

		//printf("offset: %d\n", offset);

		time = pagehead.time;
		printf("time: %lld\n", time);
		while(offset < 4096)
		{	

			//fread(&len_time,sizeof(len_time),1,fp);
			memcpy(&len_time, page_buf+offset, sizeof(len_time));
			offset += sizeof(len_time);

			//printf("offset: %d\n", offset);			

			if (offset >= 4096)
				break;
			if((len_time&0x1f) == 30)
			{ 
				//fread(&extrahead,sizeof(extrahead),1,fp);
				memcpy(&extrahead, page_buf+offset, sizeof(extrahead));
				offset += sizeof(extrahead);

				//printf("offset: %d\n", offset);

				if (offset >= 4096)
					break;
			}
			delta_time = (len_time >> 5) & 0x07ffffff;
			time += delta_time;
			//fread(&out_ent, sizeof(out_ent), 1, fp);
			memcpy(&ftrace_entry, page_buf+offset, sizeof(ftrace_entry));
			offset += sizeof(ftrace_entry);

			//printf("offset: %d\n", offset);

			if (offset >= 4096)
				break;
			//printf("type:%x\n", ftrace_entry.type);
			switch(ftrace_entry.type) {
			case 1:/*FUNCTION_TRACER*/
				memcpy(&fn_entry, page_buf+offset, sizeof(fn_entry));
				offset += sizeof(fn_entry);
				if (fn_entry.ip != 0 && fn_entry.parent_ip != 0) 
				{
					get_funcname(fn_entry.ip,result_ip);
					get_funcname(fn_entry.parent_ip,result_pip);
					fprintf(out_fp,"PID:%d TID:%d TIME:0x%x RETURN_TIME:NULL "/*%lld*/, ftrace_entry.pid, ftrace_entry.tid,time);
					fprintf(out_fp,"FROM:%s",result_pip);
					fprintf(out_fp,"TO:%s",result_ip);
					fprintf(out_fp,"AT:NULL TIME:NULL\n");
				}
				break;
			case 10:/*TRACE_GRAPH_RET*/
				memcpy(&fg_return, page_buf+offset, sizeof(fg_return));
				offset += sizeof(fg_return);
				//get_funcname(fg_return.func,result_ip);
				//fprintf(out_fp, "RETURN:%s, DEPT:%d\n", result_ip, fg_return.depth);
				//pop stack
				while (!func_stack.empty()) {
					tmp_ent = func_stack.top();
					if (tmp_ent.func == fg_return.func && tmp_ent.depth == fg_return.depth) {
						//一个有效条目
						//找到该函数的调用函数
						call_func_stack.pop();
						while (call_func_stack.top().depth == fg_return.depth)
						{
							tmp_stack.push(call_func_stack.top());
printf("***********111\n");
printf("%d\n",call_func_stack.top().depth);
							call_func_stack.pop();
printf("***********222\n");
						}
						call_func_ip = call_func_stack.top().func;
						while (!tmp_stack.empty()){
							call_func_stack.push(tmp_stack.top());

							tmp_stack.pop();
						}

	printf("DEPT:%d ", fg_return.depth);
						get_funcname(fg_return.func,result_ip);
						get_funcname(call_func_ip,result_pip);

						fprintf(out_fp,"PID:%d TID:%d TIME:0x%x ", 
							ftrace_entry.pid, ftrace_entry.tid, time/*fg_return.calltime*/);
						fprintf(out_fp,"RETURN_TIME:0x%x ", fg_return.rettime);
						fprintf(out_fp,"FROM:%s",result_pip);
						fprintf(out_fp,"TO:%s",result_ip);
						fprintf(out_fp,"AT:NULL TIME:NULL\n");

						//printf("PID:%d TID:%d TIME:0x%x RETURN_TIME:0x%x "/*%lld*/, 
						//	ftrace_entry.pid, ftrace_entry.tid,fg_return.calltime, fg_return.rettime);
						//printf("FROM:%s",result_pip);
						//printf("TO:%s",result_ip);
						//printf("AT:NULL TIME:NULL\n");
						//fprintf(out_fp, "RETURN:%s, DEPT:%d\n", result_ip, fg_return.depth);
						func_stack.pop();
						
						
						break;
					}
					func_stack.pop();
printf("***********\n");
					call_func_stack.pop();
				}
				break;
			case 11:/*TRACE_GRAPH_ENT*/
				memcpy(&fg_entry, page_buf+offset, sizeof(fg_entry));
				offset += sizeof(fg_entry);
				//get_funcname(fg_entry.func,result_ip);
				//fprintf(out_fp, "ENTRY:%s, DEPT:%d\n", result_ip, fg_entry.depth);
				tmp_ent = fg_entry;

				while (!func_stack.empty() && func_stack.top().depth > fg_entry.depth)
				{
					//中间有数据丢失
					func_stack.pop();
					call_func_stack.pop();
				}

				func_stack.push(tmp_ent);
				call_func_stack.push(tmp_ent);

				break;
			}

			//getchar();
		}
	} 	
	fclose(fp);	
	fclose(out_fp);	
	return 0;
}
