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
    OAUTH2_GRANT_AUTH_CODE = 0,
    OAUTH2_GRANT_PASSWORD,
    OAUTH2_GRANT_ASSERTION,
    OAUTH2_GRANT_REFRESH
} oauth2_grant_type;

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

typedef struct _oauth2_config
{
    char* client_id;
    char* client_secret;
    char* auth_server_uri;
    char* redirect_uri;
    oauth2_error last_error;
} oauth2_config;

//Methods

//Initialiser
oauth2_config* oauth2_init(char* client, char* secret, char* auth_server_uri);

//Set the redirect URI for auth code authentication. This must be set before using oauth2_request_auth_code too.
void oauth2_set_redirect_uri(oauth2_config* conf, char* redirect_uri);

//Returns URL to redirect user to.
char* oauth2_request_auth_code(oauth2_config* conf, char* scope, char* state);
char* oauth2_access_auth_code(oauth2_config* conf, char* auth_code, char* scope);
char* oauth2_access_resource_owner(oauth2_config* conf, char* username, char* password);
char* oauth2_access_refresh_token(oauth2_config* conf, char* refresh_token);
char* oauth2_request(oauth2_config* conf, char* uri);  
void oauth2_cleanup(oauth2_config* conf);


#endif
