

#define BUFFER_SIZE 1024

/* 
    根据一个索引节点号，得到物理字节地址
*/
int GetINodeAddress( int inodeIndex );
/*
    根据一个索引节点号，得到其内部的所有blocks数组
*/
void GetInode(int inodeIndex, struct INode* inode);
