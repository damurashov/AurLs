//
// parser.h
//
// Created: 2023-01-17
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> geoscan.aero)
//

#ifndef DMLS_PARSER_H_
#define DMLS_PARSER_H_

typedef enum DmlsFlags {
	DmlsFlagsList = 1 << 0,
} DmlsFlags;

/// Result of argument parsing
typedef struct DmlsArguments {
	const char *directory;
	int flags;
} DmlsArguments;

/// `arguments` - output variable
/// Returns 0, if successful.
int dmls_arguments_init_with_sys(DmlsArguments *arguments, const char **textargs, int nargs);

#endif  // DMLS_PARSER_H_
