//
// fs.c
//
// Created: 2023-01-18
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> geoscan.aero)
//

#include "dmls.h"
#include "fs.h"
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

static void dmls_dir_entry_init_with_stat(DmlsDirEntry *dmls_dir_entry, struct stat *st);

int dmls_iter_directory(const char *directory_name, DmlsOnIterDirectoryCb it_cb)
{
	DIR *dir;
	int result = DmlsResultOk;
	dir = opendir(directory_name);

	if (dir != NULL) {
		if (chdir(directory_name) == 0) {  // Do `chdir`, so we don't have to concatenate strings in order to provide `stat()` w/ the complete path
			struct dirent *drnt;

			while ((drnt = readdir(dir))) {  // Iterate through files
				struct stat st;

				if (lstat(drnt->d_name, &st) == 0) {
					DmlsDirEntry dmls_dir_entry;
					dmls_dir_entry_init_with_stat(&dmls_dir_entry, &st);
				}
			}

		} else {
			result = DmlsResultError;
			dmls_set_error_message("Unable to chdir");
		}
	} else {
		dmls_set_error_message("Unable to open directory");
	}

	return result;
}

static void dmls_dir_entry_init_with_stat(DmlsDirEntry *dmls_dir_entry, struct stat *st)
{
	(void)dmls_dir_entry;
	(void)st;
}
