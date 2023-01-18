#include "parser.h"
#include "dmls.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

static const char *s_error_wrong_arguments = "Wrong arguments. Usage `dmls -l DIRECTORY";

int dmls_arguments_init_with_sys(DmlsArguments *arguments, const char **args, int nargs)
{
	assert(arguments != NULL);
	int result = DmlsResultOk;

	if (nargs == 3) {
		if (strcmp(args[1], "-l") == 0) {
			arguments->flags = DmlsFlagsList;
		} else {
			result = DmlsResultError;
		}

		arguments->directory = args[2];
	} else {
		result = DmlsResultError;
	}

	if (result != 0) {
		dmls_set_error_message(s_error_wrong_arguments);
	}

	return result;
}
