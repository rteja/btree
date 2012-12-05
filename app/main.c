/****************************************/
/* NAME: RAGHU TEJA BOGGAVARAPU		*/
/* ROLL: MTC-1218			*/
/* ASSIGNMENT: RED BLACK TREE 		*/
/*	IMPLEMENTATION IN SECONDARY, 	*/
/*	STORAGE.			*/
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

int main()
{
     char fileName[] = "btfs.bin";
     char str[] = "trial";

     Tree bt(fileName);

     printf("BINARY TREE - Secondary storage implementation\n");
     printf("Enter elements into the tree...\n");
     printf("Enter number of elements:");
     int i, n;
     scanf("%d", &n);

     if (n < 0)
     {
	printf("Invalid input.\n");
	//exit(1);
     }


     for (i = 0; i < n; i++)
     {
	
	int elm;
	printf("enter element:#%d: ", i+1);
	scanf("%d", &elm);
	bt.Insert(elm, str);
     }

     printf("Keys..\n");
     bt.PrintKeys();
     printf("\n stored in file btfs.bin\n");
     return 1;
}
