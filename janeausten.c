#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>

#include "arr.h"
#include "set.h"

int main(void) {
	
   set *sa, *sb, *sc;
   FILE *fp1, *fp2;
   arrtype strings1, strings2;
   sa = set_init();
   sb = set_init();
   memset(strings1.str, '\0', sizeof(arrtype));
   memset(strings2.str, '\0', sizeof(arrtype));
   fp1 = fopen("sense-and-sense.txt", "r");
   fp2 = fopen("pride-and-prej.txt", "r");
 
   printf("Start...\n");	
  
   if(fp1 == NULL) {
      ON_ERROR("File 1 doesn't exist\n");
   }
   while(fscanf(fp1, "%s", strings1.str) != EOF) {
      if(strlen(strings1.str) != 0) {
         set_insert(sa, strings1);
      }
   }

   if(fp2 == NULL) {
      ON_ERROR("File 2 doesn't exist\n");
   }
   while(fscanf(fp2, "%s", strings2.str) != EOF) {
      if(strlen(strings2.str) != 0) {
         set_insert(sb, strings2);
      }
   }

   sc = set_intersection(sa, sb);
   
   printf("There are %d unique words in sense-and-sense.txt\n", sa->ua->pz);
   printf("There are %d unique words in pride-and-prej.txt\n" , sb->ua->pz);
   printf("There are %d common words\n", sc->ua->pz);

   fclose(fp1);
   fclose(fp2);

   set_free(&sa);
   set_free(&sb);
   set_free(&sc);

   printf("End...\n");
   return 0;
} 
