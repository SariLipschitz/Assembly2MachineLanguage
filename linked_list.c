#include "errors.h"

list* createList(void)
{
	list *my_list = (list*) malloc(sizeof(list));

	if (my_list == NULL)
	{
		fprintf(stderr, "Not enough allocation found\n");
		exit(0);
	}

	createMyList(my_list);

	return my_list;
}


void createLableList(void)
{
	leble_list = (list*)malloc(sizeof(list));

	if (leble_list == NULL)
	{
		fprintf(stderr, "Not enough allocation found\n");
		return;
	}
	
	createMyList(leble_list);
}


void createMyList(list *the_list) 
{

	(*the_list).head = (var_in_list*)malloc(sizeof(var_in_list));
	(*the_list).tail = (var_in_list*)malloc(sizeof(var_in_list));

	(*(*the_list).head).instruction_var = NULL;
   	(*(* the_list).tail).instruction_var = NULL;	
 	(*(*the_list).head).lable_var = NULL;
   	(*(* the_list).tail).lable_var = NULL;
	(*(*the_list).head).macro_var = NULL;
   	(*(* the_list).tail).macro_var = NULL;
	(*(*the_list).head).print_var = NULL;
   	(*(* the_list).tail).print_var = NULL;

	(*the_list).count = 0;
	
}

void insertLable(list *list_to_insert, lebels *data) 
{
	lebels *l = (lebels*)malloc(sizeof(lebels));
	(*l).leble_name = (char*)malloc(strlen((*data).leble_name) * sizeof(char));
	copy_leble(l, (*data));

	if ((*(*list_to_insert).head).lable_var == NULL)
	{
		(*(*list_to_insert).head).lable_var = l;
	}

	if ((*(*list_to_insert).tail).lable_var != NULL)
		(*(*(*list_to_insert).tail).lable_var).next_leble=l;

	(*(*list_to_insert).tail).lable_var = l;
	(*list_to_insert).count++;
}

void insertMacro(list *macro_list, char *data, char *name, int index)
{
	macros *m;

	if (index == 1)
		errorDoubleMacroName(name, macro_list);

	m = (macros*)malloc(sizeof(macros));
	if (!m)
	{
		fprintf(stderr, "Not enough allocation found\n");
		exit(0);
	}

	if (!((*m).macro_line = (char*)malloc((strlen(data) + 10) * sizeof(char))))
	{
		fprintf(stderr, "Not enough allocation found\n");
		exit(0);
	}

	if (!((*m).macro_name = (char*)malloc(strlen(name) * sizeof(char))))
	{
		fprintf(stderr, "Not enough allocation found\n");
		exit(0);
	}

	(*m).next_macro = NULL;
	(*m).macro_line = copyString(data);
	(*m).macro_name = copyString(name);
	(*m).index = index;

	if ((*(*macro_list).head).macro_var == NULL)
	{
		(*(*macro_list).head).macro_var = m;
	}

	if ((*(*macro_list).tail).macro_var != NULL)
		(*(*(*macro_list).tail).macro_var).next_macro = m;

	(*(*macro_list).tail).macro_var = m;
	(*macro_list).count++;
}

void insertPrint(list *print_list, char *data,printType type)
 {
	printLine *p;

	if(!(p = (printLine*)malloc(sizeof(printLine))))
	{
		fprintf(stderr, "Not enough allocation found\n");
		return;
	}

	if(!((*p).line = (char*)malloc(strlen(data)*sizeof(char))))
	{
		fprintf(stderr, "Not enough allocation found\n");
		return;
	}

	(*p).type = type;
	(*p).line = copyString(data);
	(*p).next_print = NULL;

	if((*(*print_list).head).print_var == NULL)
	{
		(*(*print_list).head).print_var=p;
	}

	if((*(*print_list).tail).print_var != NULL)
		(*(*(*print_list).tail).print_var).next_print = p;

	(*(*print_list).tail).print_var = p;
	(*print_list).count++;

}

void insertInstruction(list *instruction_list, int data)
{
	instructions *in;

	if(!(in=(instructions*)malloc(sizeof(instructions))))
	{
		fprintf(stderr,"Not enough allocation found\n");
		return;
	}
	(*in).next_instruction=NULL;
	(*in).data=data;
	if((*(*instruction_list).head).instruction_var==NULL)
	{
		(*(*instruction_list).head).instruction_var=in;
	}
	if((*(*instruction_list).tail).instruction_var!=NULL)
		(*(*(*instruction_list).tail).instruction_var).next_instruction=in;
	(*(*instruction_list).tail).instruction_var=in;
	(*instruction_list).count++;
}

void copy_leble(lebels *target, lebels source)
{
	
	(*target).next_leble = NULL;
	(*target).leble_name=copyString(source.leble_name);
	(*target).label_address=source.label_address;
	(*target).era=source.era; 
}

void freeMacroList(list *macro_list)
{
	macros *m = (*(*macro_list).head).macro_var;

    	while (m != NULL)
	{
		free((*m).macro_name);
		free((*m).macro_line);
		m = (*m).next_macro;
		free((*(*macro_list).head).macro_var);
		(*(*macro_list).head).macro_var = m;
        
    	}
	freeList(macro_list);
}

void freeLabelList(list *label_list)
{
	lebels *l=(*(*label_list).head).lable_var;

    	while (l != NULL) 
	{
		free((*l).leble_name);
		l = (*l).next_leble;
		free((*(*label_list).head).lable_var);
		(*(*label_list).head).lable_var = l; 
	}

	freeList(label_list);
}

void freeInstructionList(list *instruction_list)
{
	instructions *in = (*(*instruction_list).head).instruction_var;

	while (in != NULL) 
	{
		in = (*in).next_instruction;
		free((*(*instruction_list).head).instruction_var);
		(*(*instruction_list).head).instruction_var = in;
	}

	freeList(instruction_list);
}

void freePrintList(list *print_list)
{
	printLine *p = (*(*print_list).head).print_var;
	while (p != NULL) 
	{
		free((*p).line);
		p = (*p).next_print;
		free((*(*print_list).head).print_var);
	       (*(*print_list).head).print_var = p;
		
	}
	freeList(print_list);
}

/*
 *each type of list will free all parts in the type list and in the end "freeList" will free the list itself
 */
void freeList(list *list)
{
	free((*list).head);
	free((*list).tail);
	free(list);
}

