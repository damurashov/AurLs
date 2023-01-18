#include "fs.h"
#include "parser.h"
#include "dmls.h"
#include <stdio.h>

int main(int argc, const char **argv)
{
	DmlsArguments dmls_arguments;
	if (dmls_arguments_init_with_sys(&dmls_arguments, argv, argc) == DmlsResultOk) {
	} else {
		printf("Error: %s", dmls_last_error_message());
	}
}
