/**
 *  showlinks: Extract URLs from a webpage
 *  Copyright (C) 2013  Matthijs van Drunen <wvdrunen@liacs.nl>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 **/

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <stdbool.h>

#include "htmlstreamparser.h"

CURLcode code;

struct my_web
{
  char *inner_text;
  int length;
}web_def = {NULL, 0};

typedef struct my_web *web_struct;

// Private functions
// -----------------
// static size_t write_cb(void *data, size_t size, size_t nmemb, void *userp)
// {
//     buffer_t *buffer = (buffer_t*) userp;
//     buffer_append(buffer, size * nmemb, data);
//     return size * nmemb;
// }

// Determine if url is absolute
static bool is_absolute(const char *url)
{
    // Based on W3's HTParse: check if there is a : before /, ? or #
    const char *ptr = url;
    while (*ptr) {
        if (*ptr == ':') return true;
        if (*ptr == '/' || *ptr == '?' || *ptr == '#') return false;
        ptr++;
    }
    return false;
}

// Extract the host part of an url
// This function returns a pointer to the path part of the original url
// Example:
// http://www.example.org:80/dir/file:
//                          ^       ^ inclusive
static const char *get_path(const char *url)
{
    const char *ptr = url;
    
    while (*ptr && *ptr != ':') {       // skip to :
        if (*ptr == '/') return NULL;
        ptr++;
    }
    if (*ptr == ':') ptr++;             // skip over :
    while (*ptr && *ptr == '/') ptr++;  // skip over /'s
    if (!*ptr) return NULL;
    while (*ptr && *ptr != '/') ptr++;  // skip to last / or end of string

    return ptr;
}

// Extract the host part of an url (including scheme)
// This function returns a copy of the host part of the url
// Example:
// http://www.example.org:80/dir/file:
// ^                       ^ inclusive
static char *get_host(const char *url)
{
    char *ret;
    const char *ptr = get_path(url);
    if (!ptr) return NULL;
    size_t len = ptr - url;
    ret = malloc(len + sizeof(char));
    memcpy(ret, url, len);
    ret[ptr - url] = '\0';
    return ret;
}

// Format URLs: convert relative to absolute URLs
static char *format_url(const char *url, const char *host, const char *path)
{
    char *ret;

    if (is_absolute(url)) {
        ret = strdup(url);
    } else if (url[0] == '/') {
        // Relative to domain
        size_t len = strlen(host) + strlen(url) + sizeof(char);
        ret = malloc(len);
        memset(ret, 0, len);
        strcat(ret, host);
        strcat(ret, url);
    } else {
        // Relative to directory
        size_t len = strlen(host) + strlen(path) + 
                         strlen(url) + 2*sizeof(char);
        ret = malloc(len);
        memset(ret, 0, len);
        strcat(ret, host);
        strcat(ret, path);
        strcat(ret, "/");
        strcat(ret, url);
    }

    return ret;
}

// Get the next attribute value
static inline char *GetAttrValue(HTMLSTREAMPARSER *hsp, char *tagname,
                                 char *attrname, size_t sz,
                                 const char *mywebpage, size_t *p)
{
    char *ret = NULL;
    while (*p < sz) {
        html_parser_char_parse(hsp, mywebpage[*p]);
        if (html_parser_cmp_tag(hsp, tagname, strlen(tagname))) {
            if (html_parser_cmp_attr(hsp, attrname, strlen(attrname))) {
                if (html_parser_is_in(hsp, HTML_VALUE_ENDED)) {
                    ret = html_parser_val(hsp);
                    ret[html_parser_val_length(hsp)] = '\0';
                    break;
                }
            }
        }
        (*p)++;
    }
    return ret;
}


static size_t write_callback(char *buffer, size_t size, size_t nmemb, web_struct web_page){ 
    
    if(nmemb == 0)
        return nmemb;
    
    //if we dont have anything yet
    if(web_page->length == 0){
        
        //allocate the space we need
        web_page->inner_text = malloc(nmemb + 1);
        
        //copy the data into our structure
        strncpy(web_page->inner_text, buffer, nmemb);
        web_page->inner_text[nmemb] = '\0';
    
    //if we do have data already 
    }else{
        //reallocate the memory of the array where we store the page
        web_page->inner_text = realloc(web_page->inner_text, web_page->length + nmemb + 1);
        //and append the new data we received
        strncat(web_page->inner_text, buffer, nmemb);
        web_page->inner_text[web_page->length + nmemb] = '\0';
    }

    //increment data length
    web_page->length += nmemb;  

    return size*nmemb;
} 


char *GetWebPage(char *myurl){
    
    web_struct web_page = &web_def;
    char errbuf[CURL_ERROR_SIZE];

    //if the structure is already full empty it
    if(web_page->length > 0){
        free(web_page->inner_text);
        web_page->length = 0;

    }

    /*curl_easy_init() initializes curl and this call must have a corresponding call to curl_easy_cleanup();*/   
    CURL *curl = curl_easy_init();

    /*tell curl the URL address we are going to download*/
    curl_easy_setopt(curl, CURLOPT_URL, myurl);
    
    /*Pass a pointer to the function write_callback( char *ptr, size_t size, size_t nmemb, void *userdata); write_callback gets called by libcurl as soon as there is data received, and we can process the received data, such as saving and weblinks extraction. */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    /*it tells the the pointer userdata argument in the write_callback function the data comes from hsp pointer*/
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, web_page);
    /*A parameter set to 1 tells the library to follow any Location: header that the server sends as part of a HTTP header.This means that the library will re-send the same request on the new location and follow new Location: headers all the way until no more such headers are returned.*/
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    
    /*allow curl to perform the action*/
    CURLcode curl_res = curl_easy_perform(curl);

    if(curl_res==0){ 
        printf("HTML file downloaded success\n"); 
    }
    
    /* else { 
        printf("ERROR in dowloading file\n"); 
        curl_easy_cleanup(curl); 
        return NULL;
    } */
    if(curl_res != CURLE_OK) {
    size_t len = strlen(errbuf);
    fprintf(stderr, "\nlibcurl: (%d) ", curl_res);
    if(len)
      fprintf(stderr, "%s%s", errbuf,
              ((errbuf[len - 1] != '\n') ? "\n" : ""));
    else
      fprintf(stderr, "%s\n", curl_easy_strerror(curl_res));
    return NULL; 
    }
    /*release all the previously allocated memory ,and it corresponds to the function curl_easy_init();*/
    curl_easy_cleanup(curl);

    web_page->inner_text = realloc(web_page->inner_text, web_page->length + 1);
    web_page->length++;
    strcat(web_page->inner_text, "\0");
    // web_page->inner_text[web_page->length -1] = '\0';


    return (web_page->inner_text);
}

char *GetLinksFromWebPage(char *myhtmlpage, char *myurl){
    int p;
    HTMLSTREAMPARSER *hsp = html_parser_init(); 
    char tag[2];
    char attr[5];
    char val[256];  
    html_parser_set_tag_to_lower(hsp, 1);   
    html_parser_set_attr_to_lower(hsp, 1); 
    html_parser_set_tag_buffer(hsp, tag, sizeof(tag));  
    html_parser_set_attr_buffer(hsp, attr, sizeof(attr));    
    html_parser_set_val_buffer(hsp, val, sizeof(val)-1); 

    char *results;
    int cur_size = 0;

    for (p = 0; p < strlen((const char*) myhtmlpage); p++) {       

        /*Parse the char specified by the char argument*/        
        html_parser_char_parse(hsp, ((char *)web_def.inner_text)[p]);
        
        /*Compares the tag name and the string “a”.The argument 1 is a 
        string length.Returns 1 for equality otherwise returns 0*/ 
        if (html_parser_cmp_tag(hsp, "a", 1)){  
            
            /*Compares the attribute name and the string “href”. The argument
             4 is a string length.Returns 1 for equality otherwise returns 0*/ 
            if (html_parser_cmp_attr(hsp, "href", 4)){ 
                
                /*Returns 1 if the parser is inside HTML_VALUE_ENDED part of HTML code.*/
                if (html_parser_is_in(hsp, HTML_VALUE_ENDED)){ 
                    
                    html_parser_val(hsp)[html_parser_val_length(hsp)] = '\0';

                    if(html_parser_val(hsp)[0] == '#')
                        continue;
                    
                    int is_relative_path = (html_parser_val(hsp)[0] == '/' ||
                        (strncmp(html_parser_val(hsp), "http://", 7) != 0 && strncmp(html_parser_val(hsp), "https://", 8) != 0)
                        );

                    //calculate the length of the link
                    //+2 is for '\0' and '\n' 
                    int link_size = (is_relative_path) ?
                        ((html_parser_val_length(hsp) + cur_size + 2) + strlen((const char*) myurl)) * sizeof(char) :
                        (html_parser_val_length(hsp) + cur_size + 2) *  sizeof(char);
                    
                    //check if we need to add a forward slash
                    if(is_relative_path && html_parser_val(hsp)[0] != '/'){
                        link_size++;
                    }

                    //if this is the first link
                    if(cur_size==0)
                        //allocate the needed space
                        results = malloc(link_size*sizeof(char));
                    else
                        //reallocate the memory with the new space required
                        results = realloc(results, link_size*sizeof(char));
                    
                    //if this is the first link
                    if (cur_size == 0){

                        //if it is relative path  
                        if(is_relative_path){
                            
                            //first copy our base URL
                            strcpy(results, myurl);
                            //append a forward slash if needed
                            if(html_parser_val(hsp)[0] != '/')
                              strncat(results, "/", 1);
                        }else{
                            strcpy(results, html_parser_val(hsp));
                        }

                    }else{
                        //if it is relative path
                        if(is_relative_path){
                            //first append our base URL
                            strncat(results, myurl, strlen((const char*) myurl));
                            //append a forward slash if needed
                            if(html_parser_val(hsp)[0] != '/')
                                strncat(results, "/", 1);
                        }
                        //and finally append the path
                        strncat(results, html_parser_val(hsp), html_parser_val_length(hsp));
                    }


                    //append a '\n' to separate the links
                    strncat(results, "\n", 1);

                    //increment the size
                    cur_size = link_size;
                }
            }
        }
    }

    if (cur_size == 0)
      return NULL;

    return results;
 }

const char *GetLastWebError()
{
    return curl_easy_strerror(code);
}
