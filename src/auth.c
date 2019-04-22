// !c++
//      ------------------------------
//              MIT-License 0x7e3
//       <ergotamin.source@gmail.com>
//      ------------------------------
//
#include <string.h>
//
#define _HEADER_    "@HEADER@"
//
const char *get_token(void *buf)
{
#//HACK:
#include _HEADER_
#
    strcpy((char *)buf, (const char *)dstr);

    return (const char *)buf;
}  // end
# // of
// file
