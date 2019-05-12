

union SemUnion{
    int val;
    struct semid_ds     *buf;
    unsigned short       *array;
    struct seminfo      *__buf;
    void                *__pad;
};


int GetSemID(const char* pathName, int nsems, int flag);
int GetState(int id, struct semid_ds* buf );
int GetSemNum(int id);


int GetValue(int id, int semIndex);
int GetAllValues(int id, short* buf);


int SetValue(int id, int semIndex, int value);
int SetAllValue(int id, short* values);


int SemP(int id, struct sembuf* sops, unsigned nsops);
int SemPAt(int id, int semIndex, int pValue, int flag);
int SemV(int id, struct sembuf* sops, unsigned nsops);
int SemVAt(int id, int semIndex, int pValue, int flag);

int DeleteSem(int id);