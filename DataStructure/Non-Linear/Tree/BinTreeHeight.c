/**
 * @author E.K.Jithendiran
 * Date: 16.03.2023
 */
#include <stdio.h>
#include <stdlib.h>

#pragma pack(1)
struct node
{
    int value;
    struct node *lft, *rit;
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
    new_node->lft = NULL;
    new_node->rit = NULL;
    return new_node;
}

/**
 * Traverse Binary tree in Pre-Order
 * Root, Left, Right
 * @param head root node
 */
void preorder(node *head)
{
    printf("%d ", head->value);
    if (head->lft)
        preorder(head->lft);
    if (head->rit)
        preorder(head->rit);
}

/**
 * It will insert value to Binary Search Tree
 * @param head root node
 * @param value value to insert
 *
 */
node *insert(node *head, int value)
{
    if (head == NULL)
    {
        head = allocateMemory();
        head->value = value;
    }
    else if (value < head->value)
    {
        // left subtree
        head->lft = insert(head->lft, value);
    }
    else
    {
        // right subtree
        head->rit = insert(head->rit, value);
    }
    return head;
}

/**
 * This function will get node's height
 * @param head root node
 * @returns height of the node
 */
int node_height(node *head)
{
    int lh = 0, rh = 0;
    if (head->lft)
    {
        lh = node_height(head->lft) + 1;
    }
    if (head->rit)
    {
        rh = node_height(head->rit) + 1;
    }

    return lh > rh ? lh : rh;
}

/**
 * This function will get node by value
 * @param head root node
 * @param value desire value
 * @returns node's address
 */
node *get_node_address_by_value(node *head, int value)
{
    node *tmp = NULL;
    if (head && head->value == value)
    {
        return head;
    }
    else
    {
        if (head->lft && (head->value > value) && !tmp)
            tmp = get_node_address_by_value(head->lft, value);
        if (head->rit && (head->value < value) && !tmp)
            tmp = get_node_address_by_value(head->rit, value);
        return tmp;
    }
}

void main()
{
    node *head = NULL;
    int option = 0;
    int values[] = {70, 65, 68, 95, 99, 100, 90, 80, 79, 78, 82, 83, 84, 85, 81};
    for (int i = 0; i < sizeof(values) / sizeof(int); i++)
    {
        head = insert(head, values[i]);
    }

    printf("Pre Order \n");
    preorder(head);

    printf("\nNode\theight\n");
    for (int i = 0; i < sizeof(values) / sizeof(int); i++)
    {
        node *tmp = get_node_address_by_value(head, values[i]);
        printf("%d\t%d\n", values[i], node_height(tmp));
    }
}