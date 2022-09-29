/* See LICENSE file for copyright and license details. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "oauth2.h"
#include "curl_request.h"

oauth2_config* oauth2_init(char* client, char* secret)
{
    int input_strlen;
    oauth2_config* ret_val = malloc(sizeof(oauth2_config));

    if(ret_val == NULL)
        return NULL;

    //Copy in the client id etc
    input_strlen = strlen(client)+1;
    ret_val->client_id = malloc(input_strlen * sizeof(char));
    strcpy(ret_val->client_id, client);

    assert(ret_val->client_id[input_strlen-1] == '\0');

    input_strlen = strlen(secret)+1;
    ret_val->client_secret = malloc(input_strlen);
    strcpy(ret_val->client_secret, secret);

    assert(ret_val->client_secret[input_strlen-1] == '\0');

    ret_val->redirect_uri = NULL;

    //Clear the error
    ret_val->last_error.error = OAUTH2_ERROR_NO_ERROR;
    ret_val->last_error.error_description = NULL;
    ret_val->last_error.error_uri = NULL;
    ret_val->last_error.state = NULL;
    ret_val->auth_code = NULL;
	return ret_val;
}

void oauth2_set_redirect_uri(oauth2_config* conf, char* redirect_uri)
{
    int input_strlen;

    assert(conf != NULL);

    input_strlen = strlen(redirect_uri)+1;
    conf->redirect_uri = malloc(sizeof(char) * input_strlen);
    strcpy(conf->redirect_uri, redirect_uri);
}

void oauth2_set_auth_code(oauth2_config* conf, char* auth_code)
{
    int input_strlen;

    assert(conf != NULL);

    input_strlen = strlen(auth_code)+1;
    conf->auth_code = malloc(sizeof(char) * input_strlen);
    strcpy(conf->auth_code, auth_code);
}

char* oauth2_request_auth_code(oauth2_config* conf, char* auth_server, char* scope, char* state, char* access_type)
{
    int core_len;
    int scope_len;
    int state_len;
    int access_type_len;
    char* core_fmt;
    char* scope_fmt;
    char* state_fmt;
    char* access_type_fmt;
    char* final_str;

    scope_len = 1;
    state_len = 1;
    access_type_len = 1;

    assert(conf != NULL);

    //We just need to build the request string, since we can't actually handle the callback ourselves
    //URL Format: <server>?response_type=code&client_id=<client_id>&redirect_uri=<redir_uri>&scope=<scope>&state=<state>
    //Get the final length
    core_fmt = "%s?response_type=code&client_id=%s&redirect_uri=%s";
    scope_fmt = "&scope=%s";
    state_fmt = "&state=%s";
    access_type_fmt = "&access_type=%s";

    //Get the string lengths
    core_len = snprintf(NULL, 0, (const char*)core_fmt, auth_server, conf->client_id, conf->redirect_uri) + 1;
    if(scope != NULL)
        scope_len = snprintf(NULL, 0, (const char*)scope_fmt, scope) + 1;
    if(state != NULL)
        state_len = snprintf(NULL, 0, (const char*)state_fmt, state) + 1;
    if(access_type != NULL)
        access_type_len = snprintf(NULL, 0, (const char*)access_type_fmt, access_type) + 1;

    //Actually build the string
    final_str = malloc(((core_len-1)
                        +(scope_len-1)
                        +(state_len-1)
                        +(access_type_len-1) +1)*sizeof(char));

    sprintf(final_str, (const char*)core_fmt, auth_server, conf->client_id, conf->redirect_uri);
    if(scope != NULL)
        sprintf((char*)(final_str+(core_len-1)),
                (const char*)scope_fmt,
                scope);

    if(state != NULL)
        sprintf((char*)(final_str+(core_len-1)+(scope_len-1)),
                (const char*)state_fmt,
                state);

    if(access_type != NULL)
        sprintf((char*)(final_str+(core_len-1)+(scope_len-1)+(state_len-1)),
                (const char*)access_type_fmt,
                access_type);

    return final_str;
}

char* oauth2_access_auth_code(oauth2_config* conf, char* auth_server, char* auth_code, char* scope)
{
    //Build up the request
    char* uri;
    char* query_fmt;
    char* output;
    char* acc_code;
    int query_len;
    char* acc_pos_s;
    char* acc_pos_e;
    int acc_pos_len;

    assert(conf != NULL);
    assert(auth_server != NULL);
    assert(auth_code != NULL);

    query_fmt = "grant_type=authorization_code&client_id=%s&client_secret=%s&code=%s&redirect_uri=%s";

    query_len = snprintf(NULL, 0, query_fmt, conf->client_id, conf->client_secret, auth_code, conf->redirect_uri);
    uri = malloc(sizeof(char)*query_len);
    sprintf(uri, query_fmt, conf->client_id, conf->client_secret, auth_code, conf->redirect_uri);
    output = curl_make_request(auth_server, uri);
    free(uri);

    if (conf->access_auth_code_transformer != NULL) {
        return conf->access_auth_code_transformer(output);
    }

    //Strip out the access token
    #define __OAUTH2_ACCESS_TOKEN_STR "access_token="
    acc_pos_s = strstr(output, __OAUTH2_ACCESS_TOKEN_STR);
    if(acc_pos_s == NULL)
    {
        printf("%s\n", output);
        free(output);
        return NULL;
    }

    acc_pos_s += sizeof(__OAUTH2_ACCESS_TOKEN_STR); //Skip past access_token
    //Find the end of the token
    acc_pos_e = acc_pos_s;
    while(*acc_pos_e != '&' && *acc_pos_e != '\0') ++acc_pos_e;

    //Now extract it
    acc_pos_len = acc_pos_e - acc_pos_s;
    acc_code = malloc(sizeof(char) * acc_pos_len);

    memcpy(acc_code, acc_pos_s, acc_pos_len);
    free(output);

    return acc_code;
}

char* oauth2_access_resource_owner(oauth2_config* conf, char* auth_server, char* username, char* password)
{
    char* uri;
    char* query_fmt;
    char* output;
    int query_len;

    assert(conf != NULL);
    assert(auth_server != NULL);
    assert(username != NULL);
    assert(password != NULL);

    query_fmt = "grant_type=password&client_id=%s&username=%s&password=%s";

    //Get the length of the query
    query_len = snprintf(NULL, 0, query_fmt, conf->client_id, username, password);

    //Allocate space for it and request
    uri = malloc(query_len+1);

    sprintf(uri, query_fmt, conf->client_id, username, password);

    //Now make the request!
    output = curl_make_request(auth_server, uri);

    //Cleanup
    free(uri);

    return output;
}

char* oauth2_access_refresh_token(oauth2_config* conf, char* refresh_token)
{
    assert(0);
    return NULL;
}

char* oauth2_request(oauth2_config* conf, char* uri, char* params)
{
    //For now, we'll just include the access code with the request vars
    //This is discouraged, but I don't know if most providers actually
    //support the header-field method (Facebook is still at draft 0...)

    char* ret_val;
    char* uri2;
    int uri_len;

    //Sanity checks
    assert(conf != NULL);
    assert(conf->client_id != NULL);
    assert(conf->auth_code != NULL);
    assert(uri != NULL);

    //Are we POSTing?
    if(params != NULL)
    {
        //Attach the token to the params
        uri_len = snprintf(NULL, 0, "%s&access_token=%s", params, conf->auth_code);
        uri2 = malloc(sizeof(char)*uri_len);
        sprintf(uri2, "%s&access_token=%s", params, conf->auth_code);

        ret_val = curl_make_request(uri, uri2);
        free(uri2);
        return ret_val;
    }
    else
    {
        return NULL; //I'm not doing this now.
    }
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

    free(conf);
}
