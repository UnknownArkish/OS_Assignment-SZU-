
enum EXT2_FILE_TYPE{
    EXT2_FT_UKNOWN          = 0,
    EXT2_FT_REG_FILE,
    EXT2_FT_DIR,
    EXT2_FT_CHRDEV,
    EXT2_FT_BLKDEV,
    EXT2_FT_FIFO,
    EXT2_FT_SOCK,
    EXT2_FT_SYMLINK,
    EXT2_FT_MAX,
};

struct DirEntry{
    int inode_index;                    // 索引节点号
    int rec_len;                        // 目录项长度
    int name_len;                       // 名字长度
    int file_type;                      // 文件类型
    char name[256];                     // 名字
};