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
/* Default physical size of array - no need to share this */
#define ARRDEFSZ 32

/* Set up set with no elements, but a physical container having a default ARRDEFSZ size */
/* Create empty set */
set* set_init(void)				
{
   set *p;
   p = (set*)calloc(1, sizeof(set));
	
   if (p == NULL) {
      ON_ERROR("Creation of Array Failed\n");
   }
   p->ua = (arr*)calloc(1, sizeof(arr));
   if(p == NULL) {
      ON_ERROR("Creation of Array Failed\n");
   } 
   p->ua->data = (arrtype*)calloc(ARRDEFSZ, sizeof(arrtype));
   if(p->ua->data == NULL) {
      ON_ERROR("Creation of Array Failed\n");
   }
   p->ua->pz = ARRDEFSZ;					
   p->sz = 0;							
	
   return p;
}

/* Create new set, copied from another */
set* set_copy(set* s)
{
   int i;
   set *p;
	
   if(s == NULL) {
      return set_init();
   }
   p = set_init();
   /* for(i=0;i<s->ua->pz;i++) */ 
   for(i = 0;i < s->sz;i++) {
      set_insert(p, s->ua->data[i]);
   }
   p->ua->pz = s->ua->pz;			
   p->sz = s->sz;					
	
   return p;
}

/* Create new set, copied from an array of length n */
set* set_fromarray(arrtype* a, int n)
{
   set *p;
   int i;
	
   if(a == NULL) {
      return set_init();
   }
   p = set_init();
   for(i = 0 ; i < n ; i++) {
      set_insert(p, a[i]);
   }
	
   return p;
}

/* Basic Operations */
/* Add one element into the set */
void set_insert(set* s, arrtype l)
{
   if(s == NULL) {
      /*s=set_init();*/
      return ;
   }
   if(set_contains(s, l) == 0) {
      /*arr_set(s->ua, s->sz, l);*/
      s->ua->data[s->sz] = l;
      s->sz++;
   }
}

int set_size(set* s)
{
   if (s == NULL) {
      return 0;
   }
   return s->sz;
}

/* Returns true if l is in the array, false elsewise */
int set_contains(set* s, arrtype l)
{
   int i;
	
   if(s == NULL) {
      /*s=set_init();	*/
      return 0;	 
   }
   for(i = 0 ; i < s->sz ; i++) {
      if(l == s->ua->data[i]) {
         return 1;
      }
   }
	
   return 0;
}

/* Remove l from the set (if it's in) */
void set_remove(set* s, arrtype l)
{
   int i, j;
	
   if(s == NULL) {
      s = set_init();
   }
   for(i = 0 ; i < s->sz ; i++) {
      if(l == s->ua->data[i]) {
         for(j = i ; j < s->sz ; j++) {
            if(j == s->sz-1) {
               s->ua->data[j] = 0;
            }
            else {
               s->ua->data[j] = s->ua->data[j+1];
            }
         }
      s->sz = s->sz-1;
      }
   }
   
   return ;
}

/* Remove one element from the set - there's no particular order for the elements, so any will do */
arrtype set_removeone(set* s)
{	
   int i;
	
   if(s == NULL) {
      return 0;
   }
   i = s->ua->data[s->sz-1];
   s->ua->data[s->sz-1] = 0;
   s->sz = s->sz-1;
	
   return i;
}

/* Operations on 2 sets */
/* Create a new set, containing all elements from s1 & s2 */
set* set_union(set* s1, set* s2)
{
   set *l;
   int i;
   
   l = set_init();
   if(s1 != NULL && s2 != NULL) {
      for(i=0;i<s1->sz;i++) {
         if(set_contains(s2, s1->ua->data[i])==0) {
            set_insert(l, s1->ua->data[i]);
         }
      }
      for(i = 0 ; i < s2->sz ; i++) {
         set_insert(l, s2->ua->data[i]);		
      }	
   }
   else {
      if(s1 != NULL && s2 == NULL) {
         l = set_copy(s1);
      }
      else if(s1 == NULL && s2 != NULL) {
         l = set_copy(s2);
      }
   }
   
   return l;	
}

/* Create a new set, containing all elements in both s1 & s2 */
set* set_intersection(set* s1, set* s2)
{
   set *p;
   int i, j;
   
   p = set_init();	
   if (s1 == NULL || s2 == NULL) {
      p = set_init();
   }
   else {
      for(i = 0 ; i < s1->sz ; i++) {
         for(j = 0 ; j < s2->sz ; j++) {
            if(s1->ua->data[i] == s2->ua->data[j]) {
               if(set_contains(p, s1->ua->data[i]) == 0) {
                  set_insert(p, s1->ua->data[i]);						
               }
            }
         }
      }		
   }
	
   return p;
}

/* Finish up */
/* Pass in the pointer to the set pointer ,so it can be set to NULL after */
/* Clears all space used, and sets pointer to NULL */
void set_free(set **s)
{
   if (s == NULL) {
      return;
   }
   else {
      set* a = *s;
      free(a->ua->data);
      free(a->ua);
      free(a);
      /* Helps to assert that the job has been done.*/
      *s = NULL;
   }
}





