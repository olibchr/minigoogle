#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "web.h"

#define MAXQSIZE 9000000       // Maximum size of the queue, q
#define MAXURL 100000          // Maximum size of a URL
#define MAXPAGESIZE 20000          // Maximum size of a webpage
#define MAXDOWNLOADS 2000      // Maximum number of downloads we will attempt
#define MAXWORDLEN 100          // Maximum length of a word to store      

//
//  This code is a framework for a basic web spider.  It uses functions written
//  in a previous assignment.  
//
//  Compile: make
//
//  Run: webspider http://www.leidenuniv.nl


char *ShiftP(char *p, char *q)
//
//  This function returns the shifted position of p to the next URL
//  We also pass the queue so that we can check for the end of the queue
//
{
  
  int k;
  for(k=0;k<MAXURL;k++)
  {
    if(p[k]=='\n') {p = p + k + 1;
                    return(p);}
  }
      
  return(0);
}


int deposit (char *tt) 
{
     FILE *fp;
     
     size_t i = strlen(tt)+5;
     char *myword;
     char *cp = tt;
     char url[i];
    
     strcpy(cp, tt);
     strcpy(url, tt);
//     strcat(tt, "\n");
    
     int k=0; 
     for (k=0;k<=i;k++) 
     {
         
        if(cp[k]=='.' || cp[k]=='/' || cp[k]=='-'|| cp[k]==':' || cp[k]=='+') 
        {
//            printf("found one %s\n", cp);
            
            myword = cp;
            myword[k] = '\0';
//            printf("the word is %s\n", myword);
            
            cp = cp + k +1;
//            cp = cp +1;
//            printf("shiftet!%s\n", cp);
            if (strlen(myword)<=1) {k = 0;} else{

            char *filepath = "webindex/";

            char fileSpec[strlen(filepath)+strlen(myword)+1];
            snprintf( fileSpec, sizeof( fileSpec ), "%s%s", filepath, myword );

            if((fp = fopen(fileSpec , "a")) != 0)
            {
//                printf("saving url %s\n", url);
                fprintf(fp,"%s\n",url);
                fclose(fp);
            }
            k = 0;
            i = strlen(cp)+5; }
        }
         if (cp[k]=='\0') {
//           printf("found one %s\n", cp);
            
            myword = cp;
            myword[k] = '\0';
//            printf("the word is %s\n", myword);
              if (strlen(myword)<=1) {k = 0;} else{

            char *filepath = "keywords/";

            char fileSpec[strlen(filepath)+strlen(myword)+1];
            snprintf( fileSpec, sizeof( fileSpec ), "%s%s", filepath, myword );

            if((fp = fopen(fileSpec , "a")) != 0)
            {
//                printf("saving url %s\n", url);
                fprintf(fp,"%s\n",url);
                fclose(fp);
            }
            }  
             k = k+i;
             return (1);  
         }
             
//         printf("k: %d, length: %zu", k, strlen(cp));
     }
     return(1);
}


int QSize(char *q)
//
//  This function counts the number of links in the queue
//
{
  int k, total;
  total = 0;
  for(k=0;k<MAXQSIZE;k++)
  {
    if(q[k] == '\n') {total++;}
    if(q[k] == '\0') {return(total);}
  }
  return(total);
}


void AppendLinks(char *p, char *q, char *weblinks)

    {

    if (weblinks == NULL) {return;}

    int x;
    int y;
    x = QSize(q);
    //printf("%d\n", x);
    //printf("%s", weblinks);
    y = strlen(weblinks);

    char *lp;
    lp = weblinks;


    char *appendurl;
    char urlspace[MAXURL];
        appendurl = urlspace;
    char *ret;


    if ((x+y) < MAXQSIZE) {

        int k;

        for(k=0;k<=y;k++){

                GetNextURL(lp,lp,appendurl);
                if ((strncmp(appendurl,"h", 1))!=0 || (strcmp(appendurl,"http://127.0.0.1")==0))
                    {k=k+y;}
                else {
                    ret = strstr(q,lp);
                    if (ret== NULL) { 
                        char spliturl[strlen(appendurl)];
//                        printf("Appending and storing the url: %s\n", appendurl);
                        strcat(q,appendurl);
                        strcat(q,"\n");
                        deposit(appendurl);
                    } else {printf("This link already exists in queue: %s\n", appendurl);}
                lp = ShiftP(lp, weblinks);
                }
        }
    }
}

int GetNextURL(char *p, char *q, char *myurl)
//
//  This function puts the next URL from p into myurl.
//  We also pass the queue so that we can check for the end of the queue
//
{
// printf("Receiving URL");
  int k;
  for(k=0;k<MAXURL;k++)
  {
    if(p[k]=='\n')
    {
      myurl[k] = p[k];
      myurl[k] = '\0';
      return(1);
    }
    else {myurl[k] = p[k];}
  }
//
// If no URL is found, then return localhost in myurl and a zero from the function
// This is to ensure that this function always returns an http URL
//
  strcpy(myurl,"http://127.0.0.1");
  return(0);
}


int main(int argc, char* argv[]) 
// 
//  This code was written with speed and to some extent clarity in mind.  As you
//  are supposed to be able to convert your own functions from the previous assignment
//  to a WWW robot in under 2 hours, we also did a fast implementation to double-check
//  the time required. 
//
//  This basic WWW robot is using the two functions specified in class.
//  html = GetWebPage(url);
//  weblinks = GetLinksFromWebPage(html, url);
//
//  It does attempt to limit the downloads to Leiden University domains
//  It does limit the total number of downloads
//  It does not check for the Robot Exclusion Protocol: robots.txt because
//  we are only looking at domains which belong to us.  
//
{
    char *url = NULL;
    char *p, *q;
    char urlspace[MAXURL];
    char *html, *weblinks, *imagelinks;
    int k, qs, ql;
    int v;

//
// We will be putting all of the links found into q, short for "queue", and terminate with '\0'
// Initially, it will just end at MAXQSIZE, however, we could easily turn it into a ring queue
// where the end meets the beginning
//
    q = ((char *) malloc(MAXQSIZE));  // When done, use free(q) to free the memory back to the OS
    if (q==NULL) {printf("\n\nProblem with malloc...exiting\n\n"); exit(0);}
    q[0]='\0';
    p = q;

    if (argc <= 1) {printf("\n\nNo webpage given...exiting\n\n"); exit(0);} 
    else { 
      url = argv[1];
      if(strstr(url,"http") != NULL) {printf("\nInitial web URL: %s\n\n", url);}
      else {printf("\n\nYou must start the URL with lowercase http...exiting\n\n"); exit(0);}
    }
// 
//  Place the input URL into q and end it with '\n'
//
    strcat(q, url);  strcat(q, "\n");
    url = urlspace;
//
//  The loop limitation, MAXDOWNLOADS is the maximum number of downloads we
//  will allow the robot to perform.  It is just a precaution for this assignment
//  to minimize runaway bots
//
    for(k=0;k<MAXDOWNLOADS;k++)
    {
      qs = QSize(q); ql = strlen(q);
      printf("\nDownload #: %d   Weblinks: %d   Queue Size: %d\n",k, qs, ql);
//
//  As we go through the queue, we need to grab the next URL and shift the position of p
//  The method using the two functions GetNextURL and ShiftP was written for clarity.  
//  
      //
      //  *****ADD YOUR CODE HERE*****
      // Add code here to get the next URL and shift the position of q within q.
      //
      
            
        GetNextURL(p, q, url);
      
        p = ShiftP(p,q);

        
//
//  This is a simple test to see if the url is within one of the allowed domains
//  liacs.nl, leidenuniv.nl, leiden.edu, or mastersinleiden.nl.  It is
//  not perfect but will probably work for most of the cases and was trivial to write.
//
      if(1==1)
      {
//        printf("url=%s\n",url);
        html = GetWebPage(url);

// You can adjust this error check depending on the rest of your code.  In this case
// the downloaded webpage was NULL - had no html nor links
          
        if(html==NULL){printf("\n\nThe downloaded webpage was NULL\n\n");
	} else {

//        v = strlen(html); printf("\n\nwebpage size of %s is %d\n\n",url,v);
        if(html) { 
          weblinks = GetLinksFromWebPage(html, url);
          AppendLinks(p, q, weblinks);
        }
	}
      }
      else {printf("\n\nNot in allowed domains: %s\n\n",url);}
    }
    FILE *fp;
    fp = fopen("mylinks.txt","w");
    if(fp != NULL) {fprintf(fp, "%s",q);}
    free(q);
}
