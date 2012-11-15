/****************************************/
/* NAME: RAGHU TEJA BOGGAVARAPU		*/
/* ROLL: MTC-1218			*/
/* ASSIGNMENT: RED BLACK TREE 		*/
/*	IMPLEMENTATION IN SECONDARY, 	*/
/*	STORAGE.			*/
/****************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <algorithm>

#include "rb.h"

bool Tree::sanity()
{
     rewind(fp);
     char *str = (char*) calloc(strlen(MAGIC_STR), sizeof(char));
     
     fread(str, sizeof(char), strlen(MAGIC_STR), fp);
     if (!memcmp(str, MAGIC_STR, strlen(MAGIC_STR) * sizeof(char)))
	  return true;
    
     return false;     
}

Tree::Tree(char *fName)
{
     unsigned int wb = 0;

     fp = fopen(fName, "rb+");

     if (fp)
     {
	  printf("opening file %s..\n", fName);
	  assert(sanity());
	  printf("sanity passed..\n");
	  fread(&nrecords, sizeof(int), 1, fp);
	  printf("\t%d records present\n", nrecords);
	  fclose(fp);
	  fp = fopen(fName, "r+");
     }
     else
     {
	  printf("file %s not found...creating\n", fName);
	  fp = fopen(fName, "w+");
	  fwrite(MAGIC_STR, strlen(MAGIC_STR), 1, fp);
	  nrecords = 0;
	  fwrite(&nrecords, sizeof(int), 1, fp);
     }	  
}

Tree::~Tree()
{
     rewind(fp);
     char str[5];
     fread(str, sizeof(char), strlen(MAGIC_STR), fp);

     printf("\tclosing file with %d records\n", nrecords);
     fwrite(&nrecords, sizeof(int), 1, fp);

     if (fp)
	  fclose(fp);     
}


void Tree::frewind()
{
     rewind(fp);
     fseek(fp, sizeof(char) *  strlen(MAGIC_STR), SEEK_CUR);     
     fseek(fp, sizeof(int), SEEK_CUR);
}

bool Tree::fgotorec(unsigned int rec)
{
     if (rec == 0 || nrecords == 0)
          return false;

     frewind();

     fseek(fp, (rec - 1) * (sizeof(node) + strlen(REC_VALID) * sizeof(char)), SEEK_CUR);

     char valid[3];
     fread(valid, sizeof(char), strlen(REC_VALID), fp);
     if (memcmp(valid, REC_VALID, sizeof(char) * strlen(REC_VALID)))
          return false;
     
     return true;
}

bool Tree::fgetrec(unsigned int rec, node *n)
{
     
     if (!fgotorec(rec))
	  return false;     

     fread(n, sizeof(node), 1, fp);
     fseek(fp, (-1) * (sizeof(node) + strlen(REC_VALID) * sizeof(char)), SEEK_CUR);
     return true;
}

bool Tree::Search(int key, char *value)
{
     node root;
     int rec = 1;
     
     while (fgetrec(rec, &root))
     {
	  if (key == root.key)
	       return true;

	  if (key > root.key)
	       rec = root.right;
	  else
	       rec = root.left;
     }

     return false;
}

bool Tree::isred(unsigned int rec)
{
     if (!rec)
	  return false;

     node n;

     fgetrec(rec, &n);
     return n.coloured;
}

unsigned int Tree::rotateleft(unsigned int rec)
{
     node x,h;
     
     fgetrec(rec, &h);
     unsigned int rightpos = h.right;
     fgetrec(rightpos, &x);
     
     h.right = x.left;
     x.left = rightpos;
     x.coloured = h.coloured;
     h.coloured = true;

     fgotorec(rec);
     fwrite(&x, sizeof(node), 1, fp);

     fgotorec(rightpos);
     fwrite(&h, sizeof(node), 1, fp);

     return rec;
     
}

unsigned int Tree::rotateright(unsigned int rec)
{
     node x,h;

     fgetrec(rec, &h);
     unsigned int leftpos = h.left;
     fgetrec(leftpos, &x);
     h.left = x.right;
     x.right = leftpos;
     x.coloured = h.coloured;
     h.coloured = true;

     
     fgotorec(rec);
     fwrite(&x, sizeof(node), 1, fp);

     fgotorec(leftpos);
     fwrite(&h, sizeof(node), 1, fp);
    
     return rec;
}

void Tree::flipcolors(unsigned int rec)
{
     node n,p;

     fgetrec(rec, &n);
     n.coloured = true;
     
     fgetrec(n.left, &p);
     p.coloured = false;
     fseek(fp, sizeof(char) * strlen(REC_VALID), SEEK_CUR);
     fwrite(&p, sizeof(node), 1, fp);
     

     fgetrec(n.right, &p);
     p.coloured = false;
     fseek(fp, sizeof(char) * strlen(REC_VALID), SEEK_CUR);
     fwrite(&p, sizeof(node), 1, fp);

     fgotorec(rec);
     fwrite(&n, sizeof(node), 1, fp);
       
     return;
}


void Tree::Insert(int key, char *value)
{
     node root;
     int rec = 1;
     bool found = false;
     std::vector<unsigned int> pathTrace;

     while (nrecords && fgetrec(rec, &root))
     {
	  pathTrace.push_back(rec);
	  
          if (key == root.key)
	  {
               found = true;
	       break;
	  }

          if (key > root.key)
               rec = root.right;
          else
               rec = root.left;
     }
     

     if (found)
     {
	  /* Just change the value ptr */
	  printf("\t\tkey %d already found\n", key);
     }
     else
     {
	  nrecords++;
	  node n;
	  
	  
	  char valid[3];
	  fread(valid, sizeof(char), strlen(REC_VALID), fp);
	  if (!feof(fp) && memcmp(valid, REC_VALID, sizeof(char) * strlen(REC_VALID)))
	  {	       
	       fread(&n, sizeof(node), 1, fp);
	       n.key = key;
	       n.coloured = true;
	       /* change the value ptr also */
	       fseek(fp, (-1) * (sizeof(node) + strlen(REC_VALID) * sizeof(char)), SEEK_CUR);
	       fwrite(REC_VALID, sizeof(char), strlen(REC_VALID), fp);
	       fwrite(&n, sizeof(node), 1, fp);
	       fseek(fp, (-1) * (sizeof(node) + strlen(REC_VALID) * sizeof(char)), SEEK_CUR);
	  }
	  else if (!feof(fp))
	  {
	       fread(&n, sizeof(node), 1, fp);

	       if (n.key > key)
		    n.left = nrecords;
	       else
		    n.right = nrecords;
	       
	       fseek(fp, (-1) * sizeof(node), SEEK_CUR);
	       fwrite(&n, sizeof(node), 1, fp);
	       fseek(fp, 0, SEEK_END);
	       
	       n.key = key;
	       n.left = 0;
	       n.right = 0;
	       /* work for value_ptr */
	       n.value_ptr = 0;
	       n.coloured = true;
	       fwrite(REC_VALID, sizeof(char), strlen(REC_VALID), fp);
	       fwrite(&n, sizeof(node), 1, fp);	       
	  }
	  else if (feof(fp))
	  {
	       n.key = key;
	       n.left = 0;
               n.right = 0;
	       /* work for value_ptr */
	       n.value_ptr = 0;
	       n.coloured = true;
               fwrite(REC_VALID, sizeof(char), strlen(REC_VALID), fp);
               fwrite(&n, sizeof(node), 1, fp);

	  }
	  
	  
	  while (pathTrace.size())
	  {	       	 
	       unsigned int current = pathTrace.back();
	       fgetrec(current, &n);

	       if (isred(n.right) && !isred(n.left))
		    current = rotateleft(current);
	       
	       node p; 
	       fgetrec(current, &n);
	       fgetrec(n.left, &p);	       
	       
	       if (isred(n.left) && isred(p.left))
		    current = rotateright(current);
	       
	       fgetrec(current, &n);
	       
	       if (isred(n.left) && isred(n.right))
		    flipcolors(current);	       	       
	       
	       pathTrace.pop_back();
	  }
	  
     }          
}

void Tree::PrintKeys(unsigned int baseRec)
{
     node n;
     if (!fgetrec(baseRec, &n))
	  return;

     PrintKeys(n.left);
     printf("\t %d", n.key);
     PrintKeys(n.right);
     
     return;
}



