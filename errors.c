#include "errors.h"


void errorDoubleMacroName(char *macro_name, list *macro_list)
{
	char *error_line = (char *) malloc((MAX_ERROR_LINE + strlen(macro_name)) * sizeof(char));
	macros *m;

	m = (*(*macro_list).head).macro_var;

	while (m != NULL)
	{
		/* This macro name has already been defined */
		if ((strcmp((*m).macro_name, macro_name) == 0) && ((*m).index == 1))
		{
			insertPrint(print_list, "Error defined macro more than one time", error);
			sprintf(error_line, "macro_name: %s", macro_name);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		m = (*m).next_macro;
	} /* end while */

	free(error_line);
}

void errorDoubleLabelName(char *label_name, list *list_to_check)
{
	char *error_line = (char *) malloc((MAX_ERROR_LINE + strlen(label_name)) * sizeof(char));

	if (getLabelAddress(list_to_check,label_name) != NULL)
	{
		insertPrint(print_list, "Error defined label more than one time", error);
		sprintf(error_line, "label name: %s	line num: %d", label_name, line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
	}
	free(error_line);
}

/* Checks if the macro is valid */
void checkMacroName(const char *str)
{
	char delime[5];
	char *s;
	int length;
	char error_line[MAX_ERROR_LINE];

	delime[0] = LABEL;
	delime[1] = SPACE;
	delime[2] = ENTER;
	delime[3] = END;
	delime[4] = TAB;
	
	s = copyString(str);
	strtok(s, delime);
	length = strlen(s);

	if ((!strcmp("mov", s)) || (!strcmp("add", s)) || (!strcmp("sub", s)) || (!strcmp("not", s)) || (!strcmp("clr", s)) 
	|| (!strcmp("inc", s)) || (!strcmp("dec", s)) || (!strcmp("red", s)) || (!strcmp("prn", s)) || (!strcmp("lea", s))
	|| (!strcmp("cmp", s)) || (!strcmp("rts", s)) || (!strcmp("jmp", s)) || (!strcmp("jsr", s)) || (!strcmp("bne", s)))
	{
			insertPrint(print_list, "Error - macro name cannot be an action", error);
			sprintf(error_line, "macro name: %s	line num: %d", s, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return;	
	}
	if ((length == 2) && (s[0] == 'r') && (isdigit(s[1])))
	{
		if ((!strcmp("r0", s)) || (!strcmp("r1", s)) || (!strcmp("r2", s)) || (!strcmp("r3", s))
		|| (!strcmp("r4", s)) || (!strcmp("r5", s)) || (!strcmp("r6", s)) || (!strcmp("r7", s)))
		{
			insertPrint(print_list, "Error - macro cannot be name of a register", error);
			sprintf(error_line, "macro name: %s	line num: %d", s, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return;	
		}
	}

	if ((!strcmp("data", s)) || (!strcmp("string", s)) || (!strcmp("entry", s)) || (!strcmp("extern", s)) || (!strcmp("stop", s)))
	{
		insertPrint(print_list, "Error - macro cannot be a reserved word in assembly", error);
		sprintf(error_line, "macro name: %s	line num: %d", s, line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(s);
		return;

	}

	free(s);
	return;
}

/* checks if there are incorrect commas */
int checkCommas(const char s[])
{
	int start = 3;
	int end;
	int i, j1 ,j2;
	int j3 = 0;
	int j4 = 0;
	char error_line[MAX_ERROR_LINE];
	char *str;
	char first;
	char second;

	str = copyString(s);
	end = strlen(str) - 1;

	while ((start <= end) && (isspace(str[start])))
		start++;
	    
	while ((end >= 0) && (isspace(str[end])))
		end--;

	if (str[start] == COMMA)
	{
		insertPrint(print_list, "Error - invalid comma before an operand", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
	}
	if (str[end] == COMMA)
	{
		insertPrint(print_list, "Error - invalid comma after an operand", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
	}
		    
	i = start;             
	while (i <= end)
	{
		while (i <= end && isspace(str[i]))
			i++;
		j1 = i;
		first = str[i];    
		i++;

		while ((i <= end) && (isspace(str[i])))
			i++;

		if (i <= end)    
			second = str[i];
		else
			second = END;

		j2 = i;

		if (first == COMMA && second == COMMA)
		{
			insertPrint(print_list, "Error - two commas", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		if ((first != COMMA) && (second != COMMA) && (second != END) && (j2-j1 != 1))
		{
			insertPrint(print_list, "Error - the operands must be separated by a comma", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			j4++;
		}

		if ((first == COMMA) || (second == COMMA))
			j3++;

	}

	if (j3 != 0)
	{
		insertPrint(print_list, "Error - incorrect commas", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(str);
		return 1;
	}

	if (j3 == 0 && j4 != 0)
	{
		insertPrint(print_list, "Error - invalid extra operand", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(str);
		return 1;
	}

	free(str);
	return 0;
}

/* Checks whether the commas are correct */
int checkvalidCommas(const char s[])
{
	int start = 3;
	int end;
	int i ;
	int j1 ,j2;
	int j3 = 0;
	int j4 = 0;

	char *str;
	char first;
	char second;
	char error_line[MAX_ERROR_LINE];
	str = copyString(s);

	end = strlen(str) - 1;

	while ((start <= end) && (isspace(str[start])))
		start++;
	    
	while ((end >= 0) && (isspace(str[end])))
		end--;

	if (str[start] == COMMA)
	{
		insertPrint(print_list, "Error - invalid comma before an operand", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
	}
		
	
	if (str[end] == COMMA)
	{
		insertPrint(print_list, "Error - invalid comma after an operand", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
	}
		    
	i = start;             
	while (i <= end)
	{
		while (i <= end && isspace(str[i]))
			i++;
		j1 = i;
		first = str[i];    
		i++;

		while (i <= end && isspace(str[i]))
		    	i++;
		    
		if (i <= end)    
		    	second = str[i];
		else
		    	second = END;
		    
		j2 = i;
		    
		if (first == COMMA && second == COMMA)
		{
			insertPrint(print_list, "Error - two commas", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		    
		if ((first != COMMA) && (second != COMMA) && (second != END) && (j2-j1 != 1))
		{
			insertPrint(print_list, "Error - the operands must be separated by a comma", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			j4++;
		}

		if ((first == COMMA) || (second == COMMA))
		    	j3++;
			
	}

	if (j3 != 2)
	{
		insertPrint(print_list, "Error - incorrect commas", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(str);
		return 1;
	}

	if ((j3 == 2) && (j4 != 0))
	{
		insertPrint(print_list, "Error - invalid extra operand", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(str);
		return 1;
	}

	free(str);
	return 0;
}

/* Checks if the number after label # is valid */
int checkNumber(const char str[])
{
	int length;
	int i;
	int val;
	char error_line[MAX_ERROR_LINE];
	char *s;

	s = copyString(str);

	length = strlen(s);

    
	if (s[0] == IMMEDIATE)
    {
		if (length < 2)
		{
			insertPrint(print_list, "Error - wrong operand", error);
			sprintf(error_line, "operand: %s	line num: %d", s, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
        	return 1;
		}
        else if (length == 2)
        {
           	if (isdigit(s[1]))
			{
				free(s);
                return 0;
			}
        	else
			{
				insertPrint(print_list, "Error - wrong operand, # must be followed by a number", error);
				sprintf(error_line, "operand: %s	line num: %d", s, line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);
				free(s);
				return 1;
			}
		}
		else if (length > 2)
		{
			if ((s[1] == NEGETIVE) || (s[1] == POSITIVE) || (isdigit(s[1])))
			{
				for(i = 2; i < length; i++)
				{
					if (!(isdigit(s[i])))
					{
						insertPrint(print_list, "Error - wrong value", error);
						sprintf(error_line, "line num: %d", line_num);
						insertPrint(print_list, error_line, error);
						insertPrint(print_list, "----------------------------------------------", error);
						free(s);
						return 1;
					}
				}
				val = strtol(s + 1, NULL, 10);

				if ((val > MAX_NUM_FOR_INSTRUCTION) || (val < MIN_NUM_FOR_INSTRUCTION))
				{
					insertPrint(print_list, "Error - number out of range", error);
					sprintf(error_line, "operand: %d	line num: %d", val, line_num);
					insertPrint(print_list, error_line, error);
					insertPrint(print_list, "----------------------------------------------", error);
					free(s);
					return 1;	
				}
				free(s);
				return 0;
			}
		}
	}

	free(s);
	return 1;
}


/* Checks if the register is valid */
int checkRegister(const char s[])
{
	if ((!strcmp(s, "r0")) || (!strcmp(s, "r1")) ||(!strcmp(s, "r2")) || (!strcmp(s, "r3"))
	|| (!strcmp(s, "r4")) || (!strcmp(s, "r5")) || (!strcmp(s, "r6")) || (!strcmp(s, "r7")))
	{
		return 0;
	}
	return 1;
}


/* Checks if the label is valid */
int checkLabel(const char str[])
{
	char delime[5];
	char *s;
	int length;
	int j = 1;
	char error_line[MAX_ERROR_LINE];

	delime[0] = LABEL;
	delime[1] = SPACE;
	delime[2] = ENTER;
	delime[3] = END;
	delime[4] = TAB;

	s = copyString(str);
	strtok(s, delime);
	length = strlen(s);

	if (length > 30)
	{
		insertPrint(print_list, "Error - the maximum length of a label is 30 characters", error);
		sprintf(error_line, "label name: %s	line num: %d", s, line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		return 1;			
	}

	if ((!strcmp("mov", s)) || (!strcmp("add", s)) || (!strcmp("sub", s)) || (!strcmp("not", s)) || (!strcmp("clr", s)) 
	|| (!strcmp("inc", s)) || (!strcmp("dec", s)) || (!strcmp("red", s)) || (!strcmp("prn", s)) || (!strcmp("lea", s))
	|| (!strcmp("cmp", s)) || (!strcmp("rts", s)) || (!strcmp("jmp", s)) || (!strcmp("jsr", s)) || (!strcmp("bne", s)))
	{
			insertPrint(print_list, "Error - label cannot be an action", error);
			sprintf(error_line, "label name: %s	line num: %d", s, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;	
	}
	if ((length == 2) && (s[0] == 'r') && (isdigit(s[1])))
	{
		if ((!strcmp("r0", s)) || (!strcmp("r1", s)) || (!strcmp("r2", s)) || (!strcmp("r3", s))
		|| (!strcmp("r4", s)) || (!strcmp("r5", s)) || (!strcmp("r6", s)) || (!strcmp("r7", s)))
		{
			insertPrint(print_list, "Error - label cannot be name of a register", error);
			sprintf(error_line, "label name: %s	line num: %d", s, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;	
		}
	}

	if ((!strcmp("data", s)) || (!strcmp("string", s)) || (!strcmp("entry", s)) || (!strcmp("extern", s)) ||
		(!strcmp("stop", s)))
	{
		insertPrint(print_list, "Error - label cannot be a reserved word in assembly", error);
		sprintf(error_line, "label name: %s	line num: %d", s, line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(s);
		return 1;

	}

	if (('A' <= s[0] && s[0] <= 'Z') || ('a'<= s[0] && s[0] <= 'z'))
	{
		while (j < length)
		{
			if ((isdigit(s[j])) || ('A'<= s[j] && s[j] <= 'Z') || ('a'<= s[j] && s[j] <= 'z'))
			{
				j++;
			}
			else
			{
				insertPrint(print_list, "Error - invalid label name", error);
				sprintf(error_line, "label name: %s	line num: %d", s, line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);
				free(s);
				return 1;
			}
		
		}
	}
	else if (s[0] == IMMEDIATE)
	{
		free(s);
		return 1;
	}
	else
	{
		insertPrint(print_list, "Error - invalid label name", error);
		sprintf(error_line, "label name: %s	line num: %d", s, line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(s);
		return 1;
	}
	free(s);
	return 0;
}


/* Checks if the string is valid */
int checkString(const char str[])
{
	int start = 0;
	int end;
	char *s;
	char error_line[MAX_ERROR_LINE];

	s = copyString(str);
	end = strlen(s) - 1;

	while ((start <= end) && (isspace(s[start])))
		start++;

	while ((end >= 0) && (isspace(s[end])))
		end--;     

	if ((s[start] != '"') || (s[end] != '"'))
	{
		insertPrint(print_list, "Error - the string must begin and end with quotation marks", error);
		sprintf(error_line, "the string: %s	line num: %d", s, line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
		free(s);
		return 1;
	}

	free(s);
	return 0;

}

/* Checks if the commas in data sentences are correct */
int checkDataCommas(const char s[])
{
	int start = 0;
	int end;
	int i;
	int j1 ,j2;
	char first;
	char second;
	char str[MAX_LINE_LENGTH];
	char error_line[MAX_ERROR_LINE];

	copyArr(str, s);
	end = strlen(str) - 1;

	while ((start <= end) && (isspace(str[start])))
		start++;

	while ((end >= 0) && (isspace(str[end])))
		end--;	

	if (str[start] == COMMA)
	{
		insertPrint(print_list, "Error - invalid comma before the first number", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
	}	
	if (str[end] == COMMA)
	{
		insertPrint(print_list, "Error - invalid comma after last number", error);
		sprintf(error_line, "line num: %d", line_num);
		insertPrint(print_list, error_line, error);
		insertPrint(print_list, "----------------------------------------------", error);
	} 
	i = start;       
	while (i <= end)
	{
		while (i <= end && isspace(str[i]))
			i++;
		j1 = i;
		first = str[i];    
		i++;

		while (i <= end && isspace(str[i]))
			i++;
			
		if (i <= end)    
			second = str[i];
		else
			second = END;
			
		j2 = i;
			
		if (first == COMMA && second == COMMA)
		{
			insertPrint(print_list, "Error - invalid commas", error); /* 2 commas */
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			return 1;
		}
			
		if ((first != COMMA) && (second != COMMA) && (second != END) && (j2-j1 != 1))
		{
			insertPrint(print_list, "Error - invalid commas", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			return 1;
		}

	}

	return 0;

}

/* Checks if the numbers in data are valid */
int checkData(const char str[])
{
	char s[MAX_LINE_LENGTH];
	char delime[4];
	char *number;
	int length = 0;
	int i = 1;
	int val;
	char error_line[MAX_ERROR_LINE];

	copyArr(s, str);

	delime[0] = COMMA;
	delime[1] = SPACE;
	delime[2] = TAB;
	delime[3] = ENTER;

	number = strtok(s, delime);

	while (number != NULL)
	{
		if (number[0] == POSITIVE || number[0] == NEGETIVE || isdigit(number[0]))
		{
			length = strlen(number);
			while (i <length)
			{
				if (!isdigit(number[i]))
				{
				insertPrint(print_list, "Error - invalid number", error);
				sprintf(error_line, "number:%c	line num: %d", number[i], line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);
				return 1;
				}
			i++;
			}

			val = strtol(number, NULL, 10);

			if (val > MAX_NUM_FOR_DATA || val < MIN_NUM_FOR_DATA)
			{
				insertPrint(print_list, "Error - number out of range", error);
				sprintf(error_line, "number: %d	line num: %d",val, line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);	
			}

		}
		else
		{
			insertPrint(print_list, "Error - invalid number", error);
			sprintf(error_line, "number:%c	line num: %d", number[0], line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);	
			return 1;
		}

		number = strtok(NULL, delime);
	}

	return 0;

}

/* Checks whether the teaching sentences are valid */

int checkTeachingSentences(const char str[])
{
	char com[4];
	char com1[5];
	char error_line[MAX_ERROR_LINE];
	char *s;	
	char delime[5];
	char *operand1;
	char *operand2;
	char *operand3;
	int i = 0;

	delime[0] = COMMA;
	delime[1] = SPACE;
	delime[2] = TAB;
	delime[3] = ENTER;
	delime[4] = END;

	s = copyString(str);

	operand3 = " ";

	com[0] = s[0];
	com[1] = s[1];
	com[2] = s[2];
	com[3] = END;

	com1[0] = s[0];
	com1[1] = s[1];
	com1[2] = s[2];
	com1[3] = s[3];
	com1[4] = '\0';


	if ((!strcmp("mov", com)) || (!strcmp("add", com)) || (!strcmp("sub", com)))
	{
		if (s[3] != SPACE && s[3] != TAB) 
		{
			insertPrint(print_list, 
				"Error - the operation name must be separated from the source operand with a space and/or a tab",
				error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);	
		}

		checkvalidCommas(s);

		operand1 = strtok(s, delime);
		while (operand1 != NULL && i < 1) 
		{
			operand1 = strtok(NULL, delime);
			operand2 = strtok(NULL, delime);
			i++;
		}

		if (operand1 == NULL || operand2 == NULL)
		{
			insertPrint(print_list, "Error - missing operand", error);
			sprintf(error_line, "operand1: %s  operand2: %s	line num: %d", operand1, operand2, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}
				
		if (checkRegister(operand1) == 0 || checkNumber(operand1) == 0 || checkLabel(operand1) == 0)
		{

			if (checkRegister(operand2) == 0 || checkLabel(operand2) == 0)
			{

				free(s);
				return 0;
			}
			else
			{
			insertPrint(print_list, "Error - The destination operand is invalid", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
				
				free(s);
				return 1;
			}
		}
		else
		{
			insertPrint(print_list, "Error - the source operand is invalid", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;

		}
		
		
	}

	else if ((!strcmp("not", com)) || (!strcmp("clr", com)) || (!strcmp("inc", com)) || (!strcmp("dec", com)) ||
		(!strcmp("red", com)))
	{
		if (s[3] != SPACE && s[3] != TAB) 
		{

			insertPrint(print_list,
			"Error - the operation name must be separated from the source operand with a space and/or a tab", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}

		checkCommas(s); 	
				
		operand1 = strtok(s, delime);
		while ( operand1 != NULL && i < 1 ) 
		{
			operand1 = strtok(NULL, delime);
			operand2 = strtok(NULL, delime);
			i++;
		}

		if (operand1 == NULL)
		{
			insertPrint(print_list, "Error - missing operand", error);
			sprintf(error_line, "the operand: %s	line num: %d", operand1, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;


		}
		if (checkRegister(operand1) == 0 || checkLabel(operand1) == 0)
		{
			if (operand2 != NULL)
			{
				insertPrint(print_list, "Error - there is no source operand for this operation name", error);
				sprintf(error_line, "line num: %d", line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);
				free(s);
				return 1;
			}
			else
			{
				free(s);
				return 0;
			}
		}
		else
		{
			insertPrint(print_list, "Error - the destination operand is invalid", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}
	}


	else if ((!strcmp("jmp", com)) || (!strcmp("bne", com)) || (!strcmp("jsr", com)))
	{

		if (s[3] != SPACE && s[3] != TAB) 
		{
			insertPrint(print_list,
				"Error - the operation name must be separated from the source operand with a space and/or a tab",
				error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}

		operand1 = strtok(s, WHITE_CHARACTER);
 
		while (operand1 != NULL && i < 1) 
		{
			operand1 = strtok(NULL, WHITE_CHARACTER);
			operand2 = strtok(NULL, WHITE_CHARACTER);
			operand3 = strtok(NULL, WHITE_CHARACTER);
			i++;
		}
		if (operand1 == NULL)
		{
			insertPrint(print_list, "Error - missing operand", error);
			sprintf(error_line, "the operan: %s 	line num: %d", operand1, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}
		
		if ((checkRegister(operand1) == 0 || checkLabel(operand1) == 0) && operand2 == NULL)
		{
			free(s);
			return 0;
		} 

		if (checkRegister(operand1) == 0 && operand2 != NULL)
		{
			insertPrint(print_list, "Error - there is no additional operand for this operation name", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		} 
		


		if (checkLabel(operand1) == 0)
		{
		
			if (operand2 == NULL)
			{
			insertPrint(print_list, "Error - missing operand", error);
			sprintf(error_line, "the operan: %s 	line num: %d", operand2, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
			}

			if (checkRegister(operand2) == 0 || checkNumber(operand2) == 0 || checkLabel(operand2) == 0)
			{

				if (operand3== NULL)
				{
					insertPrint(print_list, "Error - missing parameter", error);
					sprintf(error_line, "parameter: %s	line num: %d", operand3, line_num);
					insertPrint(print_list, error_line, error);
					insertPrint(print_list, "----------------------------------------------", error);
					free(s);
					return 1;
				}
				else if (checkRegister(operand3) == 0 || checkNumber(operand3) == 0 || checkLabel(operand3) == 0)
				{
					free(s);
					return 0;						
				}
				else
				{
					insertPrint(print_list, "Error - invalid parameter", error);
					sprintf(error_line, "parameter: %s	line num: %d", operand3, line_num);
					insertPrint(print_list, error_line, error);
					insertPrint(print_list, "----------------------------------------------", error);
					free(s);
					return 1;
				}

			}
			else
			{
				if (operand3== NULL)
				{
					insertPrint(print_list, "Error - invalid parameter", error);
					sprintf(error_line, "parameter: %s	line num: %d", operand2, line_num);
					insertPrint(print_list, error_line, error);
					insertPrint(print_list, "----------------------------------------------", error);
					
					insertPrint(print_list, "Error - missing parameter in line", error);
					sprintf(error_line, "parameter: %s	line num: %d", operand3, line_num);
					insertPrint(print_list, error_line, error);
					insertPrint(print_list, "----------------------------------------------", error);
					free(s);
					return 1;
				}
				else if (checkRegister(operand3) == 0 || checkNumber(operand3) == 0 || checkLabel(operand3) == 0)
				{
					insertPrint(print_list, "Error - invalid parameter", error);
					sprintf(error_line, "parameter: %s	line num: %d", operand2, line_num);
					insertPrint(print_list, error_line, error);
					insertPrint(print_list, "----------------------------------------------", error);
					free(s);
					return 1;
				}
				else
				{
					insertPrint(print_list, "Error - invalid parameters", error);
					sprintf(error_line, "parameters: %s   %s	 line num: %d", operand2, operand3, line_num);
					insertPrint(print_list, error_line, error);
					insertPrint(print_list, "----------------------------------------------", error);
					free(s);
					return 1;
				}

				
			}
		}
		else
		{
			insertPrint(print_list, "Error - invalid operand", error);
			sprintf(error_line, "parameters: %s	line num: %d", operand1, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}

	}


	else if (!strcmp("prn", com))
	{
		if (s[3] != SPACE && s[3] != TAB) 
		{
			insertPrint(print_list,
				"Error - the operation name must be separated from the source operand with a space and/or a tab",
				error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		checkCommas(s);
				
		operand1 = strtok(s, delime);
		while (operand1 != NULL && i < 1) 
		{
			operand1 = strtok(NULL, delime);
			operand2 = strtok(NULL, delime);
			i++;
		}

		if (operand1 == NULL)
		{
			insertPrint(print_list, "Error - missing operand", error);
			sprintf(error_line, "the operand: %s	line num: %d", operand1, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}

		if (checkRegister(operand1) == 0 || checkNumber(operand1) == 0 || checkLabel(operand1) == 0)
		{
			if (operand2 != NULL)
			{
				insertPrint(print_list, "Error - there is no source operand for this operation name", error);
				sprintf(error_line, "line num: %d", line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);
				free(s);
				return 1;
			}
			else
			{
				free(s);
				return 0;
			}
		}
		else
		{
			insertPrint(print_list, "Error - the destination operand is invalid", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;

		}
		

	}

	else if (!strcmp("cmp", com))
	{
		if (s[3] != SPACE && s[3] != TAB) 
		{
			insertPrint(print_list, "Error - the operation name must be separated from the source operand with a space and/or a tab", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		checkvalidCommas(s); 
						
		operand1 = strtok(s, delime);
		while ( operand1 != NULL && i < 1 ) 
		{
			operand1 = strtok(NULL, delime);
			operand2 = strtok(NULL, delime);
			i++;
		}

		if (operand1 == NULL || operand2 == NULL)
		{
			insertPrint(print_list, "Error - missing operand", error);
			sprintf(error_line, "operand1: %s	operand2: %s	line num: %d", operand1, operand2, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}

		if (checkRegister(operand1) == 0 || checkNumber(operand1) == 0 || checkLabel(operand1) == 0)
		{
			if (checkRegister(operand2) == 0 || checkNumber(operand2) == 0 || checkLabel(operand2) == 0)
			{
				free(s);
				return 0;
			}
			else
			{
				insertPrint(print_list, "Error - The destination operand is invalid", error);
				sprintf(error_line, "line num: %d", line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);
				free(s);
				return 1;
			}
		}
		else
		{
			insertPrint(print_list, "Error - the source operand is invalid", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;

		}
	}

	else if (!strcmp("lea", com))
	{
		if (s[3] != SPACE && s[3] != TAB) 
		{
			insertPrint(print_list, "Error - the operation name must be separated from the source operand with a space and/or a tab", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		checkvalidCommas(s);
		
		operand1 = strtok(s, delime);

		while (operand1 != NULL && i < 1) 
		{
			operand1 = strtok(NULL, delime);
			operand2 = strtok(NULL, delime);
			i++;
		}

		if (operand1 == NULL || operand2 == NULL)
		{
			insertPrint(print_list, "Error - missing operand", error);
			sprintf(error_line, "the operands: %s %s	line num: %d", operand1, operand2, line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}
		if (checkLabel(operand1) == 0)
		{
			if (checkRegister(operand2) == 0 || checkLabel(operand2) == 0)
			{
				free(s);
				return 0;
			}
			else
			{
				insertPrint(print_list, "Error - The destination operand is invalid", error);
				sprintf(error_line, "line num: %d", line_num);
				insertPrint(print_list, error_line, error);
				insertPrint(print_list, "----------------------------------------------", error);
				free(s);
				return 1;
			}
		}
		else
		{
			insertPrint(print_list, "Error - the source operand is invalid", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
			free(s);
			return 1;
		}
		
	}
	 
	else if ((!strcmp("rts", com)) || (!strcmp("stop", com1)))
	{
		int end = strlen(s) - 1;

		while ((end >= 0) && (isspace(s[end])))
			end--;
		if (s[end] == COMMA)
		{
			insertPrint(print_list, "Error - invalid comma", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		delime[0] = TAB;
		delime[1] = ENTER;
		delime[2] = SPACE;
		delime[3] = ' ';
		
		operand1 = strtok(s, delime);
		operand2 = " ";
		while ( operand1 != NULL && i < 1 ) 
		{
			operand1 = strtok(NULL, delime);
			i++;
		}
			
		if (operand1 != NULL)
		{
			insertPrint(print_list, "Error - no operands for this operation name", error);
			sprintf(error_line, "line num: %d", line_num);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}
		else
		{
			free(s);
			return 0;
		}
	}
	return 0;
}

void checkEntryNotDefined(list *entry_label_list)
{
	char error_line[MAX_ERROR_LINE];
	flags flag = OFF;
	labels *l;
	labels *l_ent;

	l = (*(*label_list).head).label_var;
	l_ent = (*(*entry_label_list).head).label_var;

	while (l_ent != NULL)
	{
		while (l != NULL)
		{
			if (strcmp((*l).label_name, (*l_ent).label_name) == 0)
			{
			flag = ON;
			}
			l = (*l).next_label;
		}
		if (flag == OFF)
		{
			insertPrint(print_list, "Error - there is an entry to an undefined label", error);
			sprintf(error_line, "label name: %s", (*l_ent).label_name);
			insertPrint(print_list, error_line, error);
			insertPrint(print_list, "----------------------------------------------", error);
		}

		l_ent = (*l_ent).next_label;
		l = (*(*label_list).head).label_var;
		flag = OFF;
	}
}
