#ifndef LINKED_H
#define LINKED_H

#include "code.h"
#include <stddef.h>

/* var_in_list is a union that will be a varable in all types off lists */
typedef union var_in_lists
{
	print_line *print_var; /* list of all kinds of printings each printing is a print_line struct */
	labels *label_var; /* use for labels list */
	macros *macro_var; /* use for macro list */
	instructions *instruction_var; /* use for instruction list */
} var_in_list;

/*
 * a struct of list whith union var_in_list head and union var_in_list tail
 */
typedef struct linked_list list;

struct linked_list {
    var_in_list *head;
    var_in_list *tail;
    int count;
};

/**
 * globals variables-lists
 */
list *label_list;
list *print_list;
int line_num;

/**
 * this function creat the label_list alocate memory and send the label_list to createMyList function
 */
void createLabelList(void);

/*
 * a function tha creat many kinds of list allocate memmory and return this list
 * (instruction_list, macro_lists, entry_list)
 */
list* createList(void);
/*
A method that responsible for initialize the linked list each var_in_list of head and tail with NULL
*/
void createMyList(list *the_list);

/* A method that responsible for entering data into the list */
void insertLabel(list *list_to_insert,labels *data);
/*-------------------------------------------free----------------------------------------------------*/
/*
 * methods that responsible for freeing the memory that has been allocated for the list each free 
 * function will free each part of the list and in the end will send the list to a freeList funcrion
 */
void freeMacroList(list *macro_list);
void freeLabelList(list *label_list);
void freeInstructionList(list *instruction_list);
void freePrintList(list *print_list);
void freeList(list *list);
/*---------------------------------------------------------------------------------------------------*/

void insertPrint(list *print_list, char *data, print_type type);

/**
 * this function checkes if there is a macro name and if so the function prints to the file the 
 * macro command insted of the
 * macro name and if not the function return 0 = false
 * @param newf the file to print the macro command if it's a macro name
 * @param macro_list the list of macros to check if the first_word_in_line is identical to one of the macro names
 * @param first_word_in_line the string to check if its a macro name
 */
int addMacro(FILE *newf, list *macro_list, char *first_word_in_line);

/*
 * a function that insert each macro name and macro command line into macro_list
 */
void insertMacro(list *macro_list, char *data, char *name, int index);

/*
 * the function checs if its an instruction of ".data" or 
 * ".string" command and if so the function sends to "insertInstruction" function to insert into instruction list the data
 */
void addToInstruction(list *instruction_list, char type, char * line);
void insertInstruction(list *instruction_list, int data);

/**
 * This function puts each label in the label list and saves the label address
 * @param label_name-the name of the label
 * @param ic the label address
 */
void addToLabels(list *list_to_add, char * label_n, int ic,label_ERA era);
void calculateInstructionLabel(list *label_list, int ic);

/**
 * this function insert to the print_list all the entry labls-the print list will be print in the end of the program
 */
void printEntry(list *entry_label_list);

/**
 * this function sends the all the instruction data that we need to 
 * print(strings or integers data) to a printBinary function   that the printings 
 * into print_list that will be printing in the end of the program
 */
void printInstruction(list *instruction_list);

/*
 * this function prepare the line for printing
 */
void prepareLine(char *line_to_prepare, int int_to_print);

/**
 * the function returns the label address 
 */
labels* getLabelAddress(list *list_to_search, char line[]);

#endif /* LINKED_H */
