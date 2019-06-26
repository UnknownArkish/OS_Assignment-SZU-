
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<math.h>

#include"const.h"
#include"util.h"
#include"ext2/SuperBlock.h"
#include"ext2/GroupDescription.h"
#include"ext2/INode.h"
#include"main.h"

int InitDEV();
void Clean();

// 块设备路径
const char* DEV_PATH = "/dev/loop1";

int fd_dev;

struct Super_Block super_block;                     // 超级块
struct GroupDescription* group_description = 0;     // 块组描述符数组

char buf[BUFFER_SIZE] = {0};

int GetINodeAddress( int inodeIndex ){
    int groupNumber = (inodeIndex - 1) / super_block.inodes_per_group;
    int tableOffset = ( inodeIndex - 1 ) % super_block.inodes_per_group;
    int result = group_description[groupNumber].inode_table * super_block.block_size + tableOffset * INODE_SIZE;
    return result;
}
void GetInode(int inodeIndex, struct INode* inode){
    int inodeAddress = GetINodeAddress(inodeIndex);
    lseek( fd_dev, inodeAddress, SEEK_SET );
    read( fd_dev, buf, INODE_SIZE );

    int tmep = GetContent(buf, 0, 4, LITTLE_ENDIAN);
    printf("temp:: %d\n", tmep);

    inode->size = GetContent( buf, OFFSET_INODE_SIZE, 4, LITTLE_ENDIAN);

    for( int i = 0 ; i < 15 ; i++ ){
        inode->block[i] = GetContent( buf, OFFSET_INODE_BLOCKS + i * 4, 4, LITTLE_ENDIAN );
    }
}

int main(int argc, char** argv){
    if( InitDEV() < 0){
        fprintf( stderr, "open file %s failed\n", DEV_PATH );
        return -1;
    }

    printf("inodes_count:: %d\n", super_block.inodes_count);
    printf("blocks_count:: %d\n", super_block.blocks_count);
    printf("block_size:: %d\n", super_block.block_size);
    printf("blocks_per_group:: %d\n", super_block.blocks_per_group);
    printf("inode_per_group:: %d\n", super_block.inodes_per_group);
    printf("block_groups_count:: %d\n", super_block.block_groups_count);
    for( int i = 0 ; i < super_block.block_groups_count; i++ ){
        printf("group: %d inode_table::%d\n", i, group_description[i].inode_table);
    }

    int inodeIndex = 15;
    printf("inode %d's address: %d\n", inodeIndex, GetINodeAddress(inodeIndex));
    struct INode inode;
    GetInode( inodeIndex, &inode );
    printf("inode %d's size: %d\n", inodeIndex, inode.size);
    for( int i = 0 ; i < 15 ; i++ ){
        printf("%d ", inode.block[i]);
    }
    printf("\n");

    Clean();
    return 0;
}


/*
    初始化超级块
*/
void InitSuperBlock(){
    // @TODO: 读取超级块的信息
    lseek( fd_dev, BOOT_SIZE, SEEK_SET );
    read( fd_dev, buf, SUPER_BLOCK_SIZE );

    super_block.inodes_count = GetContent( buf, OFFSET_SUPER_BLOCK_INODES_COUNT, 4, LITTLE_ENDIAN );
    super_block.blocks_count = GetContent( buf, OFFSET_SUPER_BLOCK_BLOCKS_COUNT, 4, LITTLE_ENDIAN );
    super_block.block_size = BLOCK_SIZE;
    super_block.blocks_per_group = GetContent( buf, OFFSET_SUPER_BLOCK_BLOCK_PER_GROUP, 4, LITTLE_ENDIAN );
    super_block.inodes_per_group = GetContent( buf, OFFSET_SUPER_BLOCK_INODE_PER_GROUP, 4, LITTLE_ENDIAN );

    int totalSize = super_block.blocks_count * super_block.block_size;
    int blockGroupSize = super_block.blocks_per_group * super_block.block_size;

    super_block.block_groups_count = ceil((double)(totalSize - BOOT_SIZE)/ blockGroupSize);
}

/*
    解析得到一个Groupescription
*/
void ParseGroupDescription( char buf[], struct GroupDescription* dest ){
    dest->block_bitmap = GetContent( buf, OFFSET_GROUP_DESC_BLOCK_BITMAP, 4, LITTLE_ENDIAN );
    dest->inode_bitmap = GetContent( buf, OFFSET_GROUP_DESC_INODE_BITMAP, 4, LITTLE_ENDIAN );
    dest->inode_table = GetContent( buf, OFFSET_GROUP_DESC_INODE_TABLE, 4, LITTLE_ENDIAN );
}
/*
    初始化所有的块组描述符
*/
void InitGroupDescription(){
    // 分配内存
    group_description = malloc( super_block.block_groups_count * sizeof( struct GroupDescription ) );
    // 跳到第二个块中
    lseek( fd_dev, 2 * super_block.block_size, SEEK_SET );

    for( int i = 0 ; i < super_block.block_groups_count ; i++ ){
        read( fd_dev, buf, GROUP_DESC_SIZE );
        ParseGroupDescription( buf, &group_description[i] );

        lseek( fd_dev, GROUP_DESC_SIZE, SEEK_CUR );
    }
}

/*
    初始化块设备
*/
int InitDEV(){
    // @TODO: 打开块设备
    fd_dev = open(DEV_PATH, O_RDONLY);
    if( fd_dev == -1 ){
        return -1;
    }

    // @TODO: 读取超级块的信息
    InitSuperBlock();
    // @TODO: 读取块组描述符
    InitGroupDescription();

    return 1;
}

/*
    进行清理
*/
void Clean(){
    // 释放指针
    if( group_description != NULL ){
        free(group_description);
    }
    // 关闭文件
    close(fd_dev);
}