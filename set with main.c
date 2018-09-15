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
set* set_init(void) {
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
set* set_copy(set* s) {
   int i;
   set *p;
	
   if(s == NULL) {
      return set_init();
   }
   p = set_init();
   /* for(i=0;i<s->ua->pz;i++) */ 
   for(i = 0; i < s->sz; i++) {
      set_insert(p, s->ua->data[i]);
   }
   p->ua->pz = s->ua->pz;			
   p->sz = s->sz;					
	
   return p;
}

/* Create new set, copied from an array of length n */
set* set_fromarray(arrtype* a, int n) {
   set *p;
   int i;
	
   if(a == NULL) {
      return set_init();
   }
   p = set_init();
   for(i = 0; i < n; i++) {
      set_insert(p,a[i]);
   }
	
   return p;
}

/* Basic Operations */
/* Add one element into the set */
void set_insert(set* s, arrtype l) {
   if(s == NULL) {
      /*s=set_init();*/
      return ;
   }
   if(set_contains(s,l) == 0) {
      /*arr_set(s->ua, s->sz, l);*/
      s->ua->data[s->sz] = l;
      s->sz++;
   }
}

int set_size(set* s) {
	
   if (s == NULL) {
      return 0;
   }
   return s->sz;
}

/* Returns true if l is in the array, false elsewise */
int set_contains(set* s, arrtype l) {
   int i;
	
   if(s==NULL) {
      /*s=set_init();	*/
      return 0;	 
   }
   for(i=0;i<s->sz;i++) {
      if(l==s->ua->data[i]) {
         return 1;
      }
   }
	
   return 0;
}

/* Remove l from the set (if it's in) */
void set_remove(set* s, arrtype l) {
   int i, j;
	
   if(s == NULL) {
      s = set_init();
   }
   for(i = 0; i < s->sz; i++) {
      if(l == s->ua->data[i]) {
         for(j = i; j < s->sz; j++) {
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
arrtype set_removeone(set* s) {	
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
set* set_union(set* s1, set* s2) {
   set *l;
   int i;
   
   l = set_init();
   if(s1 != NULL && s2 != NULL) {
      for(i = 0; i < s1->sz; i++) {
         if(set_contains(s2, s1->ua->data[i]) == 0) {
            set_insert(l, s1->ua->data[i]);
         }
      }
      for(i = 0; i < s2->sz; i++) {
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
set* set_intersection(set* s1, set* s2) {
   set *p;
   int i, j;
   
   p=set_init();	
   if (s1 == NULL || s2 == NULL) {
      p = set_init();
   }
   else {
      for(i = 0; i < s1->sz; i++) {
         for(j = 0; j < s2->sz; j++) {
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
void set_free(set **s) {
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

int main(void) {
   int i;   	
   set *sa, *sb, *sc, *su, *si;
   arrtype aa[] = {5,1,7,5};
   arrtype ab[] = {4,2,6,7};

   printf("Basic Set Tests ... Start\n");

/* Check basic set NULL (empty) operations are dealt with correctly */
   assert(set_size(NULL) == 0);
   set_remove(NULL,18);
   set_insert(NULL, 1);
   set_free(NULL);
   assert(set_contains(NULL, 0) == 0);

/* Check 2 set NULL (empty) operations are dealt with correctly */
   sa = set_fromarray(NULL, 0);
   assert(sa != NULL);
   assert(set_size(sa) == 0);
   sb = set_copy(sa);
   assert(sb != NULL);
   assert(set_size(sb) == 0);
   set_free(&sa);
   set_free(&sb);

   sa = set_fromarray(ab, 4);			/* ab[]={4, 2, 6, 7} */
   assert(set_size(sa) == 4);
   sc = set_intersection(NULL,sa);		/*空集合和集合交集為空集合*/
   assert(sc != NULL);				/* sc = set_init()*/ 
   assert(set_size(sc) == 0);		/* sc沒值 sc->sz=0 */
   sb = set_intersection(sa,NULL);
   assert(sc != NULL);
   assert(set_size(sb) == 0);
   set_free(&sa);
   set_free(&sb);
   set_free(&sc);

   sa = set_fromarray(ab,4);
   sc = set_union(NULL,sa);			/* sc->ua->data={4, 2, 6, 7} */
   assert(set_size(sc) == 4);
   sb = set_union(sa,NULL);
   assert(set_size(sb) == 4);
   set_free(&sa);
   set_free(&sb);
   set_free(&sc);  
    
   sa = set_union(NULL, NULL);
   assert(sa != NULL);
   assert(set_size(sa) == 0);
   set_free(&sa);

/* Remove */
   sa = set_fromarray(ab,4);
   for(i = 0; i < 4; i++){
      set_removeone(sa);
      assert(set_size(sa) == 3-i);		/* set_removeone依序移除最後一個 */
   }
   set_free(&sa);

/* Basic Set creation */
   sa = set_fromarray(aa,4);				/* sa->ua->data[]={5, 1, 7} set_insert會把相同的幹掉 */
   assert(set_size(sa) == 3);			 
   sb = set_fromarray(ab,4);				/* sb->ua->data[]={4, 2, 6, 7} */
   assert(set_size(sb) == 4);
   assert(set_contains(sa,1) == 1);
   assert(set_contains(sb,1) == 0);
   assert(set_contains(sa,5) == 1);
   assert(set_contains(sb,5) == 0);
   assert(set_contains(sa,7) == 1);
   assert(set_contains(sb,7) == 1);

/* Union */
   su = set_union(sa, sb);					/* su->ua->data[]={5, 1, 7, 4, 2, 6} */
   assert(set_size(su) == 6); 		
   assert(set_contains(su,1) == 1);
   assert(set_contains(su,2) == 1); 

 /* Intersections */
   si = set_intersection(sa, sb);			/* si->ua->data[]={ 7 } */
   assert(set_contains(si,7) == 1);
   assert(set_contains(si,5) == 0);
   assert(set_size(si) == 1);

/* Copying */
   sc = set_copy(sa);
   assert(set_size(sa) == set_size(sc));    /* set_size(sc)=3 */
   for(i = 0; i < set_size(sc); i++){		
      assert(set_contains(sa, set_removeone(sc)));			
   }

/* Clear */
   set_free(&sa);
   assert(sa == NULL);
   set_free(&sb);
   assert(sb == NULL);
   set_free(&sc);
   assert(sc == NULL);
   set_free(&si);
   assert(si == NULL);
   set_free(&su);
   assert(su == NULL);

   printf("Basic Set Tests ... Stop\n");

   return 0;
} 




