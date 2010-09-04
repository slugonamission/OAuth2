#include <stdio.h>
#include "oauth2.h"

int main(int argc, char** argv)
{
    oauth2_config* conf = oauth2_init("client_id", "client_secret");
    oauth2_set_redirect_uri(conf, "http://notarealhosthonest.com/");
    char* redir_uri = oauth2_request_auth_code(conf, "https://graph.facebook.com/oauth/authorize", "publish_stream", "LOL");

    printf("%s\n", redir_uri);

    //Now test token based auth
    char* code = "received_token";
    char* rv = oauth2_access_auth_code(conf, "https://graph.facebook.com/oauth/access_token", code, NULL);

    oauth2_set_auth_code(conf, rv);
    printf("%s\n", rv);
    free(rv);
    
    rv = oauth2_request(conf, "https://graph.facebook.com/slugonamission/feed", "message=Hello From oauth2!");

    printf("%s\n", rv);

    oauth2_cleanup(conf);

    return 0;
}
