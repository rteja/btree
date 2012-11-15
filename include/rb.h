#ifndef _RB_H_
#define _RB_H_

typedef struct node node;

#define MAGIC_STR "RBFS"
#define REC_VALID "RV"
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
     unsigned int fmalloc();
     void ffree(unsigned int rpos);
     bool fgotorec(unsigned int record);
     bool fgetrec(unsigned int n, node *rec);
     
     bool isred(unsigned int rec);
     unsigned int rotateleft(unsigned int rec);
     unsigned int rotateright(unsigned int rec);
     void flipcolors(unsigned int rec);

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
