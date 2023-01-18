#include "fs.h"
#include "parser.h"
#include "dmls.h"
#include <stdio.h>

int main(int argc, const char **argv)
{
	DmlsArguments dmls_arguments;

	if (dmls_arguments_init_with_sys(&dmls_arguments, argv, argc) == DmlsResultOk) {
		if (dmls_arguments.flags & DmlsFlagsList) {
			if (dmls_iter_directory(dmls_arguments.directory,
					printf_dmls_dir_entry) != DmlsResultOk) {
				printf("Error: %s", dmls_last_error_message());
			}
		}
	} else {
		printf("Error: %s", dmls_last_error_message());
	}
}
