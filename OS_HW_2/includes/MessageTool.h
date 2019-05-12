
#define MAXIMUM_MESSSAGE_SIZE 80

struct MessageBuffer{
    long messageType;
    char message[MAXIMUM_MESSSAGE_SIZE];
};

int GetMessageQueueID(const char* pathName);
void SendMessage(int id, const char* content, int n, long type);
void ReadMessage(int id, char* content, int* size, long type, int msgflag);
void RemoveMessageQueue(int id);

