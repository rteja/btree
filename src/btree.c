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

#include "btree.h"

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
     fp = fopen(fName, "rb+");

     if (fp)
     {
	  printf("opening file %s..\n", fName);
	  assert(sanity());
	  printf("sanity passed..\n");
	  fread(&nrecords, sizeof(int), 1, fp);
	  fread(&nnodes, sizeof(int), 1, fp);
	  fread(&root, sizeof(int), 1, fp);
	  printf("\t%d records present\n", nrecords);
	  printf("\t%d tnodes present\n", nnodes);
	 
	  fclose(fp);
	  fp = fopen(fName, "r+");
     }
     else
     {
	  printf("file %s not found...creating\n", fName);
	  fp = fopen(fName, "w+");
	  fwrite(MAGIC_STR, strlen(MAGIC_STR), 1, fp);
	  nrecords = 0;
	  nnodes = 0;
	  root = 0;
	  fwrite(&nrecords, sizeof(int), 1, fp);
	  fwrite(&nnodes, sizeof(int), 1, fp);
	  fwrite(&root, sizeof(int), 1, fp);
     }	  
}

Tree::~Tree()
{
     rewind(fp);
     char str[5];
     fread(str, sizeof(char), strlen(MAGIC_STR), fp);

     printf("\tclosing file with %d records\n", nrecords);
     fwrite(&nrecords, sizeof(int), 1, fp);
     fwrite(&nnodes, sizeof(int), 1, fp);
     if (fp)
	  fclose(fp);     
}


void Tree::frewind()
{
     rewind(fp);
     fseek(fp, sizeof(char) *  strlen(MAGIC_STR), SEEK_CUR);     // Magic String
     fseek(fp, sizeof(int), SEEK_CUR);   // No. of records 
     fseek(fp, sizeof(int), SEEK_CUR);   // No. of nodes
     fseek(fp, sizeof(int), SEEK_CUR);   // Root Node
}

void Tree::UpdateRoot(unsigned int newroot)
{
     frewind();
     
     fseek(fp, (-1) * sizeof(int), SEEK_CUR);
     fwrite(&newroot, sizeof(int), 1, fp);
}

bool Tree::fgotorec(int rec)
{
     if (rec == 0 || nnodes == 0)
          return false;

     if (rec < 0)
	  return false;

     frewind();

     fseek(fp, (rec - 1) * (sizeof(node) + strlen(REC_VALID) * sizeof(char)), SEEK_CUR);

     char valid[3];
     fread(valid, sizeof(char), strlen(REC_VALID), fp);
     if (memcmp(valid, REC_VALID, sizeof(char) * strlen(REC_VALID)))
          return false;
     
     return true;
}

bool Tree::fgetrec(int rec, node *n)
{
     
     if (!fgotorec(rec))
	  return false;     

     fread(n, sizeof(node), 1, fp);
     fseek(fp, (-1) * (sizeof(node) + strlen(REC_VALID) * sizeof(char)), SEEK_CUR);
     return true;
}

bool Tree::fwriterec(int rec, node *n)
{     
     fgotorec(rec);
     
     if (rec < 0)
	  return false;
     
     fseek(fp, (-1) * (strlen(REC_VALID) * sizeof(char)), SEEK_CUR);
     fwrite(REC_VALID, sizeof(char), strlen(REC_VALID), fp);
     fwrite(n, sizeof(node), 1, fp);
     fseek(fp, (-1) * (sizeof(node) + strlen(REC_VALID) * sizeof(char)), SEEK_CUR);
     return true;	  
}

int Tree::fmalloc()
{
     nnodes++;
     frewind();
     node null;
     fseek(fp, 0, SEEK_END);
     fwrite(REC_INVALID, sizeof(char), strlen(REC_INVALID), fp);
     fwrite(&null, sizeof(node), 1, fp);
     fseek(fp, (-1) * (sizeof(node) +  strlen(REC_VALID) * sizeof(char)), SEEK_CUR);


     return nnodes;
}

void Tree::ffree(int recpos)
{
     frewind();
     node null;
     // Asssert if recpos is greater than total number of records
     // Goto the record position
     fgotorec(recpos);
     
     fwrite(REC_INVALID, sizeof(char), strlen(REC_INVALID), fp);
     fwrite(&null, sizeof(node), 1, fp);
     nnodes--;     
}


bool Tree::Search(int key, char *value)
{
     
     return false;
}

int Tree::SearchKeyPos(int key, node *n)
{
     int pos = 0;
     while (key > n->keys[pos] && pos < n->occupancy)
	  pos++;
     return pos;
}

/* return 0: Success */
/* return 1: Insert at the root */
/* return 2: Duplicate key*/
/* return 4: Sucessful insertion */
int Tree::Insert_R(unsigned int curnode, int key, int *upkey, unsigned int *nextnode)
{
     node n;
     int retval;

     if (curnode == 0)
     {
	  *upkey = key;
	  *nextnode = 0;
	  retval = 1;
	  return retval;
     }

     int pos, newkey, i, lastkey, splitpos;
     unsigned int newnode_pos, lastnode_pos;

     fgetrec(curnode, &n);
     
     pos = SearchKeyPos(key, &n);
     if (pos < n.occupancy && key == n.keys[pos])
	  return 2;

     retval = Insert_R(n.links[pos], key, &newkey, &newnode_pos);
     
     if (retval != 1)
	  return retval;
     
     if (n.occupancy < M - 1)
     {
	  for (i = n.occupancy; i > pos; i--)
	  {
	       n.keys[i] = n.keys[i - 1];
	       n.links[i + 1] = n.links[i];
	  }
	  n.keys[pos] = newkey;
	  n.links[pos + 1] = newnode_pos;
	  n.occupancy++;
	  fwriterec(curnode, &n);
	  return 4;
     }

     if (pos == M - 1)
     {
	  lastkey = newkey;
	  lastnode_pos = newnode_pos;
     }
     else
     {
	  lastkey = n.keys[n.occupancy - 1];
	  lastnode_pos = n.links[n.occupancy];
	  
	  for (i = M - 2; i > pos; i--)
	  {
	       n.keys[i] = n.keys[i - 1];
	       n.links[i + 1] = n.links[i];
	  }
	  
	  n.keys[pos] = newkey;
	  n.links[pos + 1] = newnode_pos;
     }

     splitpos = (M - 1) / 2;
     *upkey =  n.keys[splitpos];
     *nextnode = fmalloc();
     node nnode;

     n.occupancy = splitpos;
     nnode.occupancy = M - 1 - splitpos;

     for (i = 0; i < (nnode.occupancy); i++)
     {
	  nnode.links[i] = n.links[splitpos + 1 + i];
	  if (i < nnode.occupancy - 1)
	       nnode.keys[i] = n.keys[splitpos + 1 + i];
	  else
	       nnode.keys[i] = lastkey;
     }
     
     nnode.links[nnode.occupancy] = lastnode_pos;
     fwriterec(*nextnode, &nnode);
     fwriterec(curnode, &n);
     return 1;
     
}

void Tree::Insert(int key, char *value)
{
     int upkey;
     unsigned int nextnode;

     int retval = Insert_R(root, key, &upkey, &nextnode);
     
     if (retval == 2)
     {
	  printf("key %d already exists\n", key);
     }
     else if (retval == 1)
     {
	  node p;
	  unsigned int p_pos = fmalloc();
	  p.occupancy = 1;
	  p.keys[0] = upkey;
	  p.links[0] = root;
	  p.links[1] = nextnode;	  
	  fwriterec(p_pos, &p);
	  UpdateRoot(p_pos);

     }

     if (retval != 2)
	  nrecords++;
}

unsigned int Tree::GetPrevNode(unsigned int node_pos, int pos)
{
     node n;
     fgetrec(node_pos, &n);
     unsigned int tmp_pos = n.links[pos];
     
     while(true)
     {
	  node tmp;
	  fgetrec(tmp_pos, &tmp);
	  unsigned int tmp_1 = tmp.links[tmp.occupancy];
	  if (tmp_1 == 0)
	       break;
	  tmp_pos = tmp_1;
     }
     return tmp_pos;
}

/* return: 1 : key not found */
/* return: 2 : successful key deletion */
/* return: 3 : less number of keys in tnode */
int Tree::Delete_R(unsigned int root_pos, unsigned int curnode_pos, int key)
{
     int i, pos; 
     unsigned char min_order;
     int retval;
     min_order = (M - 1) >> 1;

     if (curnode_pos == 0)
	  return 1;
     
     node curnode, root;
     fgetrec(curnode_pos, &curnode);
     fgetrec(root_pos, &root);

     pos = SearchKeyPos(key, &curnode);
     
     if (curnode.links[0] == 0)
     {
	  if (pos == curnode.occupancy || key < curnode.keys[pos])
	       return 1;
	  
	  for (i = pos + 1; i < curnode.occupancy; i++)
	  {
	       curnode.keys[i - 1] = curnode.keys[i];
	       curnode.links[i] = curnode.links[i + 1];
	  }
	  fwriterec(curnode_pos, &curnode);
	  if (curnode_pos == root_pos)
	  {
	       return (curnode.occupancy > 1) ? 2 : 3;
	  }
	  return (curnode.occupancy > min_order) ? 2 : 3;
     }

     if (pos < curnode.occupancy && key == curnode.keys[pos])
     {
	  unsigned prevnode_pos = GetPrevNode(curnode_pos, pos);
	  node prevnode;
	  fgetrec(prevnode_pos, &prevnode);
	  curnode.keys[pos] = prevnode.keys[prevnode.occupancy - 1];
	  prevnode.keys[prevnode.occupancy - 1] = key;
	  fwriterec(prevnode_pos, &prevnode);
	  fwriterec(curnode_pos, &curnode);
     }

     retval = Delete_R(root_pos, curnode.links[pos], key);
     
     if (retval != 3)
	  return retval;

     node curnode_tmp;
     fgetrec(curnode.links[pos], &curnode_tmp);
     if (pos > 0 && (curnode.links[pos]) && (curnode_tmp.occupancy > min_order))
     {
	  unsigned int left_pos = curnode.links[pos - 1];
	  unsigned int right_pos = curnode.links[pos];

	  node left, right;
	  fgetrec(left_pos, &left);
	  fgetrec(right_pos, &right);

	  right.links[right.occupancy + 1] = right.links[right.occupancy];
	  
	  for (i = right.occupancy; i > 0; i--)
	  {
	       right.links[i] = right.links[i - 1];
	       right.keys[i] =  right.keys[i - 1];
	  }

	  right.occupancy++;
	  right.keys[0] = curnode.keys[pos - 1];
	  right.links[0] =  left.links[left.occupancy];
	  curnode.keys[pos - 1] = left.keys[left.occupancy - 1];
	  left.occupancy--;
	  
	  fwriterec(curnode_pos, &curnode);
	  fwriterec(left_pos, &left);
	  fwriterec(right_pos, &right);
	  return 2;
     }

     fgetrec(curnode.links[pos + 1], &curnode_tmp);
     if (curnode.links[pos + 1] && curnode_tmp.occupancy > min_order)
     {
	  unsigned int left_pos = curnode.links[pos];
	  unsigned int right_pos = curnode.links[pos + 1];
	  
	  node left, right;
	  fgetrec(left_pos, &left);
	  fgetrec(right_pos, &right);

	  left.keys[left.occupancy] = curnode.keys[pos];
	  left.occupancy++;
	  left.keys[left.occupancy] =  right.links[right.occupancy];
	  curnode.keys[pos] = right.keys[0];
	  right.occupancy--;

	  for (i = 0; i < right.occupancy; i++)
	  {
	       right.keys[i] = right.keys[i + 1];
	       right.links[i] = right.links[i + 1];
	  }
	  right.links[right.occupancy] = right.links[right.occupancy + 1];
	  
	  fwriterec(curnode_pos, &curnode);
	  fwriterec(left_pos, &left);
	  fwriterec(right_pos, &right);
	  return 2;
     }
     int relative_pos = (pos == curnode.occupancy) ? (pos - 1) : pos;
     unsigned int left_pos = curnode.links[relative_pos];
     unsigned int right_pos = curnode.links[relative_pos + 1];
     
     node left, right;
     fgetrec(left_pos, &left);
     fgetrec(right_pos, &right);
     
     left.keys[left.occupancy] = curnode.keys[relative_pos];
     left.keys[left.occupancy + 1] = right.links[0];
     
     for (i = 0; i < right.occupancy; i++)
     {
	  left.keys[left.occupancy + 1 + i] = right.keys[i];
	  left.keys[left.occupancy + 2 + i] = right.links[i + 1];
     }
     left.occupancy += (right.occupancy + 1);
     //free(right)
     for (i = pos + 1; i < curnode.occupancy; i++)
     {
	  curnode.keys[i - 1] = curnode.keys[i];
	  curnode.links[i] =  curnode.links[i + 1];
     }
     curnode.occupancy--;

     fwriterec(curnode_pos, &curnode);
     fwriterec(left_pos, &left);
     fwriterec(right_pos, &right);
     
     if (curnode_pos == root_pos)
	  return (curnode.occupancy > 1) ? 2 : 3;

     return (curnode.occupancy > min_order) ? 2 : 3;     
}
 
bool Tree::Delete(int key)
{
     int retval = Delete_R(root, root, key);

     if (retval == 2)
     {
	  nrecords--;
	  return true;
     }
     
     printf("error: key %d not found\n", key);
     return false;
}


void Tree::PrintKeys()
{
     node n;
     int i = 0;

     for (i = 1; i <= nnodes; i++)
     {
	  fgetrec(i, &n);
	  for (int j = n.occupancy; j > 0; j--)
	       printf("node:%d key%d\n", i, n.keys[j - 1]);
     }
          
     return;
}



