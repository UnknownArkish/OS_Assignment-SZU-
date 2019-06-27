
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<math.h>
#include<string.h>

#include"const.h"
#include"util.h"
#include"ext2/SuperBlock.h"
#include"ext2/GroupDescription.h"
#include"ext2/INode.h"
#include"ext2/DirEntry.h"
#include"main.h"

int InitDEV();
int InitEnv();
void Clean();

int fd_dev;                                         // 块设备文件
struct Super_Block super_block;                     // 超级块
struct GroupDescription* group_description = 0;     // 块组描述符数组

char** path_split;
int path_count;

// 初始化并切割输入的路径
void InitPathSplit( char* input ){
    path_count = 0;
    for( int i = 0 ; input[i] != '\0' ;  ){
        if( input[i] == '/' ){
            int len = 0;
            for( int j = i + 1 ; input[j] != '\0' && input[j] != '/' ; j++ ){ len++; }
            memcpy( path_split[path_count], input + i + 1, len );
            path_split[path_count][len] = '\0';
            path_count++;
            i += (len + 1);
        }
    }
}

void SearchPath( char* path ){
    // @TODO: 使用路径更新path_split
    InitPathSplit( path );

    int curInodeIndex = ROOT_INODE_INDEX;
    struct INode curInode;                      // 当前搜索的索引节点
    struct DirEntry curDirEntry[256];           // 当前索引节点下的目录项
    int curDirEntryCount;                       // 当前索引节点下的目录项个数

    int curPathIndex = 0;                       // 当前层级路径下标
    struct DirEntry result;                     // 找到的目录项

    do{
        char* name = path_split[curPathIndex];
        if( strcmp(name, "") == 0 ) strcpy( name, "." );

        GetInode( curInodeIndex, &curInode );
        ParseInodeIntoDirEntry( curInode, curDirEntry, &curDirEntryCount );

        int searchResult = SearchDirEntry( &result, curDirEntry, curDirEntryCount, path_split[curPathIndex] );
        if( searchResult == 0 ) break;

        // 如果不是最后一个，并且找到的不是目录文件，则break
        if( (curPathIndex != path_count - 1 ) && result.file_type != EXT2_FT_DIR ){
            break;
        }else{
            curInodeIndex = result.inode_index;
        }
        curPathIndex++;
    }while( curPathIndex < path_count );

    if( curPathIndex == path_count ){
        // 如果是文件夹，输出文件夹目录项
        if( EXT2_FT_DIR == result.file_type ){
            HandleDirectory(result.inode_index);
        }else if( EXT2_FT_SYMLINK == result.file_type ){
            HandleLinkFile(result.inode_index);
        }else{
            HandleOtherFile(result.inode_index);
        }
    }else{
        fprintf(stderr, "error: does not exist such a file or directory: %s\n", path);
    }
}

void PrintDirEntry( struct DirEntry* src ){
    struct INode inode;
    GetInode(src->inode_index, &inode);
    printf("%10s %8d B  %-16s\n", file_type_name[src->file_type], inode.size, src->name);
}
void HandleDirectory( int inodeIndex ){
    struct INode inode;
    GetInode( inodeIndex, &inode );

    struct DirEntry dirEntry[256];
    int dirEntryCount = 0;
    ParseInodeIntoDirEntry( inode, dirEntry, &dirEntryCount );
    for( int i = 0 ; i < dirEntryCount; i++ ){
        PrintDirEntry( &dirEntry[i] );
    }
}
void HandleLinkFile( int inodeIndex ){
    struct INode inode;
    GetInode(inodeIndex, &inode);

    char path[inode.size];
    // 软链接中，如果大小小于60B，可以直接存储在i_block中
    if( inode.size <= 60 ){
        int offset = 0;
        for( offset = 0 ; offset < inode.size; offset += 4 ){
            int blockIndex = offset / 4;
            memcpy( path + offset, &inode.block[blockIndex], 4 );
        }
    }else{
        GetInodeContent( inode, path, inode.size );
    }
    SearchPath( path );
}
void HandleOtherFile(int inodeIndex ){
    struct INode inode;
    GetInode( inodeIndex, &inode );

    char content[inode.size+1];
    GetInodeContent( inode, content, inode.size );
    content[inode.size] = '\0';
    puts(content);
}

int SearchDirEntry( struct DirEntry* dest,  struct DirEntry src[], int count, char name[] ){
    for( int i = 0 ; i < count ; i++ ){
        if( strcmp( src[i].name, name ) == 0 ){
            dest->file_type = src[i].file_type;
            dest->inode_index = src[i].inode_index;
            dest->name_len = src[i].name_len;
            dest->rec_len = src[i].rec_len;
            strcpy( dest->name, src[i].name );
            return 1;
        }
    }
    return 0;
}




int main(int argc, char** argv){
    if( argv[1] == 0 ){
        printf("path lost...!\n");
        printf("usage: ./mian <path>\n");
        return -1;
    }

    if( InitDEV() < 0){
        fprintf( stderr, "open file %s failed\n", DEV_PATH );
        return -1;
    }
    
    if( InitEnv() < 0 ){
        fprintf( stderr, "error with InitEnv" );
    }else{
        SearchPath( argv[1] );
    }
    Clean();
    return 0;
}



int GetINodeAddress( int inodeIndex ){
    int groupNumber = (inodeIndex - 1) / super_block.inodes_per_group;
    int tableOffset = ( inodeIndex - 1 ) % super_block.inodes_per_group;
    int result = group_description[groupNumber].inode_table * super_block.block_size + 
        tableOffset * INODE_SIZE;
    return result;
}
void GetInode(int inodeIndex, struct INode* inode){
    int inodeAddress = GetINodeAddress(inodeIndex);
    char buf[INODE_SIZE];

    lseek( fd_dev, inodeAddress, SEEK_SET );
    read( fd_dev, buf, INODE_SIZE );

    // 得到文件长度和i_block属性
    inode->size = GetContent( buf, OFFSET_INODE_SIZE, 4, LITTLE_ENDIAN);
    for( int i = 0 ; i < 15 ; i++ ){
        inode->block[i] = GetContent( buf, OFFSET_INODE_BLOCKS + i * 4, 4, LITTLE_ENDIAN );
    }
}


int GetBlockDegreeSize( int blockDegree ){
    return pow( 256, blockDegree ) * super_block.block_size;
}

void GetBlockContent( int blockIndex, char buf[] ){
    int blockAdress = blockIndex * super_block.block_size;
    lseek( fd_dev, blockAdress, SEEK_SET );
    read( fd_dev, buf, super_block.block_size );
}

int GetBlockDegreeContent( int blockIndex, int blockDegree, char buf[] ){
    char temp[super_block.block_size];
    GetBlockContent( blockIndex, temp );
    
    if( blockDegree == 0 ){
        memcpy( buf, temp, super_block.block_size );
    }else{
        int totalSize = GetBlockDegreeSize( blockDegree );
        int childSize = GetBlockDegreeSize( blockDegree - 1 );

        char result[totalSize];                     // 此层级内容的缓存
        char child[childSize];                      // 子层级内容的缓存
        memset( result, 0, totalSize );
        for( int i = 0 ; i < 256 ; i++ ){
            int childAdress = GetContent( temp, i * 4, 4, LITTLE_ENDIAN );
            if( childAdress == 0 )  break;
            // 获得子层级的文件内容
            GetBlockDegreeContent( childAdress, blockDegree - 1, child );
            // 将子层级的内容拷贝到此层级上
            memcpy( result + i * childSize, child, childSize );
        }
        memcpy( buf, result, totalSize );
    }
}


void GetInodeContent( struct INode inode, char buf[], int size ){
    int degree;
    int degreeBlockSize;

    int degreeCounter = 0;
    int offset = 0;

    while( size > 0 ){
        if( degreeCounter >= 0 && degreeCounter < 12 ){
            degree = 0;
        }else{
            degree = degreeCounter - 11;
        }
        degreeBlockSize = GetBlockDegreeSize(degree);

        char temp[degreeBlockSize];
        GetBlockDegreeContent( inode.block[degreeCounter], degree, temp );

        int cpySize = size > degreeBlockSize? degreeBlockSize: size;
        memcpy( buf + offset, temp, cpySize );

        size -= cpySize;
        offset += cpySize;
        degreeCounter++;
    }
}


void ParseInodeIntoDirEntry( struct INode inode, struct DirEntry dirEntry[], int* dirEntryCount){
    char inodeContent[inode.size];
    GetInodeContent( inode, inodeContent, inode.size );

    int offset = 0;
    int count = 0;
    while( offset < inode.size ){
        ParseDirEntry( inodeContent + offset, &dirEntry[count]);
        // 偏移加上此目录项的长度
        offset += dirEntry[count].rec_len;
        count++;
    }
    *dirEntryCount = count;
}
void ParseDirEntry( char buf[], struct DirEntry* dirEntry ){
    dirEntry->inode_index = GetContent( buf, OFFSET_DIR_ENTRY_INODE, 4, LITTLE_ENDIAN );
    dirEntry->rec_len = GetContent( buf, OFFSET_DIR_ENTRY_REC_LEN, 2, LITTLE_ENDIAN );
    dirEntry->name_len = GetContent( buf, OFFSET_DIR_ENTRY_NAME_LEN, 1, LITTLE_ENDIAN );
    dirEntry->file_type = GetContent( buf, OFFSET_DIR_ENTRY_FILE_TYPE, 1, LITTLE_ENDIAN );
    memcpy( dirEntry->name, buf + OFFSET_DIR_ENTRY_FILE_NAME, dirEntry->name_len );
    dirEntry->name[dirEntry->name_len] = '\0';
}


/*
    初始化超级块
*/
void InitSuperBlock(){
    char buf[SUPER_BLOCK_SIZE];

    // @TODO: 读取超级块的信息
    lseek( fd_dev, BOOT_SIZE, SEEK_SET );
    read( fd_dev, buf, SUPER_BLOCK_SIZE );

    super_block.inodes_count = GetContent( buf, OFFSET_SUPER_BLOCK_INODES_COUNT, 4, LITTLE_ENDIAN );
    super_block.blocks_count = GetContent( buf, OFFSET_SUPER_BLOCK_BLOCKS_COUNT, 4, LITTLE_ENDIAN );
    super_block.block_size = BLOCK_SIZE;
    super_block.blocks_per_group = GetContent( buf, OFFSET_SUPER_BLOCK_BLOCK_PER_GROUP, 4, LITTLE_ENDIAN );
    super_block.inodes_per_group = GetContent( buf, OFFSET_SUPER_BLOCK_INODE_PER_GROUP, 4, LITTLE_ENDIAN );

    // 计算有多少个块组
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
    char buf[GROUP_DESC_SIZE];

    // 分配内存
    group_description = malloc( super_block.block_groups_count * sizeof( struct GroupDescription ) );
    
    // 跳到第二个块中
    lseek( fd_dev, 2 * super_block.block_size, SEEK_SET );
    for( int i = 0 ; i < super_block.block_groups_count ; i++ ){
        read( fd_dev, buf, GROUP_DESC_SIZE );
        ParseGroupDescription( buf, &group_description[i] );
        // 移动到下一个块组描述符的位置
        lseek( fd_dev, GROUP_DESC_SIZE, SEEK_CUR );
    }
}

int InitEnv(){
    path_split = calloc( MAXIMUM_SPERATOR_COUNT, sizeof(char*) );
    if( path_split == 0 )   return -1;
    for( int i = 0 ; i < MAXIMUM_SPERATOR_COUNT; i++ ){
        path_split[i] = calloc( MAXIMUM_FILE_NAME, sizeof(char) );
        if( path_split[i] == 0 )   return -1;
    }
    return 1;
}

/*
    初始化块设备
*/
int InitDEV(){
    // 打开块设备
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
    if( path_split != NULL ){
        for( int i = 0 ; i < MAXIMUM_SPERATOR_COUNT ; i++ ){
            if( path_split[i] != NULL ){
                free(path_split[i]);
                path_split[i] = NULL;
            }
        }
        free(path_split);
        path_split = NULL;
    }

    // 关闭文件
    close(fd_dev);
};