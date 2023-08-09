
#include "errors.h"

#define NOT_CAMMAND_LAW	(-1)
#define DECOD_LINE		(99)

void turnToBit(FILE *f, char line[], char first_word_in_line[])
{
	int count_decod_lines = DECOD_LINE; /* counts the number of line we decode */
	int second_sort = 0; /* a variable that tells if the sorting methode is jump type */
	char *copy_str;
	char delime[3];
	int length = 0;
	int command_op = 0;
	int i = 0;
	list *instruction_list;
	commands command_tab[COMMAND_NUM]; /* there are 16 types of commands */
	flags skip_this_line = OFF;
	sort sorting;

	line_num = 1;
	instruction_list = createList();
	copy_str = (char *) malloc(MAX_LINE_LENGTH * sizeof(char));
	rewind(f);

	/* initialize the command_table */
	startCommandStruct(command_tab, 0, "mov", 0, ON, ON);
	startCommandStruct(command_tab, 1, "cmp", 1, ON ,ON);
	startCommandStruct(command_tab, 2, "add", 2, ON, ON);
	startCommandStruct(command_tab, 3, "sub", 3, ON, ON);
	startCommandStruct(command_tab, 4, "not", 4, OFF, ON);
	startCommandStruct(command_tab, 5, "clr", 5, OFF, ON);
	startCommandStruct(command_tab, 6, "lea", 6, ON, ON);
	startCommandStruct(command_tab, 7, "inc", 7, OFF, ON);
	startCommandStruct(command_tab, 8, "dec", 8, OFF, ON);
	startCommandStruct(command_tab, 9, "jmp", 9, OFF, ON);
	startCommandStruct(command_tab, 10, "bne", 10, OFF, ON);
	startCommandStruct(command_tab, 11, "red", 11, OFF, ON);
	startCommandStruct(command_tab, 12, "prn", 12, OFF, ON);
	startCommandStruct(command_tab, 13, "jsr", 13, OFF, ON);
	startCommandStruct(command_tab, 14, "rts", 14, OFF, OFF);
	startCommandStruct(command_tab, 15, "stop", 15, OFF, OFF);

	delime[0] = TAB;
	delime[1] = SPACE;
	delime[2] = ENTER;

	while (!feof(f))
	{
		fscanf(f, "%s", first_word_in_line);
		jumpSpace(&first_word_in_line);

		if (first_word_in_line[0] == REMARK)
			skip_this_line = ON;

		if (!fgets(line, MAX_LINE_LENGTH, f))
			strcpy(line, " ");

		jumpSpace(&line);
		length = strlen(first_word_in_line);
		jumpSpace(&first_word_in_line);

		if (feof(f))
			break;

		/* check if there is a command in this line and if so get the op command */
		if (skip_this_line == OFF)
			command_op = compare(command_tab, first_word_in_line);

		if (strlen(line) == 0)
		{
			skip_this_line = ON;
			if (command_op != NOT_CAMMAND_LAW)
			{
				putOp(command_tab[command_op].op, 0, 0, 0);
				count_decod_lines = count_decod_lines + 1;
			}
		}
		else
		{
			copyArr(copy_str, line);
			copy_str = strtok(copy_str, delime);
		}

		 /* check if the command is not the first_word_on_line */	
		if ((command_op == NOT_CAMMAND_LAW) && (skip_this_line == OFF))
			command_op = compare(command_tab, copy_str);

		if ((command_op != NOT_CAMMAND_LAW) && (skip_this_line == OFF))
		{
			if ((command_tab[command_op].operand_source == OFF) && (command_tab[command_op].operand_des == OFF))
			{
				skip_this_line = ON;
				sorting.sort_source_type = 0;
				sorting.sort_des_type = 0;
			}
			count_decod_lines ++;
			/* the line start with a lebel so we skip the label and also skip the operation name */
			if ((first_word_in_line[length - 1] == LABEL) && (skip_this_line == OFF))
			{
				jumpSpace(&line);
				copyArr(copy_str, line);
				/* COMMAND_LENGTH = 3 */
				copyArr(copy_str, copy_str + COMMAND_LENGTH);
				jumpSpace(&copy_str);
				/* get sort type of the operand*/
				set_sort_type(copy_str, &sorting.sort_source, &count_decod_lines, & (sorting.sort_source_type), sorting);
			}
			else if (skip_this_line == OFF) /* the first word in line is the command (and not a label) */
			{
				copyArr(copy_str, line);
				/* get sort type of the operand */
				set_sort_type(copy_str,&sorting.sort_source, &count_decod_lines, & (sorting.sort_source_type), sorting);
			}
			/* there is no operand_source in this command there is just a des operand */
			if ((command_tab[command_op].operand_source == OFF) && (skip_this_line == OFF))
			{
				sorting.sort_des_type = sorting.sort_source_type;
				sorting.sort_source_type = 0;
				if (sorting.sort_des_type == jump_sort)
				{
					sorting.jump_address = sorting.sort_source;
				}
				else
					sorting.sort_des = sorting.sort_source;
				sorting.sort_source = 0;
				skip_this_line = ON; /*we dont have to check operand des */
			}
			i = 0;

			if ((copy_str[i] == IMMEDIATE) && (copy_str[i + 1] == NEGETIVE))
				i++;

			i++; /* reg and middiat length (for examle r1/#3 */
			while (isdigit(copy_str[i]))
				i++;
			while (isalpha(copy_str[i]))
				i++;

			if ((sorting.sort_source_type != jump_sort) && (skip_this_line == OFF)
				&& (sorting.sort_source_type != initialize))
			{
				copyArr(copy_str, copy_str+ i);
				jumpSpace(&copy_str);
				copyArr(copy_str, copy_str+ 1); /* because of the comma */
				/* get sort type of the operand */
				set_sort_type(copy_str, &sorting.sort_des, &count_decod_lines, & (sorting.sort_des_type), sorting);
			}
			if (sorting.sort_des_type == jump_sort)
			{
				second_sort = jump_sort;
				copyArr(copy_str, copy_str);
				copy_str = strrchr(copy_str, '(');
				copyArr(copy_str, copy_str + 1);
				/* get sort type of the operand */
				set_sort_type(copy_str, &sorting.sort_source, &count_decod_lines, & (sorting.sort_source_type), sorting);
				copy_str = strrchr(copy_str, COMMA);
				copyArr(copy_str, copy_str + 1);
				/* get sort type of the operand */
				set_sort_type(copy_str, &sorting.sort_des, &count_decod_lines, & (sorting.sort_des_type), sorting);
			}
			/* decode the op code line-prepare and insert to print_list */
			putOp(command_tab[command_op].op, sorting.sort_source_type, sorting.sort_des_type, second_sort);
			if (second_sort == jump_sort)
			{
				printBinary(sorting.jump_address);
			}
			if (!(command_tab[command_op].operand_source == OFF && command_tab[command_op].operand_des == OFF))
				nextBinaryLine(sorting, &count_decod_lines);
		}
		if ((command_op == NOT_CAMMAND_LAW) && (skip_this_line == OFF)) /* it is not a command line */
		{
			jumpSpace(&first_word_in_line);
			jumpSpace(&line);
			if ((first_word_in_line[1] == 'e') || (line[1] == 'e')) /* it .externy or entery */
				skip_this_line = ON;
			if ((first_word_in_line[length - 1] == LABEL) && (skip_this_line == OFF))
			{
				copyArr(copy_str, line);
				jumpSpace(&copy_str);
				addToInstruction(instruction_list, copy_str[1], copy_str + 1);
			}
			else
				addToInstruction(instruction_list, first_word_in_line[1], line);
		
		}

		line_num = line_num + 1;
		skip_this_line = OFF;
		second_sort = 0;
		sorting.sort_des = -1;
		sorting.sort_source = -1;
		sorting.sort_source_type = initialize;
		sorting.sort_des_type = initialize;
	} /* end while */

	printInstruction(instruction_list);

	return;
}


void addToInstruction(list *instruction_list, char type, char * line)
{
	int integer = 0;
	int i = 0;
	flags flag = ON;

	jumpSpace(&line);

	if (type == line[0])
	/* because there was a label in this line the line that we get in this function starts with ".string" and not with the data string */
		flag = OFF;

	if (type == STRING)
	{	
		if (flag == OFF) /* we need to skip the word "string" */
			copyArr(line, line + 6);

		jumpSpace(&line);
	
		/* because of the "" */
		i = 1;
		while ((line[i] != '"') && (line[i] != END))
		{
			insertInstruction(instruction_list, (int)line[i]);
			i = i + 1;
		}
		insertInstruction(instruction_list, 0);
	}

	if (type == DATA)
	{
		if (flag == OFF) /* we need to skip the word "data" */
			line = line + 4;

		jumpSpace(&line);
		line = strtok(line, ",");

		while (line != NULL)
		{
			integer = atoi(line);
			insertInstruction(instruction_list, integer);
			line = strtok(NULL, ",");
		}
	}

}

labels* getLabelAddress(list *list_to_search, char line[])
{
	char delime[6];
	labels *l = (labels *) malloc(sizeof(labels));

	if (!l)
	{
		fprintf(stderr, "No alocation\n");
		exit(0);
	}
	
	delime[0] = TAB;
	delime[1] = SPACE;
	delime[2] = ENTER;
	delime[3] = COMMA;
	delime[4] = ')';
	delime[5] = '(';

	l = (*(*list_to_search).head).label_var;
	line = strtok(line, delime);

	while (l != NULL)
	{
		if (strcmp(line, (*l).label_name) == 0)
		{
			return l;
		}
		l = (*l).next_label;
	}

	return NULL; /* erorr */
}

void printInstruction(list *instruction_list)
{
	instructions *in;

	in = (*(*instruction_list).head).instruction_var;

	while (in != NULL)
	{
		printBinary((*in).data);
		in = (*in).next_instruction;
	}
}


