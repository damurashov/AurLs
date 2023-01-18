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
#include <stdint.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

static struct PermissionInferenceRule {
	int32_t flags;
	int pos;
	char letter;
} s_permission_inference_rules[] = {
	{
		S_IRUSR,
		1,
		'r',
	},
	{
		S_IWUSR,
		2,
		'w',
	},
	{
		S_IXUSR,
		3,
		'x',
	},
	{
		S_IXUSR | S_ISUID,
		3,
		's',
	},
	{
		S_ISUID,
		3,
		'S',
	},
	{
		S_IRGRP,
		4,
		'r',
	},
	{
		S_IWGRP,
		5,
		'w',
	},
	{
		S_IXGRP,
		6,
		'x',
	},
	{
		S_IXGRP | S_ISGID,
		6,
		's',
	},
	{
		S_ISUID,
		6,
		'S',
	},
	{
		S_IROTH,
		7,
		'r',
	},
	{
		S_IWOTH,
		8,
		'w',
	},
	{
		S_IXOTH,
		9,
		'x',
	},
	{
		S_IXOTH | S_ISVTX,
		9,
		't',
	},
	{
		S_ISVTX,
		9,
		'T',
	},
};
static const int n_permission_inference_rules = sizeof(
	s_permission_inference_rules) / sizeof(s_permission_inference_rules[0]);

static int dmls_dir_entry_init_with_stat(DmlsDirEntry *dmls_dir_entry, struct stat *st);

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
					memset(&dmls_dir_entry, 0, sizeof(dmls_dir_entry));
					dmls_dir_entry.file_name = drnt->d_name;

					if (dmls_dir_entry_init_with_stat(&dmls_dir_entry, &st) == DmlsResultOk) {
						it_cb(&dmls_dir_entry);
					}
				}
			}

		} else {
			result = DmlsResultError;
			dmls_set_error_message("Unable to chdir");
		}
	} else {
		dmls_set_error_message("Unable to open directory");
		result = DmlsResultError;
	}

	return result;
}

void printf_dmls_dir_entry(const DmlsDirEntry *dmls_dir_entry)
{
	char time_buf[64] = {0};
	strftime((char *)time_buf, sizeof(time_buf), "%D %T",
		gmtime(&dmls_dir_entry->modification_time_seconds));
	printf("%s  %d  %s  %s  %d  %s  %s\n", dmls_dir_entry->flags,
		dmls_dir_entry->n_hard_links,
		(char *)dmls_dir_entry->owner_name,
		(char *)dmls_dir_entry->owner_group_name,
		dmls_dir_entry->file_size_bytes,
		time_buf,
		dmls_dir_entry->file_name);
}

static int dmls_dir_entry_init_with_stat(DmlsDirEntry *dmls_dir_entry, struct stat *st)
{
	int result = DmlsResultOk;

	// Init "permissions" field
	{
		// Init w/ placeholders
		for (int i = 0; i < sizeof(dmls_dir_entry->flags); ++i) {
			dmls_dir_entry->flags[i] = '-';
		}

		for (int i = 0; i < n_permission_inference_rules; ++i) {
			if ((s_permission_inference_rules[i].flags & st->st_mode)
					== s_permission_inference_rules[i].flags) {
				dmls_dir_entry->flags[s_permission_inference_rules[i].pos]
					= s_permission_inference_rules[i].letter;
			}
		}

		// Dir, link, or file. Separate bypass, because the bitmasks are platform-dependent
		if (S_ISDIR(st->st_mode)) {
			dmls_dir_entry->flags[0] = 'd';
		} else if (S_ISLNK(st->st_mode)) {
			dmls_dir_entry->flags[0] = 'l';
		}
	}

	dmls_dir_entry->n_hard_links = st->st_nlink;
	dmls_dir_entry->file_size_bytes = st->st_size;
	dmls_dir_entry->modification_time_seconds = st->st_mtime;

	// Init owner name
	{
		struct passwd *psswd = getpwuid(st->st_uid);

		if (psswd == NULL) {
			result = DmlsResultError;
			dmls_set_error_message("Could not retrieve username");
		} else {
			snprintf((char *)dmls_dir_entry->owner_name,
				sizeof(dmls_dir_entry->owner_name), "%s", psswd->pw_name);
		}
	}

	// Init the owner group's name
	{
		struct group *gr = getgrgid(st->st_uid);

		if (gr == NULL) {
			result = DmlsResultError;
			dmls_set_error_message("Could not retrieve group name");
		} else {
			snprintf((char *)dmls_dir_entry->owner_group_name,
				sizeof(dmls_dir_entry->owner_group_name), "%s", gr->gr_name);
		}
	}

	return result;
}
