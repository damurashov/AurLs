//
// fs.h
//
// Created on: 2023-01-18
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(DMLS_FS_H_)
#define DMLS_FS_H_

typedef struct DmlsDirEntry {
	char flags[10];
	int n_hard_links;
	const char *owner_name[33];  // Max. length is 32 char-s
	const char *owner_group_name[33];  // Max. length is 32 char-s
	int file_size_bytes;
	const char *modification_timestring;
	const char *file_name;
} DmlsDirEntry;

typedef void (*DmlsOnIterDirectoryCb)(const DmlsDirEntry *dmls_dir_entry);

void printf_dmls_dir_entry(const DmlsDirEntry *dmls_dir_entry);

/// Iterates through files, links, and directories in a given directory (as
/// defined by `directory_name`). On each entry, an iterator will be invoked.
/// Returns `DmlsResultOk` (0), if successful.
int dmls_iter_directory(const char *directory_name, DmlsOnIterDirectoryCb it_cb);

#endif // DMLS_FS_H_
