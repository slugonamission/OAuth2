/* See LICENSE file for copyright and license details. */

#ifndef __OAUTH2_H__
#define __OAUTH2_H__

//Constants
#ifndef NULL
  #define NULL 0
#endif

typedef enum
{
    OAUTH2_RESPONSE_CODE = 0,
    OAUTH2_RESPONSE_TOKEN,
    OAUTH2_RESPONSE_TOKEN_AND_CODE
} oauth2_response_type;

typedef enum
{
    OAUTH2_ERROR_NO_ERROR = 0,
    OAUTH2_ERROR_INVALID_REQUEST,
    OAUTH2_ERROR_INVALID_CLIENT,
    OAUTH2_ERROR_UNAUTHORIZED_CLIENT,
    OAUTH2_ERROR_REDIRECT_URI_MISMATCH,
    OAUTH2_ERROR_ACCESS_DENIED,
    OAUTH2_ERROR_UNSUPPORTED_RESPONSE_TYPE,
    OAUTH2_ERROR_INVALID_SCOPE,
    OAUTH2_ERROR_INVALID_GRANT,
    OAUTH2_ERROR_UNSUPPORTED_GRANT_TYPE,
} oauth2_error_type;

//Internal structs
typedef struct _oauth2_error {
    oauth2_error_type error;
    char*             error_description;
    char*             error_uri;
    char*             state;
} oauth2_error;

typedef char*(*oauth2_response_handle)(char*);
typedef struct _oauth2_config
{
    char* client_id;
    char* client_secret;
    char* redirect_uri;
    char* auth_code;
    oauth2_error last_error;
    oauth2_response_handle access_auth_code_transformer;
} oauth2_config;

//Methods

//Initialiser
oauth2_config* oauth2_init(char* client, char* secret);

//Set the redirect URI for auth code authentication. This must be set before using oauth2_request_auth_code too.
void oauth2_set_redirect_uri(oauth2_config* conf, char* redirect_uri);
void oauth2_set_auth_code(oauth2_config* conf, char* auth_code);

//Returns URL to redirect user to.
char* oauth2_request_auth_code(oauth2_config* conf, char* auth_server, char* scope, char* state, char* access_type);
char* oauth2_access_auth_code(oauth2_config* conf, char* auth_server, char* auth_code, char* scope);
char* oauth2_access_resource_owner(oauth2_config* conf, char* auth_server, char* username, char* password);
char* oauth2_access_refresh_token(oauth2_config* conf, char* refresh_token);
char* oauth2_request(oauth2_config* conf, char* uri, char* params);
void oauth2_cleanup(oauth2_config* conf);


#endif
