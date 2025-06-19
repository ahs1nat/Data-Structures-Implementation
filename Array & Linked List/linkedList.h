#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int element;
    struct node *next;
    struct node *prev;
} node;

typedef struct
{
    // declare head, tail, cur and other variables you need
    struct node *head;
    struct node *tail;
    struct node *cur;
    int size;

} linkedList;

void init(linkedList *list)
{
    // implement initialization
    list->head = list->tail = list->cur = NULL;
    list->size = 0;
}

void free_list(linkedList *list)
{
    // implement destruction of list
    node *temp = list->head;
    node *temp2;
    while (temp != NULL)
    {
        temp2 = temp->next;
        free(temp);
        temp = temp2;
    }
    list->head = list->tail = list->cur = NULL;
    list->size = 0;
}

void print(linkedList *list)
{
    // implement list printing
    if (list->size == 0)
    {
        printf("[.]\n");
        return;
    }

    node *temp = list->head;
    printf("[ ");

    if (list->cur == NULL)
    {
        printf("| ");
    }

    while (temp != NULL)
    {
        if (temp == list->cur)
        {
            printf("%d| ", temp->element);
        }
        else
        {
            printf("%d ", temp->element);
        }
        temp = temp->next;
    }
    printf("]\n");
}

void insert(int item, linkedList *list)
{
    // implement insert function
    node *newnode = (node *)malloc(sizeof(struct node));
    newnode->element = item;

    if (list->cur == NULL) // cur is pointing at the beginning
    {
        newnode->prev = NULL;
        newnode->next = list->head;
        if (list->head != NULL)
        {
            list->head->prev = newnode;
        }
        else
        {
            list->tail = newnode;
        }
        list->head = newnode;
    }
    else
    {
        newnode->prev = list->cur;
        newnode->next = list->cur->next;
        if (list->cur->next != NULL)
        {
            list->cur->next->prev = newnode;
        }
        else
        {
            list->tail = newnode;
        }
        list->cur->next = newnode;
    }
    list->cur = newnode;
    list->size++;
    print(list);
}

int delete_cur(linkedList *list)
{
    // implement deletion of current index function
    int del = list->cur->element;
    node *deleted = list->cur;
    if (deleted->prev != NULL)
    {
        deleted->prev->next = deleted->next;
    }
    else
    {
        list->head = deleted->next;
    }
    if (deleted->next != NULL)
    {
        deleted->next->prev = deleted->prev;
    }
    else
    {
        list->tail = deleted->prev;
    }
    if (deleted->next != NULL)
    {
        list->cur = deleted->next;
    }
    else
    {
        list->cur = deleted->prev;
    }
    free(deleted);
    list->size--;
    print(list);

    return del;
}

void append(int item, linkedList *list)
{
    // implement append function
    node *newnode = (node *)malloc(sizeof(struct node));
    newnode->element = item;
    newnode->next = NULL;
    if (list->head == NULL)
    {
        list->head = newnode;
        list->tail = newnode;
        list->cur = NULL;
        newnode->prev = NULL;
    }
    else
    {
        list->tail->next = newnode;
        newnode->prev = list->tail;
        list->tail = newnode;
    }
    list->size++;
    if(list->size==1)
    {
        list->cur = list->head;
    }
    print(list);
}

int size(linkedList *list)
{
    // implement size function
    return list->size;
}

void prev(int n, linkedList *list)
{
    // implement prev function
    int i = 0;
    while (i < n && list->cur != NULL)
    {
        list->cur = list->cur->prev;
        i++;
    }
    if (list->cur == NULL)
    {
        list->cur = list->head;
    }
    print(list);
}

void next(int n, linkedList *list)
{
    // implement next function
    int i = 0;
    if (list->cur == NULL)
    {
        list->cur = list->head;
        i = 1;
    }
    while (i < n && list->cur != NULL && list->cur->next != NULL)
    {
        list->cur = list->cur->next;
        i++;
    }
    print(list);
}

int is_present(int n, linkedList *list)
{
    // implement presence checking function
    node *temp = list->head;
    while (temp != NULL)
    {
        if (temp->element == n)
        {
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}

void clear(linkedList *list)
{
    // implement list clearing function
    free_list(list);
    init(list);
    print(list);
}

void delete_item(int item, linkedList *list)
{
    // implement item deletion function
    node *temp = list->head;
    while (temp != NULL && temp->element != item)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("%d not found.\n", item);
        return;
    }
    if (list->cur == temp)
    {
        list->cur = (temp->next != NULL) ? temp->next : temp->prev;
    }
    if (temp == list->head && temp == list->tail)
    {
        list->head = list->tail = list->cur= NULL;
    }
    else if (temp == list->head)
    {
        list->head = temp->next;
        list->head->prev = NULL;
    }
    else if (temp == list->tail)
    {
        list->tail = temp->prev;
        list->tail->next = NULL;
    }
    else
    {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }
    list->size--;
    print(list);
}

void swap_ind(int ind1, int ind2, linkedList *list)
{
    // implement swap function
    if (ind1 < 0 || ind2 < 0 || (ind1 >= list->size) || (ind2 >= list->size))
    {
        printf("Invalid indices.\n");
        return;
    }
    if (ind1 == ind2)
        return;
    node *a = list->head;
    node *b = list->head;
    for (int i = 0; i < ind1; i++)
    {
        a = a->next;
    }
    for (int i = 0; i < ind2; i++)
    {
        b = b->next;
    }

    int temp = a->element;
    a->element = b->element;
    b->element = temp;

    print(list);
}