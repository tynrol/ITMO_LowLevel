#include "functions.h"
#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct list
{
	int data;
	struct list* next;
} list;

list* list_create(int data)
{
	list *new_list = malloc(sizeof(struct list));
	new_list->data = data;
	new_list->next = NULL;

	return new_list;
}

list* list_add_front(int data, list* head)
{
	list* new_list = list_create(data);
	new_list->next = head;
	head = new_list;

	return head;
}

list* list_add_back(int data, list* head)
{
	list* new_list = list_create(data);
	if(head == NULL)
	{
		head = new_list;
	} else {
		list *iter = head;
		while (iter->next != NULL)
			iter = iter->next;
		iter->next = new_list;
	}
	return head;
}


int list_length(list* head)
{
	int count = 0;
	list* iter = head;
	while (iter->next != NULL)
	{
		count++;
		iter = iter->next;
	}
	return count;
}

int list_get(int index, list* head)
{
	if(list_length(head) < index)
		return 0;
	list *iter = head;
	int i;
	for(i = 0; i < index; i++)
		iter = iter->next;
	return iter->data;
}

list* list_node_at(int index, list* head)
{
        list *iter = head;
        int i;
	for(i = 0; i < index; i++)
	{
		if (iter->next == NULL) return NULL;
		iter = iter->next;
	}
	return iter;
}

void list_set(int index, int value, list* head)
{
	list* current = list_node_at(index, head);
	if(current != NULL)
		current->data = value;
}

void list_free(list* head)
{
	while(head != NULL)
	{
	list* tmp = head->next;
	free(head);
	head = tmp;
	}
}

int list_sum(list* head)
	{
	int sum = 0;
	while (head != NULL)
	{
		sum += head->data;
		head = head->next;
	}
	return sum;
}

void list_show(list* head)
{
	if (head == NULL)
		printf("You dont have elements to show\n");
	else
		printf("List: ");
	while (head != NULL)
	{
		printf("%d ", head->data);
		head = head->next;
	}
	printf("\n");
}

void foreach(list* head, void (*func) (int) )
{
	int len = list_length(head);
	int i;
	for (i=0;i<=len;i++)
		func(list_get(i, head));
}

list* map(list* head, int (*func) (int) )
{
	int len = list_length(head);
	int i;
	list* new_list = NULL;
	for(i=0;i<=len;i++)
	{
		int value = func(list_get(i, head));
		new_list = list_add_back(value, new_list);
	}
	return new_list;
}

void map_mut(list* head, int (*func) (int) )
{
	int len = list_length(head);
	int i;
	for(i=0;i<=len;i++)
	{
		int value = func(list_get(i, head));
		list_set(i, value, head);
	}
}

int foldl(list* head, int acc, int (*func) (int, int) )
{
	int len = list_length(head);
	int i;
	for(i=0;i<=len;i++)
		acc = func(list_get(i, head), acc);
	return acc;
}

list* iterate(int s, int n, int (*func) (int) )
{
	list* head = NULL;
	int i;
	for(i=0;i<=n;i++)
	{
		head = list_add_back(s, head);
		s = func(s);
	}
	return head;
}

bool save(list* head, const char* filename){
	FILE * file = fopen(filename, "w");
    	if (file == NULL)
	        return false;

    	while (head)
	{
        	if (fprintf(file, "%d ", list_get(0, head)) <= 0)
            		return false;
        	head=list_node_at(1, head);
    	}

	return !fclose(file);
}

bool load(list** head, const char* filename)
{
    	FILE * file = fopen(filename, "r");
    	int data;
    	if (file == NULL)
        	return false;
    	while (fscanf(file, "%d ", &data) == 1)
        	*head = list_add_back(data, *head);
    	if (ferror(file))
        	return false;

	return !fclose(file);
}

bool serialize(list* head, const char* filename)
{
  	FILE * file = fopen(filename, "wb");
   	if (file == NULL)
		return false;
	while (head)
	{
        	if (fprintf(file, "%d ", list_get(0, head)) <= 0)
            		return false;
        	head=list_node_at(1, head);
    	}

	return !fclose(file);
}

bool deserialize(list** head, const char* filename)
{
	FILE * file = fopen(filename, "rb");
	int data;
	if (file == NULL)
       		return false;
	while (fscanf(file, "%d", &data) == 1)
	{
        	*head = list_add_back(data, *head);
	}
	if (ferror(file))
        	return false;

	return !fclose(file);
}


void print_commands()
{
	printf("0. Print commands.\n");
	printf("1. Show the list.\n");
	printf("2. Add an element in front.\n");
	printf("3. Add an element in the back.\n");
	printf("4. Search for an element by index.\n");
	printf("5. Summarise all elemnts.\n");
	printf("6. Demonstrate higher order functions work.\n");
	printf("7. Generate n values.\n");
	printf("-1. Exit.\n");
}

void demonstrate(list* head)
{
	printf("---------------------------------------\n");
	printf("Print in line\n");
	foreach(head, print_row);
	printf("\n");
	printf("Print in column\n");
	foreach(head, print_column);

	printf("Print squares\n");
	list* new_list = map(head, square);
	list_show(new_list);
	printf("Print cubes\n");
	new_list = map(head, cube);
	list_show(new_list);

	printf("Sum: %d\n", foldl(head, 0, sum));
	printf("Min: %d\n", foldl(head, list_get(0, head), min));
	printf("Max: %d\n", foldl(head, list_get(0, head), max));

	printf("Absolute values\n");
	new_list = head;
	map_mut(new_list, abs);
	list_show(new_list);

	printf("Powers of two\n");
	new_list = iterate(1,10,pow_two);
	list_show(new_list);

	list* array = NULL;
	printf("Saving absolute values\n");
	const char* filename_txt = "filename";
	save(head, filename_txt);
	printf("Loading absolute values\n");
	load(&array, filename_txt);
	list_show(array);

	array = NULL;
	printf("Saving absolute values into bin\n");
	const char* filename_bin = "filename_bin";
	serialize(head, filename_bin);
	printf("Loading absolute values\n");
	deserialize(&array, filename_bin);
	list_show(array);

	printf("----------------------------------------\n");
}

int main(int argc, char** argv)
{
	list* head = NULL;
	int data;
	int command;
	int return_value;
	int value;

	print_commands();

	while(1)
	{
		do
		{
		printf("Enter a number of a command.\n");
		return_value = scanf("%d",&command);
		while((getchar()) != '\n');
		if (return_value == 0)
			printf("Enter a number. ex. \'4\' \n");
		}while(return_value == 0);

		switch(command)
		{
		case -1:
			list_free(head);
			return 0;
		case 0:
			print_commands();
			break;
		case 1:
			list_show(head);
			break;
		case 2:
			printf("Enter a value to add: ");
			return_value = scanf("%d", &data);
			while((getchar()) != '\n');
			if (return_value == 0)
			{
				printf("Enter a correct value.\n");
				break;
			} else {
				head = list_add_front(data, head);
				list_show(head);
				break;
			}
		case 3:
			printf("Enter a value to add: ");
			return_value = scanf("%d", &data);
			while((getchar()) != '\n');
			if (return_value == 0)
			{
				printf("Enter a correct value.\n");
				break;
			} else {
				head = list_add_back(data, head);
				list_show(head);
				break;
			}
		case 4:
			printf("Enter an index:");
			return_value = scanf("%d", &data);
			while((getchar()) != '\n');
			if (return_value == 0)
			{
				printf("Enter a correct value.\n");
				break;
			} else {
				value = list_get(data, head);
				if (value == 0)
					printf("List is too short\n");
				else
					printf("Element %d: %d \n", data, return_value);
				break;
			}
		case 5:
			data = list_sum(head);
			printf("Sum of all elements: %d \n", data);
			break;
		case 6:
			demonstrate(head);
			break;
		case 7:
			printf("How many elements to generate: ");
			int a;
			return_value = scanf("%d", &data);
			while((getchar()) != '\n');
			if (return_value == 0)
			{
				printf("Enter a correct value.\n");
				break;
			} else {
				int i;
				for(i=0;i<data;i++)
				{
					a = rand()%(21)-10;
					head = list_add_back(a,head);
				}
				list_show(head);
				break;
			}
		default:
			printf("Enter a correct number.\n");
			break;
		}
	}
	list_free(head);
	return 0;
}

