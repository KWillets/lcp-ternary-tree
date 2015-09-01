// Simplified LCP Tree
// One node type that combines lcp and string values, based on the fact that each 
// lcp node has one string node directly under it.

// Uses linked lists for sibling lcp nodes instead of ternary search tree
// linear scan for child lookups, but lcp is usually skewed to 0 so it may be faster.
// both left and right are ascending order lists; same loop works for either
// ascending/descending orders only need to be observed in Print() or other ordered traversal operations


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

public:

  Node( char *val, int _lcp, Node *_next ) : value(val), lcp(_lcp), next(_next), left(0), right(0)
  {};
  
  bool Search( char *val ) {
    int vlcp = 0;
    int cmpr = lcpstrcmp( value, val, &vlcp );
    
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
    int cmpr = lcpstrcmp( value, val, &vlcp );
    
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
	*pprev = new Node( value + vlcp, vlcp, list );
    }
      
  };

  void Print( char * parentVal ) {
    char buf[500];
    sprintf("%.*s%s", lcp, parentVal, value );
    // left 
    printf("%s\n", buf );
    // right, recursively? or reverse & print
  };
};



