// !c++
//      ------------------------------
//              MIT-License 0x7e3
//       <ergotamin.source@gmail.com>
//      ------------------------------
//
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
//
#if !defined(NULL)
#define NULL    0
#endif
//
extern const char *__progname;
extern int git_delete_repository(const char *repo);
extern int git_create_repository(const char *name, const char *description, const char *homepage);
//
unsigned int get_opt(const char *arg, int num, ...)
{
    int r = -1;
    va_list args;
    char *def = NULL;
    unsigned int opt = 0;

    va_start(args, num);
    ;
    for (--num; 0 <= num; num--) {
        ++opt;

        def = strdup(va_arg(args, const char *));
        r = strcmp(arg, def);
        free((void *)def);

        def = NULL;
        if (0 == r)
            break;
    }
    ;
    va_end(args);

    if (num == -1)
        return (unsigned int)num;
    else
        return opt;
}
//
int main(int argc __attribute__((unused)), char **argv)
{
    ++argv;
    if (*argv) {
        switch (get_opt(*argv, 2,
                        "create",
                        "delete")) {
        case 1U:
            ++argv;
            return git_create_repository(argv[0],
                                         NULL != argv[1] ? argv[1] : "",
                                         NULL != argv[2] ? argv[2] : "");

        case 2U:
            ++argv;
            return git_delete_repository(argv[0]);

        default:
            break;
        }
    }

    perror("wrong usage\n");
    return 1;
}
