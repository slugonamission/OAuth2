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

#include "curl_request.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <curl/curl.h>

size_t curl_callback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    size_t idx;
    size_t max;
    data* d;
    data* nd;
    
    d = (data*)userdata;

    idx = 0;
    max = nmemb * size;

    //Scan to the correct buffer
    while(d->next != NULL)
        d = d->next;

    //Store the data
    while(idx < max)
    {
        d->d[d->idx++] = ((char*)ptr)[idx++];

        if(d->idx == MAX_BUFFER)
        {
            nd = malloc(sizeof(data));
            nd->next = NULL;
            nd->idx = 0;
            d->next = nd;
            d = nd;
        }
    }

    return max;
}

void data_clean(data* d)
{
    data* pd;
    while(d)
    {
        pd = d->next;
        free(d);
        d = pd;
    }
}

char* curl_make_request(char* url, char* params)
{
    data* storage;
    data* curr_storage;
    CURL* handle;
    int data_len;
    char* retVal;

    assert(url != 0);
    assert(*url != 0);

    storage = malloc(sizeof(data));
    storage->idx = 0;
    storage->next = 0;

    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, curl_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, storage);

    //Do we need to add the POST parameters?
    if(params != NULL)
    {
        curl_easy_setopt(handle, CURLOPT_POST, 1);
        curl_easy_setopt(handle, CURLOPT_COPYPOSTFIELDS, params); //Copy them just incase
                                                                  //the user does something stupid
    }

    if(curl_easy_perform(handle) != 0)
    {
        //Error!
        curl_easy_cleanup(handle);
        data_clean(storage);
        return NULL;
    }

    //Everything went OK.
    //How long is the data?
    data_len = 0;
    curr_storage = storage;
    while(curr_storage)
    {
        data_len += curr_storage->idx;
        curr_storage = curr_storage->next;
    }

    //Allocate storage
    retVal = malloc(sizeof(char)*data_len);
    
    //Now copy in the data
    curr_storage = storage;
    data_len = 0;
    while(curr_storage)
    {
        memcpy(retVal+data_len, curr_storage->d, curr_storage->idx);
        curr_storage = curr_storage->next;
    }

    //Cleanup
    curl_easy_cleanup(handle);
    data_clean(storage);
    
    return retVal;
}
