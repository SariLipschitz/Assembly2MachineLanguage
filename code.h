#ifndef CODE_H
#define CODE_H

#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define REG_LENGTH		2
#define COMMAND_NUM		16
#define DEFAULT			0
#define COMMAND_LENGTH	3
#define MAX_label_LENGTH	30
#define MAX_LINE_LENGTH		81
#define IMMEDIATE	'#'
#define REG		'r'
#define DATA	'd'
#define STRING	's'
#define POINT	'.'
#define LABEL	':'
#define REMARK	';'
#define TAB		'\t'
#define SPACE	' '
#define ENTER	'\n'
#define COMMA	','
#define END		'\0'
#define NEGETIVE	'-'
#define ASCI_ZERO	48
#define POSITIVE	'+'


/**
 * this enum replace 0 with OFF 1 with ON
 */
typedef enum {OFF, ON} flags;
/* ----------------------------------------printing-------------------------------------------------------*/
/**
 * this enum we will use for print_list. every node in the list will be a print_type and in the end of the program we
 * will first send to print all the error type-if there errors printing type in the list the program will finish. but
 * if there are no error the externy/entry and binary will be print into files
 */
typedef enum {binary, externy, entry, error, ic_dc} print_type;
typedef struct print print_line;
struct print
{
	print_line *next_print;
	char *line; /*macro line */
	print_type type;
};
/*
 * in the end of the runing program we will send (from the main) the file name this function cares about the printings * from this function we send to the function printLineType with the type we want to print
 */
void printings(char *file_name);

/**
 * this function gets what type of print_type to print and prints the print_list that are in this type
 * @param type is the type to print to externy file-the externy labels, to entry file the entry labels, 
 * errors will prin the errors to stderr and the decode to a .ob file
 * @return flags the function return ON if there was errors type in print_list. else return OFF
 */
flags printLineType(char *file_name, print_type type);
/*
 * this function loops the print_list and prints each part of the list according to the type
 */
flags printLoop(FILE *f, print_type type, char *file_name);
/* -----------------------------------------------------------------------------------------------------*/

/**
 * For each command line we will save the command OPcode and  whether there is a target operand and a source
 */
typedef struct command
{
	char command_name[5]; /* the longest command name length is "stop" */
	unsigned int op:4;
	flags operand_source; /* 0 = false there is no source operand and 1 = true */
	flags operand_des; /* 0 = false there is no destination operand and 1= true */
} commands;

/**
 * this struct keeps in each sort variable for each command line in the function the sort
 * type for each operand and the value of the operand
 */
typedef enum {midiat_sort, stright_sort, jump_sort, reg_sort, initialize} types;
typedef struct sort_type
{
	types sort_source_type; /* the type of the source operand */
	types sort_des_type; /* the type of the destination operand */
	int sort_source; /* will be the number to decode for example in case of r3-sort =3, #4-sort =4*/
	int sort_des; /* will be the number to decode for example in case of r3-sort =3= 000011, #4-sort =4= 000100*/
	int jump_address;
} sort;

/**
 * will be used for instruction list - a list for all instruction command(.data/.string) the data off the 
 * instruction command will be insert into instruction list as a integer and will be print onto .ob 
 * file in the end (after casting to binary and than to machine language
 */
typedef struct instruction instructions;
struct instruction
{
	instructions *next_instruction;
	int data;
};

/**
 * will be used for a macro list each part in the list has a macro name and the macro lines of commands 
 */
typedef struct macro macros;
struct macro
{
	macros *next_macro;
	char *macro_line; /*macro line */
	int index;
	char *macro_name;
};

/**
 * will be used for label list each part of the label list contains the label name, label address and label era
 */
typedef enum {absolut, external_label, inside_label, entry_label, instruction_label} label_ERA;
typedef struct label labels;
struct label
{
	labels *next_label;
	char *label_name; /* label name */
	int label_address; /* the address of the label */
	label_ERA era; 
};

/* a function that copies labels source into label target */
void copyLabel(labels *target,labels source);

/* A global variable that checks for errors */
int is_valid;

/**
 * This function deploys the macros-when the function see a definition of a macro  = "mcr" the functiom
 * will enter the macro name and the macro commands into the macro list. and than if there this 
 * macro name is in the file the function puts the macro command instead in the after macro file.
 * @param f the Original file
 */
int turnToAm(FILE *f, char *file_name, char line[], char first_word_in_line[]);


/**
 * this function return a copy of  the string
 */
char * copyString(const char *s);

/*
 * this function gets a pointer to a string and gumps the white flags('\t', ' ')
 */
void jumpSpace(char **s);

/**
 * this function copy s arry into arr arry
 */
void copyArr(char *arr, const char *s);


/**
 * this function prints to a temporory file the program in binary
 */
void printBinary(unsigned short a);

/**
 * this function initializes the command_tab with commandName,op, if there is a operand_source and aoperand_des
 */
void startCommandStruct(commands *command_tab, int index,
	char command_name[], char op, flags operand_source, flags operand_des);

/**
 * this function decodes into print list the opcode line of the commans
 */
void putOp(char op, types sort_source_type, types sort_des_type, int sort);

/**
 * this large function that sents from the main reads each line from the file and is encoded by auxiliary functions
 */ 
void turnToBit(FILE *f, char line[], char first_word_in_line[]);

/**
 * this function will tell the sort type of the operand in order to 
 * decode and in addition this function tells the value of the operand-for a label the function 
 * will get the label address, for a middiat or a register the function will get the register or middiat num
 */
void set_sort_type(char line[], int *sorting, int *count_decode_lines, types *sorting_type, sort sort_data);

/**
 * the next decode lines (the first decode line is in function "putOp" and this function decode according 
 * to the operand value the next decode lines
 */
void nextBinaryLine(sort sorting, int *count_decode_lines);

/**
 * this large function that is sent from the main reads each line from the file -send it to a check
 * errors function and than if there are no errors the function will count dc and ic and will 
 * collect information about each row if there is a label it will go into the label list
 */
void decode(FILE *f, char *line, char *first_word_in_line, int *icounter, int *dcounter);

/**
 * This function receives an instruction statement of ".data" and summarizes the dc
 * @param line the data to count into dc
 */
int getDataDc(char *line);

/**
 * This function receives an instruction statement of ".string" and summarizes the dc
 * @param line the string to count into dc
 */
int getStrDc(char *line);

/**
 * this function chckes if there is a command in the file line and if so-return the command op
 */
int compare(commands *command_tab, char command_name[]);

/* this function checks if there is the type to print in the print list in order not to open 
 * a (extern/entry) files if not need
 */
flags isTypeExist(print_type type);

/**
 * this function prepares the decimal line number of the .ob file
 */
void setLineNumber(char *line_number);

/**
 * the function separates the first word from the rest of the line
 */
flags prepLine(char *first_word_in_line, char *line);

#endif /* CODE_H */
