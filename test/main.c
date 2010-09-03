#include <stdio.h>
#include "oauth2.h"

int main(int argc, char** argv)
{
    oauth2_config* conf = oauth2_init("foo", "bar", "http://www.google.com");
    oauth2_set_redirect_uri(conf, "http://qtcreative.co.uk");
    char* redir_uri = oauth2_request_auth_code(conf, "foo", "lol");

    printf("%s", redir_uri);

    oauth2_cleanup(conf);

    return 0;
}
