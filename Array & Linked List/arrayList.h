#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *array;
    // declare variables you need
    int capacity;
    int size;
    int curr;
} arrayList;

void init(arrayList *list)
{
    // implement initialization
    list->capacity = 2;
    list->size = 0;
    list->curr = -1;
    list->array = (int *)malloc(sizeof(int) * (list->capacity));
}

void free_list(arrayList *list)
{
    // implement destruction of list
    free(list->array);
    list->array = NULL;
}

void increase_capacity(arrayList *list)
{
    // implement capacity increase
    if (list->size > list->capacity / 2)
    {
        printf("Capacity increased from %d to %d\n", list->capacity, (list->capacity * 2));
        list->capacity *= 2;
        int *temp = (int *)malloc(sizeof(int) * list->capacity);

        for (int i = 0; i < list->size; i++)
        {
            temp[i] = list->array[i];
        }
        free(list->array);
        list->array = temp;
    }
}

void decrease_capacity(arrayList *list)
{
    // implement capacity decrease
    if (list->size < list->capacity / 4)
    {
        printf("Capacity decreased from %d to %d\n", list->capacity, (list->capacity / 2));
        list->capacity /= 2;
        int *temp = (int *)malloc(sizeof(int) * list->capacity);
        for (int i = 0; i < list->size; i++)
        {
            temp[i] = list->array[i];
        }
        free(list->array);
        list->array = temp;
    }
}

void print(arrayList *list)
{
    // implement list printing
    if (list->size == 0)
    {
        printf("[.]\n");
        return;
    }
    printf("[ ");

    if (list->curr == -1)
    {
        printf("| ");
    }

    for (int i = 0; i < list->size; i++)
    {
        // Print the current element with a vertical bar (|) if it's the current position
        if (i == list->curr)
            printf("%d| ", list->array[i]);
        else
        {
            printf("%d ", list->array[i]);
        }
    }
    printf("]\n");
}

void insert(int item, arrayList *list)
{
    // implement insert function
    for (int i = list->size - 1; i > list->curr; i--)
    {
        list->array[i + 1] = list->array[i];
    }
    list->array[list->curr + 1] = item;
    list->size++;
    increase_capacity(list);
    list->curr++;
    print(list);
}

int delete_cur(arrayList *list)
{
    // implement deletion of current index function
    if (list->curr<0)
    {
        printf("Invalid index.\n");
        return -1;
    }
    int del = list->array[list->curr];
    for (int i = list->curr; i < list->size - 1; i++)
    {
        list->array[i] = list->array[i + 1];
    }
    list->size--;
    if (list->curr >= list->size)
    {
        list->curr = list->size - 1;
    }
    decrease_capacity(list);
    print(list);
    return del;
}

void append(int item, arrayList *list)
{
    // implement append function
    list->array[list->size] = item;
    list->size++;
    if (list->size == 1)
    {
        list->curr = 0;
    }
    increase_capacity(list);
    print(list);
}

int size(arrayList *list)
{
    // implement size function
    return list->size;
}

void prev(int n, arrayList *list)
{
    // implement prev function
    list->curr -= n;
    if (list->curr < 0)
    {
        list->curr = -1;
    }
    print(list);
}

void next(int n, arrayList *list)
{
    // implement next function
    list->curr += n;
    if (list->curr > list->size - 1)
    {
        list->curr = list->size - 1;
    }
    print(list);
}

int is_present(int n, arrayList *list)
{
    // implement presence checking function
    for (int i = 0; i < list->size; i++)
    {
        if (list->array[i] == n)
        {
            return 1;
        }
    }
    return 0;
}

void clear(arrayList *list)
{
    // implement list clearing function
    list->size = 0;
    list->curr = -1;
    list->capacity = 2;
    print(list);
}

void delete_item(int item, arrayList *list)
{
    // implement item deletion function
    for (int i = 0; i < list->size; i++)
    {
        if (list->array[i] == item)
        {
            for (int j = i; j < list->size - 1; j++)
            {
                list->array[j] = list->array[j + 1];
            }
            list->size--;
            if (list->curr >= list->size)
            {
                list->curr = list->size - 1;
            }
            decrease_capacity(list);
            print(list);
            return;
        }
    }
    printf("%d not found\n", item);
    return;
}

void swap_ind(int ind1, int ind2, arrayList* list)
{
    // implement swap function
    if (ind1 >= 0 && ind1 < list->size && ind2 >= 0 && ind2 < list->size)
    {
        int temp = list->array[ind1];
        list->array[ind1] = list->array[ind2];
        list->array[ind2] = temp;
    }
    else
    {
        printf("Invalid input!");
    }
}

// you can define helper functions you need