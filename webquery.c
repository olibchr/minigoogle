#include <stdio.h>
#include <string.h>
#define MAXBUFFER 10000


int main(int argc, char *argv[])
{
  FILE *fp;
  char queryterms[MAXBUFFER];
  fp=fopen("queryterms.txt","r");
  fgets(queryterms,MAXBUFFER,fp);
  fclose(fp);
    
    
    int k;
    for (k=0;k<=strlen(queryterms);k++) 
     {   
    if(queryterms[k]=='.' || queryterms[k]=='/' || queryterms[k]=='-'|| queryterms[k]==':' || queryterms[k]=='+' || queryterms[k]==' ') { queryterms[k]='\0';}}
    
    
    char *filepath = "webindex/";

    char fileSpec[strlen(filepath)+strlen(queryterms)+1];
    snprintf( fileSpec, sizeof( fileSpec ), "%s%s", filepath, queryterms );

  fp = fopen(fileSpec,"r");
  fread(queryterms, sizeof(char), MAXBUFFER,fp);
  fclose(fp);
    
  printf("%s", queryterms);
}
