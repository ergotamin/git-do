// !c++
//      ------------------------------
//              MIT-License 0x7e3
//       <ergotamin.source@gmail.com>
//      ------------------------------
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
//
#if !defined(GITUSER)
#error "You have to define 'GITUSER' to compile."
#endif
//
#if !defined(NULL)
#define NULL    0
#endif
// NOTE: defuscate the obfuscated token.
// (token-object gets 'secure'-compiled)
extern const char *get_token(void *buf);
//
int print_credentials()