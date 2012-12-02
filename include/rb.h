#ifndef _RB_H_
#define _RB_H_

typedef struct node node;

#define MAGIC_STR "RBFS"
#define REC_VALID "RV"
#define REC_INVALID "RI"
#define REC_START 0x1


struct node
{
     unsigned int right;
     unsigned int left;
     int key;
     unsigned int value_ptr;
     bool coloured;
};

class Tree
{
     FILE *fp;
     int nrecords;
     bool sanity();
     void frewind();
     int fmalloc();
     void ffree(int recpos);
     bool fgotorec(int recpos);
     bool fgetrec(int recpos, node *rec);

     bool fwriterec(int recpos, node *rec);    
 
     bool isred(unsigned int recpos);
     unsigned int rotateleft(unsigned int recpos);
     unsigned int rotateright(unsigned int recpos);
     void flipcolors(unsigned int recpos);

public:
     node *root;
     Tree(char *fileName);
     ~Tree();
     void Insert(int key, char *value);
     bool Search(int key, char *value);
     void Delete(int key);
     void PrintKeys(unsigned int baseRec);
};



#endif
