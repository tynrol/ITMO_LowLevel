#include <stdio.h>
#include <malloc.h>

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

void list_free(list* head)
{
	while(head != NULL)
	{
	list* tmp = head->next;
	free(head);
	head = tmp;
	}
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

void print_commands()
{
	printf("0. Print commands.\n");
	printf("1. Show the list.\n");
	printf("2. Add an element in front.\n");
	printf("3. Add an element in the back.\n");
	printf("4. Search for an element by index.\n");
	printf("5. Summarise all elemnts.\n");
	printf("-1. Exit.\n");
}

int main(int argc, char** argv)
{
	list* head = NULL;
	int data;
	int command;
	int return_value;

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
			scanf("%d", &data);
			head = list_add_front(data, head);
			list_show(head);
			break;
		case 3:
			printf("Enter a value to add: ");
			scanf("%d", &data);
			head = list_add_back(data, head);
			list_show(head);
			break;
		case 4:
			printf("Enter an index:");
			scanf("%d", &data);
			return_value = list_get(data, head);
			if (return_value == 0)
				printf("List is too short\n");
			else
				printf("Element %d: %d \n", data, return_value);
			break;
		case 5:
			data = list_sum(head);
			printf("Sum of all elements: %d \n", data);
			break;
		}
	}
	list_free(head);
	return 0;
}
