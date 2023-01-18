#include "parser.h"
#include "dmls.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

int dmls_arguments_init_with_sys(DmlsArguments *arguments, const char **args, int nargs)
{
	assert(arguments != NULL);
	int result = 0;

	if (nargs == 3) {
		if (strcmp(args[1], "-l") == 0) {
			arguments->flags = DmlsFlagsList;
		} else {
			dmls_set_error_message("Wrong arguments. Usage `dmls -l DIRECTORY");
			result = 1;
		}

		arguments->directory = args[2];
	}

	return result;
}
