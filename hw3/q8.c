/*
Try out some of the other interfaces to memory allocation.
For example, create a simple vector-like data structure and
related routines that use realloc() to manage the vector.
Use an array to store the vectors elements; when a user adds
an entry to the vector, use realloc() to allocate more space
for it. How well does such a vector perform? How does it compare
to a linked list? Use valgrind to help you find bugs
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    int data;
    struct node *next;
} node;

typedef struct vector
{
    int *array;
    int size;
    int capacity;
} vector;

void vinit(vector *v)
{
    // initialize the vector
    v->size = 0;
    v->capacity = 1;
    v->array = (int *)malloc(sizeof(int));
}
void vpop(vector *v)
{
    // pop the last element
    if (v->size > 0)
    {
        v->size--;
    }
    v->array = (int *)realloc(v->array, v->size * sizeof(int));
}
void vpush(vector *v, int element)
{
    // push the element to the end of the vector
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        v->array = (int *)realloc(v->array, v->capacity * sizeof(int));
    }
    v->array[v->size] = element;
    v->size++;
}
void vclear(vector *v)
{
    // clear the vector
    free(v->array);
}
void vshow(vector *v)
{
    printf("size: %d, capacity: %d => The Value in vector are ", v->size, v->capacity);
    // show the vector
    for (int i = 0; i < v->size; i++)
    {
        printf("%d ", v->array[i]);
    }
    printf("\n");
}

void llinit(node *head)
{
    // initialize the linked list
    head->next = NULL;
}
void llshow(node *head)
{
    // show the linked list
    printf("Linked List: ");
    node *temp = head;
    while (temp != NULL)
    {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}
void llpush(node *head, int element)
{
    // insert the element to the end of the linked list
    node *temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    node *new_node = (node *)malloc(sizeof(node));
    new_node->data = element;
    new_node->next = NULL;
    temp->next = new_node;
}
void llpop(node *head)
{
    // pop the last element of the linked list
    node *temp = head;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
}
void llclear(node *head)
{
    // clear the linked list
    node *temp = head;
    while (temp != NULL)
    {
        node *temp2 = temp;
        temp = temp->next;
        free(temp2);
    }
}
int main()
{
    node *n = (node *)malloc(sizeof(node));
    n->data = 1;
    llshow(n);
    llpush(n, 2);
    llshow(n);
    llpush(n, 3);
    llshow(n);
    llpop(n);
    llshow(n);
    llpop(n);
    llshow(n);
    llclear(n);
    vector v;
    vinit(&v);
    vpush(&v, 1);
    vshow(&v);
    vpush(&v, 2);
    vshow(&v);
    vpush(&v, 3);
    vshow(&v);
    vclear(&v);
    vshow(&v);
}
