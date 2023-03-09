/**
 * @author: E.K.Jithendiran
 * Date: 08.03.2023
 * Singly Linked List
 * Time complexity:
 * Search O(n)
 * Insert O(1)
 * Delete O(1)
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
 * This function is responsible for link new node and store value
 * @param previous_node last node
 * @param new_node current node
 */
void insert(node *previous_node, node *new_node)
{

    if (previous_node != new_node)
        previous_node->next = new_node;
    printf("Enter Value:\n");
    scanf("%d", &new_node->value);
}

/**
 * This Function is responsible for return last node
 * @param head starting node
 * @returns last node
 */
node *getLastNode(node *head)
{
    while (head->next)
    {
        head = head->next;
    }
    return head;
}

// This function will print options
void print()
{
    printf("Select Option:\n1.\tInsert\n2.\tDelete\n3.\tPrint\n-1.\tExit\n");
}

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

/**
 * This function is responsible for traverse linked list and print value
 * @param head start node
 */
void traverse(node *head)
{
    /**
     * Inner Function to print node values
     * @param nodevalue node
     */
    void print_node(node * nodevalue)
    {
        printf("%d\n", nodevalue->value);
    }

    while (head && head->next)
    {
        print_node(head);
        head = head->next;
    }
    print_node(head);
}

/**
 * This function will responsible for get node at index-1
 * @param index index of node
 * @param head start node
 * @returns index matched node's previous or last node
 */
node *get_node_at_index(int index, node *head)
{
    int i = 0;
    while (head && head->next)
    {
        if (i + 1 == index)
            return head;
        head = head->next;
        i++;
    }
    return head;
}

/**
 * This function will responsible for map deleting previous node next to deleting node's next
 * @param previous_node deleting node's previous node
 */
void delete_node(node *previous_node)
{
    node *deleting_node = previous_node->next;
    node *tmp = previous_node->next ? previous_node->next->next : NULL;
    previous_node->next = tmp;
    free(deleting_node);
}

void main()
{
    node *head = NULL;
    int option;
    do
    {
        print();
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            printf("Insert\n");
            node *new_node = allocateMemory();
            if (head == NULL)
                head = new_node;
            node *last_node = getLastNode(head);
            insert(last_node, new_node);
            break;
        case 2:
            printf("Enter Delete index greater that negative values:\n");
            int index = -1;
            scanf("%d", &index);
            if (index == 0)
            {
                node *tmp = head->next;
                free(head);
                head = tmp;
            }
            else
            {
                node *tmp = get_node_at_index(index, head);
                delete_node(tmp);
            }
            break;
        case 3:
            printf("Print\n");
            traverse(head);
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