#ifndef INCLUDEnode_h_
#define INCLUDEnode_h_

typedef struct e {
    int next;
    int length;
} edge;

typedef struct n {
    int nr;
    edge nextN;
    edge nextE;
    edge nextS;
    edge nextW;
    int dj_length;
} node;



#endif  

