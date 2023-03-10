/**
 * @author: E.K.Jithendiran
 * Date: 09.03.2023
 * Queue
 * Time complexity:
 * Put O(1)
 * Get O(1)
 * Space Complexity: O(n)
 */
#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)
struct node
{
    int value;
    struct node *next;
};
typedef struct node node;

/**
 * This function will allocate new memory in heap and init values
 * @returns created node
 */
node *allocateMemory()
{
    node *new_node = (node *)malloc(sizeof(node));
    new_node->value = 0;
    new_node->next = NULL;
    return new_node;
}

// This function will print options
void print()
{
    printf("\nSelect Option:\n1.\tPut\n2.\tGet\n-1.\tExit\n\nEnter Option: ");
}

/**
 * This function is responsible creating new node and put to queue
 * @param head top of the stack
 * @returns new top
 */
node *put(node *tail)
{
    node *new_node = allocateMemory();
    printf("Enter value: ");
    scanf("%d", &new_node->value);
    if (tail == NULL)
        tail = new_node;
    else
        tail->next = new_node;
    return new_node;
}

/**
 * This function is get element from queue and deallocate fetched memory
 * @param head top of the queue
 * @returns new top
 */
node *get(node *head)
{
    if (head)
    {
        printf("%d", head->value);
        node *tmp = head;
        head = head->next;
        free(tmp);
    }
    return head;
}

void main()
{
    node *head = NULL, *tail = NULL;
    int option;
    do
    {
        print();
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Put\n");
            tail = put(tail);
            if (head == NULL)
                head = tail;
            break;
        case 2:
            printf("Get ");
            head = get(head);
            break;
        case -1:
            goto exit;
            break;
        default:
            print();
            break;
        }
    } while (option != -1);

exit:
    printf("Thank you\n");
}