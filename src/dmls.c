//
// dmls.c
//
// Created: 2023-01-17
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> geoscan.aero)
//

static const char *s_dmls_last_error_message = 0;

const char *dmls_last_error_message()
{
	return s_dmls_last_error_message;
}

void dmls_set_error_message(const char *err)
{
	s_dmls_last_error_message = err;
}
