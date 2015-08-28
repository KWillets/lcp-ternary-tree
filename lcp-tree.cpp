

// LCP Tree
// A somewhat depth-first representation of a Patricia Tree, but different
// root:  a random pivot string is chosen 
// and then subsequent keys are compared to it on insertion
// the point where they branch off left or right is the lcp against the pivot
// these branch-off lcp's are stored in a ternary search tree 
// whose nodes each point to another StringNode/recursive TPT
#include <stdio.h>

inline int lcpstrcmp( char * p, char * q, int &i) {
  for( ; q[i] == p[i] && q[i]; i++ )
    ;
  return q[i]-p[i];
}

template<bool ascending>
class LcpNode;

class StringNode {
  char * pivot = NULL ;
  LcpNode<true> *left   = NULL;
  LcpNode<false> *right = NULL;
  
public:
  StringNode( char * s );
  bool Search( char * s );
  void Insert( char * s );
  void print( char * src, int lcp );
};
  
// a ternary search tree of int lcp keys
// a match leads to a StringNode which is the root of 
// a recursive TPT for strings with the same common prefix
// ordering is either ascending or descending for the whole subtree
// (child nodes keep the  same orientation unless they're children of StringNode)
template<bool ascending>
class LcpNode {
  int lcp;
  StringNode * middle;  // singletons could be level-compressed to char * here maybe
  LcpNode<ascending> *left = NULL, *right = NULL;

public:
  LcpNode( char *s, int slcp );
  bool Search( char *s, int slcp );
  void Insert( char *s, int slcp );
  void print( char * src);
};



//=====

StringNode::StringNode( char * s ) {
  pivot = s;
}

bool StringNode::Search( char * s ) {
  int lcp=0;
  int cmpr = lcpstrcmp( pivot, s, lcp );
    if( cmpr == 0 )
      return true;
    else if( cmpr < 0 )
      return left ? left->Search( s+lcp, lcp ) : false;
    else
      return right ? right->Search( s+lcp, lcp ): false;
}

void StringNode::Insert( char * s ) {
  int lcp=0;
  int cmpr = lcpstrcmp( pivot, s, lcp );
    if( cmpr == 0 )
      return;  // already there
    else if( cmpr < 0 ) {
      if( !left )
        left = new LcpNode<true>( s +lcp, lcp );
      else
        left->Insert( s+lcp, lcp );
    } else {
      if( !right )
        right = new LcpNode<false>( s +lcp, lcp );
      else
        right->Insert( s+lcp, lcp );
    }
}

template<bool ascending>
void LcpNode<ascending>::Insert( char *s, int slcp ) {
  if( slcp == lcp )
    middle->Insert( s );
  else if( ascending ? slcp < lcp : slcp > lcp ) {
    if( !left )
      left = new LcpNode<ascending>( s, slcp );
    else
      left->Insert( s, lcp );
  } else {
    if( !right )
      right = new LcpNode<ascending>( s, lcp );
    else
      right->Insert( s, lcp );
  }
}

void StringNode::print( char * src, int lcp ) {
    char buf[500]; // concatenated path labels for use by descendants 
    sprintf( buf, "%*s%s", lcp, src, pivot );
    if( left ) left->print( src );
    printf("%s\n", buf );
    if( right ) right->print(src);
};

template<bool ascending>
 LcpNode<ascending>::LcpNode( char *s, int slcp ) {
  lcp = slcp;
  middle = new StringNode( s );
}

template<bool ascending>
bool LcpNode<ascending>::Search( char *s, int slcp ) {
  if( slcp == lcp )
    return middle->Search( s );
  else if( ascending ? slcp > lcp : slcp < lcp )
    return right ? right->Search(s, slcp) : false;
  else
    return left ? left->Search(s, slcp) : false;
}

template<bool ascending>
void LcpNode<ascending>::print( char * src) {  // use parent pivot to print up to lcp
  if(left ) left->print( src );
  middle->print( src, lcp );
  if(right) right->print( src );
}


