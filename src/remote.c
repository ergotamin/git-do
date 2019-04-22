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
#include <curl/curl.h>
//
#if !defined(GITUSER)
#error "You have to define 'GITUSER' to compile."
#endif
//
#if !defined(NULL)
#define NULL    0
#endif
// the USER_AGENT-string submitted on requests.
#define USER_AGENT "Meta/Linux" // <-FIXME: change to whatever
// NOTE: defuscate the obfuscated token.
// (token-object gets 'secure'-compiled)
extern const char *get_token(void *buf);
// form the JSON-data, sent by git_create_repo(..).
static void *json_create_repo(const char	*name,
                              const char	*description,
                              const char	*homepage)
{
    const char *json_tmpl =
        u8R"json({
"name": "%s",
"description": "%s",
"homepage": "%s",
"private": false,
"has_issues": true,
"has_projects": false,
"has_wiki": false
})json";
    unsigned long len = strlen(json_tmpl)
                        + strlen(name)
                        + strlen(description)
                        + strlen(homepage)
                        + (1 << 2);
    void *data = calloc(++len, sizeof(unsigned char));

    snprintf(((char *)data), len, json_tmpl,
             name, description,
             homepage ? homepage : "www.metasrc.tk");
    return data;
}
// build the 'Authorization' header
const char *get_auth_header(void *json_buf, void *auth_buf)
{
    snprintf((char *)json_buf, 512, "%s%s",
             "Authorization: token ",
             get_token(auth_buf));

    return (const char *)json_buf;
}
// build the required custom-headers.
struct curl_slist *build_headers(void *json_buf, void *auth_buf)
{
    struct curl_slist *header = NULL;

    // explicitly request APIv3
    header = curl_slist_append(header, "Accept: application/vnd.github.v3+text");
    // JSON payload
    header = curl_slist_append(header, "Content-Type: application/json");
    // OAUTH-token
    header = curl_slist_append(header, get_auth_header(json_buf, auth_buf));

    return header;
}
// used to suppress printing response to stdout.
unsigned long read_cb_fun(void			*dst __attribute__((unused)),
                          unsigned long sz,
                          unsigned long nm,
                          void			*ptr __attribute__((unused)))
{
    return sz * nm;
}
// create a repository with description.
int git_create_repository(const char	*name,
                          const char	*description,
                          const char	*homepage)
{
    CURL *curl;
    CURLcode res = 0;
    void *data = NULL;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        data = json_create_repo(name, description, homepage);
        printf("%s\n", (char *)data);
        res += curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
        res += curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/user/repos");

        res += curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        res += curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res += curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_cb_fun);
        res += curl_easy_setopt(curl, CURLOPT_READDATA, NULL);

        res += curl_easy_setopt(curl, CURLOPT_HTTPHEADER, build_headers(alloca(512), alloca(512)));

        res += curl_easy_setopt(curl, CURLOPT_POSTFIELDS, (char *)data);


        if (CURLE_OK != res) {
            perror("failure : a curl_easy_setopt() call failed.\n");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            free(data);
            return 1;
        }

        res = curl_easy_perform(curl);

        free(data);

        if (CURLE_OK != res) {
            fprintf(stderr,
                    "failure : %s\n",
                    curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 1;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}
// delete a repository remotely.
int git_delete_repository(const char *repo)
{
    CURL *curl;
    CURLcode res = 0;
    void *data = NULL;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        data = calloc(512, sizeof(char *));

        snprintf((char *)data, 512, "%s%s%s",
                 "https://api.github.com/repos/",
                 GITUSER "/",
                 repo);

        res += curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
        res += curl_easy_setopt(curl, CURLOPT_URL, (char *)data);

        res += curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        res += curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res += curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_cb_fun);
        res += curl_easy_setopt(curl, CURLOPT_READDATA, (void *)NULL);

        res += curl_easy_setopt(curl, CURLOPT_HTTPHEADER, build_headers(alloca(512), alloca(512)));
        res += curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

        if (CURLE_OK != res) {
            perror("failure : a curl_easy_setopt() call failed.\n");
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            free(data);
            return 1;
        }

        res = curl_easy_perform(curl);

        free(data);

        if (CURLE_OK != res) {
            fprintf(stderr,
                    "failure : %s\n",
                    curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 1;
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return 0;
}
