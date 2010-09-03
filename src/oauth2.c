/*
Copyright (c) 2010 Jamie Garside

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "oauth2.h"

oauth2_config* oauth2_init(char* client, char* secret, char* auth_server_uri)
{
    int input_strlen;
    oauth2_config* retVal = malloc(sizeof(oauth2_config));

    if(retVal == NULL)
        return NULL;

    //Copy in the client id etc
    input_strlen = strlen(client)+1;
    retVal->client_id = malloc(input_strlen * sizeof(char));
    strcpy(retVal->client_id, client);

    assert(retVal->client_id[input_strlen-1] == '\0');

    input_strlen = strlen(secret)+1;
    retVal->client_secret = malloc(input_strlen);
    strcpy(retVal->client_secret, secret);

    assert(retVal->client_secret[input_strlen-1] == '\0');

    input_strlen = strlen(auth_server_uri)+1;
    retVal->auth_server_uri = malloc(input_strlen);
    strcpy(retVal->auth_server_uri, auth_server_uri);

    assert(retVal->auth_server_uri[input_strlen-1] == '\0');

    retVal->redirect_uri = NULL;

    //Clear the error
    retVal->last_error.error = OAUTH2_ERROR_NO_ERROR;
    retVal->last_error.error_description = NULL;
    retVal->last_error.error_uri = NULL;
    retVal->last_error.state = NULL;
}

void oauth2_set_redirect_uri(oauth2_config* conf, char* redirect_uri)
{
    int input_strlen;

    assert(conf != NULL);

    input_strlen = strlen(redirect_uri)+1;
    conf->redirect_uri = malloc(sizeof(char) * input_strlen);
    strcpy(conf->redirect_uri, redirect_uri);
}

char* oauth2_request_auth_code(oauth2_config* conf, char* scope, char* state)
{
    int core_len;
    int scope_len;
    int state_len;
    char* core_fmt;
    char* scope_fmt;
    char* state_fmt;
    char* final_str;

    scope_len = 1;
    state_len = 1;

    assert(conf != NULL);
    assert(conf->redirect_uri != NULL);

    //We just need to build the request string, since we can't actually handle the callback ourselves
    //URL Format: <server>?response_type=code&client_id=<client_id>&redirect_uri=<redir_uri>&scope=<scope>&state=<state>
    //Get the final length
    core_fmt = "%s?response_type=code&client_id=%s&redirect_uri=%s";
    scope_fmt = "&scope=%s";
    state_fmt = "&state=%s";

    //Get the string lengths
    core_len = snprintf(NULL, 0, (const char*)core_fmt, conf->auth_server_uri, conf->client_id, conf->redirect_uri) + 1;
    if(scope != NULL)
        scope_len = snprintf(NULL, 0, (const char*)scope_fmt, scope) + 1;
    if(state != NULL)
        state_len = snprintf(NULL, 0, (const char*)state_fmt, state) + 1;

    //Actually build the string
    final_str = malloc(((core_len-1)
                        +(scope_len-1)
                        +(state_len-1)+1)*sizeof(char));

    sprintf(final_str, (const char*)core_fmt, conf->auth_server_uri, conf->client_id, conf->redirect_uri);
    if(scope != NULL)
        sprintf((char*)(final_str+(core_len-1)),
                (const char*)scope_fmt,
                scope);

    if(state != NULL)
        sprintf((char*)(final_str+(core_len-1)+(scope_len-1)),
                (const char*)state_fmt,
                state);

    return final_str;
}

char* oauth2_access_auth_code(oauth2_config* conf, char* auth_code, char* scope)
{
    assert(0);
    return NULL;
}

char* oauth2_access_resource_owner(oauth2_config* conf, char* username, char* password)
{
    assert(0);
    return NULL;
}

char* oauth2_access_refresh_token(oauth2_config* conf, char* refresh_token)
{
    assert(0);
    return NULL;
}

char* oauth2_request(oauth2_config* conf, char* uri)
{
    assert(0);
    return NULL;
}

void oauth2_cleanup(oauth2_config* conf)
{
    if(conf == NULL)
        return;

    //Start freeing stuff up
    if(conf->client_id != NULL)
        free(conf->client_id);

    if(conf->client_secret != NULL)
        free(conf->client_secret);
    
    if(conf->auth_server_uri != NULL)
        free(conf->auth_server_uri);

    free(conf);
}
