// vfs.h - Virtual File System Structures and Types
#include <stdint.h>

// Directory Entry
struct dirent // One of these is returned by the readdir call, according to POSIX.
{
  char name[128]; // Filename.
  u32int ino;     // Inode number. Required by POSIX.
};


// Function Typedefs
typedef uint32_t (*vfs_read_t)(inode_t*, uint32_t, uint32_t, uint8_t);
typedef uint32_t (*vfs_write_t)(inode_t*, uint32_t, uint32_t, uint8_t);
typedef void (*vfs_open_t)(inode_t*);
typedef void (*vfs_close_t)(inode_t*);
typedef struct dirent* (*vfs_readdir_t)(inode_t*, uint32_t);
typedef inode_t* (*vfs_finddir_t)(inode_t*, char *name);

// Filesystem Node
typedef struct inode{
    uint64_t inum;
    uint32_t mask;
    uint32_t uid;
    uint32_t gid;
    uint32_t flags;
    uint32_t filesystem;
    uint64_t size;
    vfs_read_t read;
    vfs_write_t write;
    vfs_open_t open;
    vfs_close_t close;
    vfs_readdir_t readdir;
    vfs_finddir_t finddir;
    inode_t* inode;
}
inode_t;

// Flags for FS Nodes
#define VFS_FILE 0x01
#define VFS_DIR 0x02
#define VFS_CHARDEV 0x03
#define VFS_BLOCKDEV 0x04
#define VFS_PIPE 0x05
#define VFS_SYMLINK 0x06
#define VFS_MOUNTPOINT 0x08


// Root
extern inode_t* vfs_root;


// Function Definitions VFS Functions
uint32_t vfs_read(inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
uint32_t vfs_write(inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer);
void vfs_open(inode_t *node, uint8_t read, uint8_t write);
void vfs_close(inode_t *node);
struct dirent *vfs_readdir(inode_t *node, uint32_t index);
inode_t *vfs_finddir(inode_t *node, char *name);