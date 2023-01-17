//
// dmls.h
//
// Created on: 2023-01-17
//     Author: Dmitry Murashov (dmtr <DOT> murashov <AT> <GMAIL>)
//

#if !defined(DMLS_DMLS_H_)
#define DMLS_DMLS_H_

const char *dmls_last_error_message();

/// Sets static variable containing an error message.
/// `err` supposed to have static lifetime.
void dmls_set_error_message(const char *err);

#endif // DMLS_DMLS_H_
