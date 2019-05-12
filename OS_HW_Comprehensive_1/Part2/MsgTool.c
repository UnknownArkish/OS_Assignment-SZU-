#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX_SEND_SIZE 80

struct MyMsgBuf{
    long msgType;
    char msgContent[MAX_SEND_SIZE];
};

extern void SendMessage(int qid, struct MyMsgBuf* qbuf, long type, char* text);
extern void ReadMessage(int qid, struct MyMsgBuf* qbuf, long type);
extern void RemoveQueue(int qid);
extern void ChangeMessageQueueMode(int qid, char* mode);
extern void usage();

int main(int argc, char** argv){
    key_t key;
    int qid;
    struct MyMsgBuf qbuf;

    if( argc == 1 ){
        usage();
    }

    key = ftok(".",'m');                            // 通过ftok创建唯一的key
    qid = msgget(key, IPC_CREAT|0666);              // 通过key创建对应的IPC对象
    if( qid < 0 ){
        perror("msgget");
        exit(1);
    }

    switch (tolower(argv[1][0]))
    {
        case 's':
            SendMessage(qid, &qbuf, atol(argv[2]), argv[3]);
            break;
        case 'r':
            ReadMessage(qid, &qbuf, atol(argv[2]));
            break;
        case 'd':
            RemoveQueue(qid);
        break;
        case 'm':
            ChangeMessageQueueMode(qid, argv[2]);
        break;
        
        default:
            usage();
        break;
    }
    return 0;
}

void SendMessage(int qid, struct MyMsgBuf* qbuf, long type, char* text){
    printf("Sending a message\n");
    qbuf->msgType = type;
    strcpy(qbuf->msgContent, text);

    if( msgsnd(qid, qbuf, strlen(qbuf->msgContent)+1, 0) == -1 ){
        perror("msgsnd");
        exit(1);
    }
    return;
}
void ReadMessage(int qid, struct MyMsgBuf* qbuf, long type ){
    printf("Reading a message\n");
    qbuf->msgType = type;
    msgrcv(qid, qbuf, MAX_SEND_SIZE, type, 0);
    printf("Type: %ld Text: %s\n", qbuf->msgType, qbuf->msgContent);
    return;
}
void RemoveQueue(int qid){
    msgctl(qid, IPC_RMID, 0);
    return;
}
void ChangeMessageQueueMode(int qid, char* mode){
    struct msqid_ds ds;

    msgctl(qid, IPC_STAT, &ds);
    sscanf(mode, "%ho", &ds.msg_perm.mode);

    msgctl(qid, IPC_SET, &ds);
    return;
}
void usage(){
    fprintf(stderr, "MsgTool -A utility for tinkering with msg queue\n");
    fprintf(stderr, "nUsage: MsgTool s(end)\n");
    fprintf(stderr, "        MsgTool r(ecv)\n");
    fprintf(stderr, "        MsgTool d(elete)\n");
    fprintf(stderr, "        MsgTool m(ode)\n");
    exit(1);
}
