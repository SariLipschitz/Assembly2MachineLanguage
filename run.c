#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

/*
@name: SARI LIPSCHITZ ID:212342877
@name: ESTY COHEN ID:325287597

my files: (.c)

linked_list.c all the lists function-The lists are built to be that each part of the list is a union of structs(macro /label / printLine / 	instructions...) and this union struct is used for evry list in my program
macro.c In this file are the macro-related functions-(turn to *.am ,error in macros....)
cod.c colect the data into lists(label list,entry list...) looking for errors in each line, counting ic,dc
bit.c convers the data to bynary
error.c This file has all the functions that check the errors of each row in the file
tools.c This file has all my tools functions that I send from the main functions

my files: (.h)
code.h header for structures and code functions
errors.h heder for errors functions
linked_list.h heder to declare  the struct list and its union struct variables and function related to lists

bit field:
command structure build of bits field

global variabels:(4)
leble_list, print_list, lineNum, isValid

------------------------------------------------program steps(run.c)-------------------------------------------------------------

step1: loops the users entered files that was sent as argoments
step2: If the desired file is present, the program will first open the file and send it to the "turn_to_am" function, this function will 	deploy the macros and create a new file(.am) . If everything went well from here the program will only use the "after macro file(.am)"

step3: The next function in operation is "decode" function. this function Reads every line of the file and send this line to a check errors function If there are error printing errors will entered into a print list, If there are no errors in this line, we will collect the data about the line:
	label-If there is a label at the beginning of the line, we will put it in the label list
	Command-line-"decode" function will count the ic of this line.
	Instruction-Line-"decode" function will count the dc of this line. and put the data into an instruction list

step4: At this point we know the ic,dc of the file, we have the label_list, instruction_list, entry_list and print list that include the 	errors(if there are)
Therefore, you can start reading the file from the beginning and encode it. "turn_to_bit" function will read each line and With the help of the lists from step 3 and other functions will collect the information needed for encoding(command opcode,value of immidiat/register, address of labels) 

step5: in step 3 and step 4 any printing data was add into a print list with a type of this print line (error/entry/exter/bynary)
	in this step if there are errors printing in the list the function will print the errors to "stderr" and won't continue to the other printings. If there are no error-type prints, the function will create a (.ent and .ext)files for the entry and extern labels (if there are) and in the end will creat the (.ob) file To which you will print the encoding of the file
----------------------------------------------------------------------------------------------------------------------------------
*/

int main(int argc, char *argv[])
{
	char *fileName;
	int i = 1;
	FILE *f;
	int ic = 0; /* count how many code line is each line */
	int dc = 0;
	char line[MAX_LINE_LENGTH];
	char first_word_in_line[MAX_LINE_LENGTH]; /* to check if the first word in line is a macro */

	fileName = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));

	if (argc <= 1)
	{
		fprintf(stderr, "Not enough argoments as entered\n");
		return 0;
	}

	for (i = 1;i < argc; i++)
	{
		
		print_list = createList();
		copyArr(fileName, argv[i]);
		strcat(fileName,".as");

		if ((f = fopen(fileName, "r")) == NULL)
		{
			fprintf(stderr, "Error can't open this file\n");
			return 0;
		}

		strtok(fileName, ".");

		/* turn the file to be after macro spreading */
		turn_to_am(f,fileName,line, first_word_in_line);
		fclose(f);

		if ((*print_list).count > 0)
		{
			printings(fileName);
			/* free the memory that eas allocate for print_list */
			freePrintList(print_list);
			continue;
		}

		if (!(f = fopen(fileName, "r+")))
		{
			fprintf(stderr,"Eror unable to open the file\n");
			return 0;
		}

		strtok(fileName,".");
		decode(f, line, first_word_in_line, &ic, &dc);

		if (isValid != 1)
			turn_to_bit(f, line, first_word_in_line);

		fclose(f);
		strtok(fileName, ".");
		/* turn the ic and dc into astring with tub-to print in the head of the ".ob" file */
		sprintf(line, "\t%d\t%d", ic, dc); 
		insertPrint(print_list, line, ic_dc);

		/* all printing function-if there is error the function will print 
		 * all errors else the function will print to externy and entry 
		 * files the externy and entry lables and will decode the file */
		printings(fileName);

		freePrintList(print_list); /* free the memory that eas allocate for print_list */ 
	}

	return 0;

}

