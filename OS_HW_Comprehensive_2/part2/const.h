
#define KB 1024
#define MB ( 1024 * KB )
#define GB ( 1024 * MB )

// 块大小
#define BLOCK_SIZE 1024

// 引导区大小
#define BOOT_SIZE 0x400

/*------------------------------超级块------------------------------*/
#define OFFSET_SUPER_BLOCK_INODES_COUNT 0           // 索引节点总数
#define OFFSET_SUPER_BLOCK_BLOCKS_COUNT 4           // 块数量
#define OFFSET_SUPER_BLOCK_BLOCK_SIZE 24            // 块大小
#define OFFSET_SUPER_BLOCK_BLOCK_PER_GROUP 32       // 块每块组
#define OFFSET_SUPER_BLOCK_INODE_PER_GROUP 40       // 索引节点每块组

/*------------------------------块组描述符------------------------------*/
#define OFFSET_GROUP_DESC_BLOCK_BITMAP 0            // 块位图
#define OFFSET_GROUP_DESC_INODE_BITMAP 4            // 索引节点位图
#define OFFSET_GROUP_DESC_INODE_TABLE 8             // 索引节点表

/*------------------------------索引节点------------------------------*/
#define OFFSET_INODE_SIZE 4                         // i_size
#define OFFSET_INODE_BLOCKS 40                      // i_block

/*------------------------------目录项------------------------------*/
#define OFFSET_DIR_ENTRY_INODE 0                    // inode 
#define OFFSET_DIR_ENTRY_REC_LEN 4                  // rec_len
#define OFFSET_DIR_ENTRY_NAME_LEN 6                 // name_len
#define OFFSET_DIR_ENTRY_FILE_TYPE 7                // file_type
#define OFFSET_DIR_ENTRY_FILE_NAME 8                // file_name

// 超级块大小
#define SUPER_BLOCK_SIZE 1024
// 块组描述符大小
#define GROUP_DESC_SIZE 16
// 索引节点大小
#define INODE_SIZE 128