#ifndef ERRORS_H
#define ERRORS_H

#include "linked_list.h"

#define MAX_ERROR_LINE	(80)
#define MAX_NUM_FOR_DATA	(16383)
#define MIN_NUM_FOR_DATA	(-16384)
#define MAX_NUM_FOR_INSTRUCTION	(4095)
#define MIN_NUM_FOR_INSTRUCTION	(-4096)
#define WHITE_CHARACTER		("\t \n \v \f \r , ()")

/* Checks if the macro name is as defined */
void errorDoubleMacroName(char *macro_name, list *macro_list);

/* Checks double definition of labels */
void errorDoubleLabelName(char *label_name, list *label_list);

/* if there is an entry to a label the function checks if the label as defined if nor = error */
void checkEntryNotDefined(list *entry_label_list);
/* Checks if the label is valid */
int checkLabel(const char str[]);

/* Checks if the string is valid */
int checkString(const char str[]);

/* Checks if the numbers in data are valid */
int checkData(const char str[]);

/* Checks if the commas in data sentences are correct */
int checkDataCommas(const char s[]);

/* checks if there are incorrect commas */
int checkCommas(const char s[]);

/* Checks whether the commas are correct */
int checkvalidCommas(const char s[]);

/* Checks if the number after label # is valid */
int checkNumber(const char str[]);

/* Checks if the register is valid */
int checkRegister(const char s[]);

/* Checks whether the teaching sentences are valid */
int checkTeachingSentences(const char str[]);

/* Checks if the macro is valid */
void checkMacroName(const char *str);

#endif /* ERRORS_H */
