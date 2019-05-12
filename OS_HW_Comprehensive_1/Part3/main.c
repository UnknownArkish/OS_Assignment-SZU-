#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

#include"Config.h"
#include"Tool.h"


int argc;
char** argv;
const char* tip = "HelloShell";

int pipeNum;
int shouldExit = 0;

// 程序初始化
void Init(){
	argc = 0;
	argv = calloc(ARGV_MAXIMUM_LENGTH, sizeof(char*));
	for( int i = 0 ; i < ARGV_MAXIMUM_LENGTH ; i++ ){
		argv[i] = calloc(ARG_MAXIMUM_SIZE, sizeof(char) );
	}
	pipeNum = 0;
}
// 程序退出前执行
void Clean(){
	pipeNum = 0;
	for( int i = 0 ; i < ARGV_MAXIMUM_LENGTH ; i++ ){
		if(argv[i] != NULL) free(argv[i]);
		argv[i] = NULL;
	}
	free(argv);
	argv = NULL;
}

// 每次输入前的初始化
void InitBeforeInput(){
	argc = 0;
	pipeNum = 0;
}
// 输出提示语
void OutputTip(){
	// 输出提示语
	char pwd[1024];
	getcwd(pwd, sizeof(pwd));
	sprintf(pwd, "%s/%s$ ", tip, pwd);
	fputs(pwd, stdout);

}

// 检查管道数目
void CheckPipeNum(){
	for( int i = 0 ; i < argc ; i++ ){
		if( strcmp(argv[i], "|") == 0 ){
			pipeNum++;
		}		
	}
}

int CheckIsSpecialCommand(char* file, char** argv){
	// 判断是否为特殊命令
	if( strcmp(file,"cd") == 0 ){
		chdir(argv[1]);
		return 0;
	}else if( strcmp(file, "exit") == 0 ){
		shouldExit = 1;
		return 0;
	}
	return -1;
}

void HandleNonPipe( char* file, char** argv ){
	if(CheckIsSpecialCommand(file, argv) == 0)	return;

	pid_t pid = fork();
	if( pid < 0 ){
		fprintf(stderr, "Error to fork!\n");
	}else if( pid == 0 ){
		execvp(file, argv);
		printf("Error on %s\n", file);
		exit(0);
	}else{
		waitpid(pid, NULL, 0);
	}
	return;
}

void HandlePipe( int* argcs, char*** argvs, int* pfd, int curProcessNum, int processNum ){
	if( curProcessNum == -1 )	return;

	int pfd_next[2];
	pipe(pfd_next);
	pid_t firstProcess = fork();
	if( firstProcess < 0 ){

	}else if( firstProcess == 0 ){
		close(pfd[0]);
		if( curProcessNum != processNum -1 ) dup2(pfd[1],1);
		else	close(pfd[1]);
		
		close(pfd_next[1]);
		if( curProcessNum != 0 ) dup2(pfd_next[0],0);
		else	close(pfd_next[0]);
		execvp(argvs[curProcessNum][0], argvs[curProcessNum]);
	}else{
		HandlePipe(argcs, argvs, pfd_next, curProcessNum-1, processNum);
	}
}
void HandleWithPipe(int argc, char** argv){
	// 进程数是管道符数量加一
	int processNum = pipeNum + 1;
	// 为每个线程的参数作准备
	int* argcs = calloc(processNum, sizeof(int));
	for( int i = 0 ; i < processNum ; i++ ){
		argcs[i] = 0;
	}
	char*** argvs = calloc(processNum, sizeof(char*));
	for( int i = 0 ; i < processNum ; i++ ){
		argvs[i] = calloc(ARGV_MAXIMUM_LENGTH, sizeof(char*));
		for( int j = 0 ; j < ARGV_MAXIMUM_LENGTH ; j++ ){
			argvs[i][j] = calloc(ARG_MAXIMUM_SIZE, sizeof(char));
		}
	}
	// 分割每个进程的参数
	for( int i = 0, j = 0 ; i < processNum ; i++ ){
		while( j < argc && strcmp(argv[j],"|") != 0 ){
			strcpy( argvs[i][argcs[i]], argv[j]);
			argcs[i]++;
			j++;
		}
		
		argvs[i][argcs[i]] = NULL;
		j++;
	}	
	
	pid_t pid = fork();
	if( pid < 0 ){

	}else if( pid == 0 ){
		int pfd[2];
		pipe(pfd);
		HandlePipe(argcs, argvs, pfd, processNum - 1, processNum);
		exit(0);
	}else{
		waitpid(pid, NULL, 0);
	}

	// @TODO: 防止内存泄露
	free(argcs);
	argcs = NULL;
	for( int i = 0 ; i < processNum ; i++ ){
		free(argvs[i]);
		argvs[i] = NULL;
	}
	free(argvs);
	argvs = NULL;
}

int main(){
	Init();

	char input[ARG_MAXIMUM_SIZE];
	while( !shouldExit ){							// 判断是否结束
		InitBeforeInput();							// 进行初始化
		OutputTip();								// 输出提示语
		fgets(input, ARG_MAXIMUM_SIZE, stdin);		// 输入
		input[strlen(input)-1] = '\0';
		argc = Split(argv, input, " ");				// 切割字符串
		
		if( argc != 0 ){
			char* temp = argv[argc];
			argv[argc] = NULL;
			CheckPipeNum();						// 检查管道数目
			if( pipeNum > 0 ){					// 如果超过一个管道的情况
				HandleWithPipe(argc, argv);
			}else{
				HandleNonPipe( argv[0], argv );
			}
			argv[argc] = temp;
		}
	}
	Clean();
	return 0;
}


// void HandlePipe( int* argcs, char*** argvs, int processNum ){
// 	printf("HandlePipe\n");

// 	int pfd[2];
// 	pipe(pfd);
// 	for( int i = 0 ; i < processNum ; i+=2 ){
// 		pid_t pidA = fork();
// 		if( pidA < 0 ){

// 		}else if( pidA == 0 ){
// 			dup2( pfd[1], 1 );
// 			if( i == 0 )	close(pfd[0]);
// 			else	dup2(pfd[0], 0);
// 			execvp( argvs[i][0], argvs[i] );
// 		}else if( i < processNum ){
// 			pid_t pidB = fork();
// 			if( pidB < 0 ){

// 			}else if( pidB == 0 ){
// 				dup2(pfd[0], 0);
// 				if( (i+1) == processNum - 1 )	close(pfd[1]);
// 				else	dup2(pfd[1],1);
// 				execvp(argvs[i+1][0], argvs[i+1]);
// 			}else{
// 				// pipe(pfd);
// 			}
// 		}
// 	}
// }
// void HandlePipe_( int* argcs, char*** argvs, int processNum ){
// 	printf("HandlePipe\n");

// 	int pfd[2];
// 	pipe(pfd);
// 	pid_t firstProcess = fork();
// 	if( firstProcess < 0 ){

// 	}else if( firstProcess == 0 ){
// 		dup2(pfd[1],1);
// 		close(pfd[0]);
// 		execvp(argvs[0][0], argvs[0]);
// 	}else{
// 		for( int i = 1 ; i < processNum ; i++ ){
// 			pid_t nextProcess = fork();
// 			if( nextProcess < 0 ){
				
// 			}else if( nextProcess == 0 ){
// 				dup2(pfd[0],0);
// 				if( i == processNum - 1 )	close(pfd[1]);
// 				else	dup2(pfd[1],1);
// 				execvp(argvs[i][0], argvs[i]);
// 			}else{
			
// 			}
// 		}
// 	}
// }