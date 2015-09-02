// Simplified LCP Tree
// One node type that combines lcp and string values, based on the fact that each 
// lcp node has one string node directly under it.

// Uses linked lists for sibling lcp nodes instead of ternary search tree
// linear scan for child lookups, but lcp is usually skewed to 0 so it may be faster.
// both left and right are ascending order lists; same loop works for either
// ascending/descending orders only need to be observed in Print() or other ordered traversal operations
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

inline int lcpstrcmp( char * p, char * q, int &i) {
  for( ; q[i] == p[i] && q[i]; i++ )
    ;
  return q[i]-p[i];
}

class Node {

  int lcp; // against parent if any
  char * value; // string key
  
  Node *next;  // sibling

  Node *left;
  Node *right;

  void revPrint( char *parentVal, int plcp ) {
    if( next )
      next->revPrint( parentVal, plcp );
    Print(parentVal, plcp );
  }
public:

  Node( char *val, int _lcp, Node *_next ) : value(val), lcp(_lcp), next(_next), left(0), right(0)
  { printf("Node:  %s  %d\n", val, _lcp); };
  
  bool Search( char *val ) {
    int vlcp = 0;
    int cmpr = lcpstrcmp( value, val, vlcp );
    
    if( cmpr == 0 )
      return true;
    else {
      Node *list = cmpr > 0 ? right : left ;
      for( ; list && vlcp > list->lcp ; list = list->next )
	;
      if( list && vlcp == list->lcp )
	return list->Search( val + vlcp );
    }
    return false;
  };

  void Insert( char *val ) {
    int vlcp = 0;
    int cmpr = lcpstrcmp( value, val, vlcp );
    
    if( cmpr == 0 )
      return;
    else {
      Node **pprev = cmpr > 0 ? &right : &left ;
      Node *list = *pprev;
      for( ; list && vlcp > list->lcp ; list = list->next )
	pprev = &list->next;
      if( list && vlcp == list->lcp )
	return list->Insert( val + vlcp );
      else 
	*pprev = new Node( val + vlcp, vlcp, list );
    }
      
  };

  void Print( char * parentVal, int plcp ) {
    char buf[500];
    memcpy(buf, parentVal, plcp+lcp );
    strcpy(buf+plcp+lcp, value);

    // left 
    for( Node *p=left; p; p=p->next )
      p->Print(buf, plcp+lcp );

    printf("%s\n", buf );
    
    // right, recursively? or reverse & print
    if(right) right->revPrint( buf, plcp+lcp );
  };
};


char ** readitems( char *fname, int *pn ) {
  int n=0;
  char **s = NULL;
  FILE * fd = fopen(fname, "r");
  if( fd == NULL ) 
    fprintf(stderr, "fopen %s failed\n", fname);
  else {
    int nalloc = 1<<16;
    s=(char **) calloc(sizeof(char *), nalloc);

    char *pp=NULL;
    ssize_t ll=0;
    size_t l=0;
    while( 0 < (ll = getline(&pp, &l, fd)) ) {
      pp[ll-1] ='\0';
      if( n >= nalloc ) {
        nalloc <<=1;
        s = (char **) realloc( s, sizeof(char *) * nalloc);
      }
        
      s[n++] = strdup(pp);

    }
  }
  *pn = n;
  return s;
}



int main( int argc, char **argv) {

  //  LcpNode<false>::max_id=1; // odds
  //LcpNode<true>::max_id=3; // odds
  int n=0;
  char ** strings = readitems( argv[1], &n );
  Node *root = new Node( strings[0], 0, 0 );
  for( int i = 1; i < n ; i++ )
    root->Insert( strings[i] );

  root->Print( "*", 0 );

  printf("\n\n==== search tests ====\n");
  for( int j = 0; j < 10; j++ ) {
    char * s=strings[ rand() % n ];

    printf( "%s %s found\n", s, root->Search(s) ? "" : "not" );
  }
  //printf("\n\n==== graphviz ====\ndigraph g {\n");
  //root->graph(-1, "");
  //printf("}\n\n");
}

