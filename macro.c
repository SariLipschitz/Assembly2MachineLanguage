#include "errors.h"
#define EROR	(-1)

/*
This function deploys the macros-when the function see a definition of a macro= "mcr" the functiom will enter the macro name and the macro commands into the macro list. and than if thre this macro name is in the file the function puts the macro command insted in the after macro file.
@param f the Original file
*/
int turnToAm(FILE *f, char *file_name, char line[], char first_word_in_line[])
{
	int index = 0;
	FILE *newf;
	typedef enum {NO_LINE, FULL_LINE} lines;
	char delime[4];
	list *macro_list;
	lines line_taken = FULL_LINE; /* if line did not taken there is one word in a line-we have to check what*/
	char *name;
	flags flag = ON;
	char *ENDMACRO = "endmcr";
	char *MACRO = "mcr"; /* define how macro line starts */

	line_num = 0;
	strcat(file_name, ".am");
	if ((newf = fopen(file_name, "w+b")) == NULL)
	{
		fprintf(stderr, "Error  unable to open the file\n");
		exit(0);
	}

	delime[0] = TAB;
	delime[1] = SPACE;
	delime[2] = ENTER;
	delime[3] = COMMA;

	fscanf(f, "%s", first_word_in_line); /* get the first word of the line */
	macro_list = createList();

	if (fgetc(f) != ENTER)
		{
		fseek(f, -1, SEEK_CUR);
		fgets(line, MAX_LINE_LENGTH, f); /* get the rest of the line into "line"*/
		}
	else
		line_taken = NO_LINE;
	do
	{
		if (strcmp(first_word_in_line, MACRO) == 0) /* check if the first word of line is "mcr"*/
		{
			index = 1;
			jumpSpace(&line);
			name = copyString(line);
			name = strtok(name, delime);
			name[strlen(name)] = END;

			checkMacroName(name);
			fgets(line, MAX_LINE_LENGTH, f); /* get into line the next line */
			while (strstr(line,ENDMACRO) == NULL)
			{	
				fseek(f, 0, SEEK_CUR);
				jumpSpace(&line);
				line[strlen(line) - 1] = SPACE;
				insertMacro(macro_list, line, name, index);
				fgets(line, MAX_LINE_LENGTH, f); /* get into line the next line */
				index = index + 1;
			}
			free(name);
		}
		else /* the first word of the line is not "mcr" */
		{
			jumpSpace(&first_word_in_line);
			if (first_word_in_line[0] == REMARK)
				flag = OFF;
			if (!addMacro(newf, macro_list, first_word_in_line) && flag == ON) /* if there is a macro name the function will enter the macro command insted*/
			{
				if (strstr(first_word_in_line, "stop") ||line_taken == 0 || strstr(first_word_in_line, "rts"))
					fprintf(newf, "%s\n", first_word_in_line);
				else	
				{
					fprintf(newf, "%s", first_word_in_line);
					fprintf(newf, "%s", line);
				}
				
			}
			else if (flag == ON) /* if the line starts with a macro name */
			{
				name = (char *) realloc(0, sizeof(char));
				jumpSpace(&line);
				name = copyString(line);
				name = strtok(name, delime);
				while (name != NULL)
				{
					jumpSpace(&name);
					addMacro(newf, macro_list, name);
					name = strtok(NULL, delime);
				}
				fprintf(newf, "\n"); /*add the enter after inserting a macro (took it out by saving) */
				
			}			
		} /* end else */
		flag = ON;
		line_taken = FULL_LINE;
		line_taken = 1;
		if (feof(f) || fgetc(f) == END)
			break;
		fseek(f, -1, SEEK_CUR);
		fscanf(f, "%s", first_word_in_line);
		if (fgetc(f) != ENTER && fgetc(f) != END && !feof(f))
			{
			fseek(f, -2, SEEK_CUR);
			fgets(line, MAX_LINE_LENGTH, f); /* get the rest of the line into "line"*/
			}
		else
			line_taken = 0;
		line_num = line_num + 1;
	} while (!feof(f));

	fclose(newf);
	freeMacroList(macro_list);

	return 0;
}

