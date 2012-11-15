/****************************************/
/* NAME: RAGHU TEJA BOGGAVARAPU		*/
/* ROLL: MTC-1218			*/
/* ASSIGNMENT: RED BLACK TREE 		*/
/*	IMPLEMENTATION IN SECONDARY, 	*/
/*	STORAGE.			*/
/****************************************/

#include <stdio.h>
#include <stdlib.h>
#include "rb.h"

int main()
{
     char fileName[] = "rbfs.bin";
     char str[] = "trial";

     Tree rb(fileName);

     printf("RED BLACK TREE - Secondary storage implementation\n");
     printf("Enter elements into the tree...\n");
     printf("Enter number of elements:");
     int i, n;
     scanf("%d", &n);

     if (n < 0)
     {
	printf("Invalid input.\n");
	exit(1);
     }

     for (i = 0; i < n; i++)
     {
	/*
     	rb.Insert(0, str);
     	rb.Insert(1, str);
     	rb.Insert(2, str);
     	rb.Insert(3, str);
	*/
	int elm;
	printf("enter element:%d", i+1);
	scanf("%d", &elm);
	rb.Insert(elm, str);
     }

     printf("Keys..\n");
     rb.PrintKeys(1);
     printf("Keys stored in file rbfs.bin\n");
     return 1;
}
