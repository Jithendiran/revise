/**
 * @author: E.K.Jithendiran
 * Date: 09.03.2023
 * Stack
 * Time complexity:
 * Push O(1)
 * Pop O(1)
 * Space Complexity: O(n)
 */
#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)
struct node
{
    int value;
    struct node *pre;
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
    new_node->pre = NULL;
    return new_node;
}

// This function will print options
void print()
{
    printf("\nSelect Option:\n1.\tPush\n2.\tPop\n-1.\tExit\n\nEnter Option: ");
}

/**
 * This function is responsible creating new node and push to stack
 * @param head top of the stack
 * @returns new top
 */
node *push(node *head)
{
    node *new_node = allocateMemory();
    printf("Enter value : ");
    scanf("%d", &new_node->value);
    if (head != NULL)
    {
        new_node->pre = head;
    }
    return new_node;
}

/**
 * This function will pop the last element from stack and deallocate memory
 * @param head top of the stack
 * @returns new top
 */
node *pop(node *head)
{
    if (head)
    {
        printf("poped: %d\n", head->value);
        node *tmp = head;
        head = head->pre;
        free(tmp);
    }
    return head;
}

void main()
{
    node *head = NULL;
    int option = 0;
    do
    {
        switch (option)
        {
        case 1:
            option = 0;
            printf("Push\n");
            head = push(head);
            break;
        case 2:
            option = 0;
            printf("Pop\n");
            head = pop(head);
            break;
        case -1:
            goto exit;
            break;
        default:
            print();
            scanf("%d", &option);
            break;
        }
    } while (option != -1);

exit:
    printf("Thank you\n");
}