
#include "errors.h"

void decode(FILE *f, char *line, char *first_word_in_line, int *icounter, int *dcounter)
{
	char *temp;
	int i = 0;
	list *entry_label_list;
	/* if there is just one word in line it will be insert to first_word_in_line and line_taken = OFF */
	flags line_taken = ON;
	char * label_name;
	flags flag = ON; /* if flag = 0, false the function wont count ic or dc*/
	flags label_flag = OFF; /* to tell if there is a label */
	char delime[3];
	char * copy_line;
	int ic = 0, dc = 0;
	int length;

	entry_label_list = createList();
	createLabelList();

	delime[0] = TAB;
	delime[1] = SPACE;
	delime[2] = ENTER;

	line_num = 1;
	temp = (char *) malloc((MAX_LINE_LENGTH) * sizeof(char));
	copy_line = (char *) malloc((MAX_LINE_LENGTH) * sizeof(char));
	label_name = (char *) malloc((MAX_LINE_LENGTH) * sizeof(char));

	while (!feof(f))
	{ 	
		fgets(line, MAX_LINE_LENGTH, f); /* get the rest of the line into "line"*/
		flag =prepLine(first_word_in_line, line);

		if (line_taken != OFF)
			jumpSpace(&line);

		if (strstr(first_word_in_line, "stop") || strstr(first_word_in_line, "rts"))
		{
			flag = 0;
			ic = ic + 1;
		}

		if (first_word_in_line[0] == REMARK) /*it's a remark*/
		{
			flag = OFF;
		}
		length = strlen(first_word_in_line);
		copyArr(copy_line, line);

		if (length == 0)
			flag = OFF;

		 /* the line start with a lebel so we skip the label and also skis the operation name */
		if (first_word_in_line[length - 1] == LABEL)
		{
			checkLabel(first_word_in_line); /* check if it a valid label */
			label_flag = ON;
	
			/* if there was a label in this line we will kip the label and the line in the label tab */
			if (label_flag == ON)
			{
				jumpSpace(&line);			
				copyArr(label_name, first_word_in_line);

				if (line[0] == POINT)
				{
					addToLabels(label_list,label_name, dc + 100, instruction_label);
				}
				else
					addToLabels(label_list,label_name, ic + 100, inside_label);
				
			}
			if (strstr(line, "stop") || strstr(line, "rts"))
			{
				ic = ic + 1;
				flag = 0;
			}
			jumpSpace(&line);
			if (line[0] != POINT && flag == ON) /* if it's not a guiding line we will skip the operation name */
			{
			length = strlen(strtok(copy_line, delime));
			copyArr(copy_line, line + length);
			jumpSpace(&copy_line);
			}
		}
		jumpSpace(&first_word_in_line);
		
		if (strcmp(first_word_in_line, ".extern") == 0)
		{
			jumpSpace(&line);
			checkLabel(line); /* check if it a valid label */
			copy_line = strtok(copy_line, delime);
			addToLabels(label_list, copy_line, 0, external_label);
			flag = OFF;
		}
		if (strcmp(first_word_in_line, ".entry") == 0)
		{
			jumpSpace(&line);
			copyArr(copy_line, line);
			checkLabel(line); /* check if it is valid label */
			jumpSpace(&copy_line);
			copy_line = strtok(copy_line, delime);
			addToLabels(entry_label_list, copy_line, -1, entry_label);
		}
		if ((flag == ON && line[0] == POINT) || first_word_in_line[0] == POINT)
		{
			copyArr(copy_line, line);
			jumpSpace(&copy_line);
			if (first_word_in_line[1] == DATA)
			{
				length = strlen(copy_line);

				if (checkData(copy_line) == 0)
				{
					if (checkDataCommas(copy_line) == 0)
						dc += getDataDc(copy_line);
				}
				copyArr(copy_line, copy_line + length);
			}

			if (first_word_in_line[1] == STRING)
			{
				if (checkString(copy_line) == 0)
					dc += getStrDc(copy_line);
			}
			copyArr(copy_line, line + 1);

			if (copy_line[0] == DATA)
			{			
				jumpSpace(&copy_line);
				length = strlen(copy_line + 4);

				if (checkData(copy_line + 4) == 0)
				{
					if (checkDataCommas(copy_line + 4) == 0)
						dc += getDataDc(copy_line);
				}
				copyArr(copy_line, copy_line + length);
			}
			if (copy_line[0] == STRING)
			{
				jumpSpace(&copy_line);
				if (checkString(copy_line + 6) == 0)
					dc += getStrDc(copy_line + 6);
				
			}
			flag = OFF;
		}
		length = strlen(line);
		if ((flag == ON) && length == 0)
		{
			ic += 1;
			flag = OFF; /* false */
		}

		jumpSpace(&line);

		if (line[0] != POINT && flag == ON)
			ic += 1;

		if (line[0] != REG && line[0] != IMMEDIATE && line[0] != POINT && flag == ON) /*it's a label */
		{
			ic += 1;
			copyArr(copy_line, line);
			temp = strrchr(copy_line, '('); /* get the string from the chat'(' */

			if (temp == NULL) /* if the char '(' is not after the label if the command is just a */
			{
				copyArr(copy_line, line);
				temp = strrchr(copy_line, COMMA);
				if (temp != NULL)
				{
					ic += 1;
				}
				flag = OFF; /* if the command is just a label we counted ic already */
			}
			if (flag == ON)
			{
				copyArr(line, temp);
				line = line + 1;
			}
			if ((flag == ON) && (line[0] != REG) && line[0] != IMMEDIATE && line[0] != POINT)
			{
				ic += 2;
				flag = OFF;
			}
		}
		if ((flag == ON) && (line[0] == REG)) /* check if it's a register */
		{
			copyArr(copy_line, line + REG_LENGTH);
			jumpSpace(&copy_line);
			if (copy_line[0] != END)
			{
				copyArr(copy_line, copy_line + 1); /*skip the comma*/
				jumpSpace(&copy_line);
			}
			if (copy_line[0] == REG || copy_line[0] == END) /* check if it's another register */
			{
				/* if ther is an operation name and than two register the command line will translate into tow code line */
				ic += 1;
			}
			/* after the operand reg there is an immidiate or a lebel operand-so the command line will translate
			 * into three code line
			 */
			else
			{
				ic += 2;
			}
		}
		/* check if the first operand is an immidiate - the the command line will translate to three code line */
		if ((flag == ON) && (line[0] == IMMEDIATE))
		{	
			i = 2;

			while (isdigit(copy_line[i]))
			{
				i = i + 1;
			}

			copyArr(copy_line, copy_line + i);
			jumpSpace(&copy_line);

			if (copy_line[0] == END)
			{
				ic += 1;
			}
			else
			{
				ic += 2;
			}
		}
		line_taken = 1;
		label_flag = 0;
		flag = ON;
		fseek(f, 0, SEEK_CUR);

		if (fgetc(f) == END || feof(f))
			break;
		fseek(f, -1, SEEK_CUR);
		line_num = line_num + 1;
	} /* end of while */

	*icounter = ic;
	*dcounter = dc;

	checkEntryNotDefined(entry_label_list);
	calculateInstructionLabel(label_list, ic);
	printEntry(entry_label_list);

	return;
}

void calculateInstructionLabel(list *label_list, int ic)
{
	labels *l = (*(*label_list).head).label_var;

	while (l != NULL)
	{
		if ((*l).era == instruction_label)
		{
			(*l).era = inside_label;
			(*l).label_address = (*l).label_address + ic;
		}
		l = (*l).next_label;
	}
}

/*
this function prints to the print_list all the entry labls-the print list will be print in the end of the program
*/
void printEntry(list *entry_label_list)
{
	char *temp_line;
	labels *l;
	labels *l_ent;

	l = (*(*label_list).head).label_var;
	l_ent = (*(*entry_label_list).head).label_var;

	while (l != NULL)
	{
		while (l_ent != NULL)
		{
			if (strcmp((*l).label_name, (*l_ent).label_name) == 0)
			{
				temp_line = (char *) malloc((strlen((*l).label_name) + 10) * sizeof(char));
				sprintf(temp_line, "%s\t%d", (*l).label_name, (*l).label_address);
				insertPrint(print_list, temp_line, entry); /*insert the line of entrny label into print_list*/
				free(temp_line);
			}
			l_ent = (*l_ent).next_label;
		}
		l = (*l).next_label;
		l_ent = (*(*entry_label_list).head).label_var;
	}
}

/*
This function receives an instruction statement of ".data" and summarizes the dc
*/
int getDataDc(char *line)
{
	char comma[2];
	int dc = 0;

	comma[0] = COMMA;

	jumpSpace(&line);
	strtok(line, ",");

	while (line != NULL)
	{
		dc++;
		line = strtok(NULL, comma);
	}
	return dc;
}

/**
 * This function receives an instruction statement of ".string" and summarizes the dc
 */
int getStrDc(char *line)
{
	int dc = 0;

	jumpSpace(&line);
	line = line + 1;

	while (line[0] != '"')
	{
		dc += 1;
		line = line + 1;
	}
	return dc + 1; /* because in the end of a string there is '\0' */
}

/**
 * This function puts each label in the label list and saves the label address
 */
void addToLabels(list *list_to_add, char * label_n, int ic,label_ERA era)
{
	labels *l = (labels*) malloc(sizeof(labels));

	if (label_n == NULL)
	{
		return;
	}

	jumpSpace(&label_n);

	if (label_n == NULL || strlen(label_n) == 0)
		return;

	(*l).label_name = (char *) malloc(strlen(label_n) * sizeof(char));
	if (!l)
	{
		fprintf(stderr, "No alocation\n");
		exit(0);
	}

	strtok(label_n, ":");
	(*l).label_name = copyString(label_n);
	(*l).label_address = ic;
	(*l).era = era;

	errorDoubleLabelName(label_n, list_to_add); /* check ERROR-IF THE LABEL HAS BEEN DEFINED ALREADY*/
	insertLabel(list_to_add,l);
	
	free((*l).label_name);
	free(l);
}

flags prepLine(char *first_word_in_line, char *line)
{
	int i = 0;
	int j = 0;
	int t = -1;
	flags flag;
	char error_line[MAX_ERROR_LINE];

	while (!isspace(line[i]))
		i++;

	if (line[i - 1] != LABEL)
	{
		jumpSpace(&line);
		t = checkTeachingSentences(line);
	}

	for(j = 0; j < i; j++)
	{
		first_word_in_line[j] = line[j];
	}

	first_word_in_line[j] = END;
	copyArr(line, line + j);

	if (t == -1)
	{
		jumpSpace(&line);
		t = checkTeachingSentences(line);
	}

	if (first_word_in_line[j - 1] == LABEL && (line[0] == END))
	{
			insertPrint(print_list, "Error - no command after the label", error);
			sprintf(error_line, "label name: %s	line num: %d", first_word_in_line, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
	}

	if (t == 1)
	{
		flag = OFF;
		is_valid =  1;
	}
	else
		flag = ON;

	return flag;
}

