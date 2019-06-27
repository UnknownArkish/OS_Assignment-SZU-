
#define MAXIMUM_SPERATOR_COUNT 256          // 最大分隔符数量
#define MAXIMUM_FILE_NAME 256               // 最大文件名长度

#define ROOT_INODE_INDEX 2

// 块设备路径
const static char* DEV_PATH = "/dev/loop1";
const static char* file_type_name[] = {
    "unknown",
    "reg_file",
    "directory",
    "chrdev",
    "blkdev",
    "fifo",
    "sock",
    "symlink",
};

static char* debug_path = "/directory/file1";


void PrintDirEntry( struct DirEntry* src );

void SearchPath( char* path );
int SearchDirEntry( struct DirEntry* dest,  struct DirEntry src[], int count, char name[] );
void HandleDirectory( int inodeIndex );
void HandleLinkFile( int inodeIndex );
void HandleOtherFile( int inodeIndex );


/* 
    根据一个索引节点号，得到物理字节地址
*/
int GetINodeAddress( int inodeIndex );

/*
    根据一个索引节点号，得到一个索引节点结构体
*/
void GetInode(int inodeIndex, struct INode* inode);

/*
    根据盘块号，得到一个块的内容
*/
void GetBlockContent( int blockIndex, char buf[] );


/*
    获取指定层数的block所能表示的内容大小
*/
int GetBlockDegreeSize( int blockDegree );
/*
    获取指定block的内容，需要指定层数
*/
int GetBlockDegreeContent( int blockAdress, int blockDegree, char buf[] );
/*
    读取inode中的block中的内容
*/
void GetInodeContent( struct INode inode, char buf[], int size );

/*
    解析一个inodeContent，转化为DirEntry数组
*/
void ParseInodeIntoDirEntry( struct INode inode, struct DirEntry DirEntry[], int* dirEntryCount);

/*
    解析buf成为一个dirntry
*/
void ParseDirEntry( char buf[], struct DirEntry* dirEntry );