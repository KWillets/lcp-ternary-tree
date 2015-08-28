# lcp-ternary-tree
A randomized string search tree based on the call graph of lcp-quicksort

This tree mimics the call graph of lcp-quicksort and alternates string and lcp nodes.  
Each string node represents one string; lcp nodes descendant from a string node 
each represent one lcp value against that string.

String nodes 

Imagine organizing a tree where each node has one string key as follows:

       
                   modifiable
                    /
                   9   
          modifiably
             /
            / modificative
           /  /
          6  9
    modificatory
        4 6 
         \ \
          \ modifier
           \
           modist



Child edges (in each direction) are labeled by their lcp value against the parent key.  Since lcp increases with proximity,
ordering the labels in ascending order before the parent and descending afterwards puts the child nodes in lexicographic order.

If multiple keys have the same lcp value,
we build a subtree from them recursively below that lcp edge.

The potential number of these edges is indefinite, so we use a ternary search tree to organize them:

    modificatory
       \  
        \   6 --- modifier
         \ /
          4   --- modist
           \
            *

Each string node has one string key, and left and right lcp subtrees.  Each lcp node has one lcp value and left/right subtree pointers, and 
a pointer to a string node which is a recursive subtree for all keys that have the same lcp against the parent string node.  

Search 

Searching for a string in the tree alternates between string and lcp comparisons; the string comparison
yields a sign (<,>,=) and an lcp against the key at that node, and the lcp is looked up in 
the left or right subtrees depending on sign.  If found, the child string node is searched, starting 
comparisons from the known lcp identified.  

