
#define BLOCK_SIZE 1024

// 引导区大小
#define BOOT_SIZE 0x400

// 超级块中 索引节点数量的偏移
#define OFFSET_SUPER_BLOCK_INODES_COUNT 0
// 超级块中 块数量 的偏移
#define OFFSET_SUPER_BLOCK_BLOCKS_COUNT 4
// 超级块中 块大小 的偏移
#define OFFSET_SUPER_BLOCK_BLOCK_SIZE 24
// 超级块中 块每组 的偏移
#define OFFSET_SUPER_BLOCK_BLOCK_PER_GROUP 32
// 超级块中 索引节点每块组 的偏移
#define OFFSET_SUPER_BLOCK_INODE_PER_GROUP 40


// 块组描述符中 块位图 的偏移
#define OFFSET_GROUP_DESC_BLOCK_BITMAP 0
// 块组描述符中 索引节点位图 的偏移
#define OFFSET_GROUP_DESC_INODE_BITMAP 4
// 块组描述符中 索引节点表 的偏移
#define OFFSET_GROUP_DESC_INODE_TABLE 8

#define OFFSET_INODE_SIZE 4
// 索引节点中 i_block 的偏移
#define OFFSET_INODE_BLOCKS 40

// 超级块大小
#define SUPER_BLOCK_SIZE 1024
// 块组描述符大小
#define GROUP_DESC_SIZE 16
// 索引节点大小
#define INODE_SIZE 128