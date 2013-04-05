#ifndef _BTREE_H_
#define _BTREE_H_

typedef struct header header;
typedef struct node node;

#define MAGIC_STR "BTFS"
#define REC_VALID "RV"
#define REC_INVALID "RI"
#define REC_START 0x1

/* For the first version define M as in Tree */
#define M 4

struct header
{
};


struct node
{
     unsigned char occupancy;
     int keys[M - 1];
     unsigned int links[M];
};

class Tree
{
     FILE *fp;
     int nrecords;
     int nnodes;
     int root;
     bool sanity();
     void frewind();
     int fmalloc();
     void ffree(int recpos);
     bool fgotorec(int recpos);
     bool fgetrec(int recpos, node *rec);
     bool fwriterec(int recpos, node *rec);    
     int Insert_R(unsigned int root, int key, int *upkey, unsigned int *nextnode);	
     int Delete_R(unsigned int root, unsigned int curnode, int key);
     unsigned int GetPrevNode(unsigned int node_pos, int key_pos);
     
     void UpdateRoot(unsigned int);
     int SearchKeyPos(int, node*);
 
public:
     Tree(char *fileName);
     ~Tree();
     void Insert(int key, char *value);
     bool Search(int key, char *value);
     bool Delete(int key);
     void PrintKeys();
};



#endif
