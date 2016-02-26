// vfs.c - The Virtual Filesystem Driver

#include <fs/vfs.h>

uint32_t vfs_read(inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer){
	if (node->read != 0)
	{
		return node->read(node, offset, size, buffer);
	}
	else
	{
		return 0;
	}
}

uint32_t vfs_write(inode_t *node, uint32_t offset, uint32_t size, uint8_t *buffer){
	if(node->write != 0)
	{
		return node->write(node, offset, size, buffer);
	}
	else
	{
		return 0;
	}
}

void vfs_open(inode_t *node, uint8_t read, uint8_t write){
	if(node->open != 0)
	{
		return node->write(node, offset, size, buffer);
	}
	else
	{
		return 0;
	}
}

void vfs_close(inode_t *node){
	if(node->close != 0)
	{
		return node->close(node);
	}
	else
	{
		return 0;
	}
}

struct dirent *vfs_readdir(inode_t *node, uint32_t index){
	if((node->flags & 0x7 == VFS_DIR) && node->readdir != 0)
	{
		return node->readdir(node, index);
	}
	else
	{
		return 0;
	}
}

inode_t *vfs_finddir(inode_t *node, char *name){
	if((node->flags & 0x7 == VFS_DIR) && node->finddir != 0)
	{
		return node->finddir(node, name);
	}
	else
	{
		return 0;
	}
}
