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

    input_strlen = strlen(secret)+1;
    retVal->client_secret = malloc(input_strlen);
    strcpy(retVal->client_secret, secret);

    input_strlen = strlen(auth_server_uri)+1;
    retVal->auth_server_uri = malloc(input_strlen);
    strcpy(retVal->auth_server_uri, auth_server_uri);

    //Clear the error
    retVal->last_error.error = OAUTH2_ERROR_NO_ERROR;
    retVal->last_error.error_description = NULL;
    retVal->last_error.error_uri = NULL;
    retVal->last_error.state = NULL;
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
