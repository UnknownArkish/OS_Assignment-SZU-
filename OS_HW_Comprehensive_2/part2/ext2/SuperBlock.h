

struct Super_Block{
    int inodes_count;               // 索引节点数量
    int blocks_count;               // 块数量
    int block_size;                 // 块大小

    int blocks_per_group;           // 块每块组
    int inodes_per_group;           // 索引节点每块组
    
    int block_groups_count;        // 块组数量
};