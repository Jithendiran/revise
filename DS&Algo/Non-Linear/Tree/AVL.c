/**
 * @author E.K.Jithendiran
 * Date: 20.03.2023
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

// This function will print options
void print()
{
    printf("\nSelect Option:\n\
    1.\tInsert\n\
    2.\tDelete\n\
    3.\tInOrder\n\
    -1.\tExit\n\n\
Enter Option: ");
}

/**
 * Traverse Binary tree in IN-Order
 * Left, Root, Right
 * @param head root node
 */
void inorder(node *head)
{
    if (head->lft)
        inorder(head->lft);
    printf("%d ", head->value);
    if (head->rit)
        inorder(head->rit);
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

void main()
{
    node *head = NULL;
    int option = 0;
    int values[] = {11, 9, 10, 8, 5, 15, 12, 16};
    for (int i = 0; i < sizeof(values) / sizeof(int); i++)
    {
        head = insert(head, values[i]);
    }

    printf("\nDelete 10,8,11\n");
    int delete_value[] = {10, 8, 11};
    for (int i = 0; i < sizeof(delete_value) / sizeof(int); i++)
    {
        head = deletenode(head, delete_value[i]);
    }
    printf("Pre Order \n");
    preorder(head);
}

/**
 * Algorithm
 * Insertion
 *
 * Insert like BST
 * After insert calculate Balancing Factor
 * Balancing Factor = height of Left sub tree - height of right sub tree
 * If balancing Factor of a node is -1, 0, 1 means it is balanced else it is not balanced
 * To Balance tree we perform rotation. Rotation will take place from non balanced node. Rotation is 4 types
 * RR -> Right Right which means Inserted node's path is Right -> Right of non-balanced node
 * RL -> Right Left which means Inserted node's path is Right -> left of non-balanced node
 * LL -> Left Left which means Inserted node's path is Left -> Left of non-balanced node
 * LR -> Left Right which means Inserted node's path is Left -> Right of non-balanced node
 *
 * To find Rotation
 *  Find First direction
 *      -> If balancing factor of the node is greater than 1 means it's first direction is Left.
 *      -> If balancing factor of the node is Lesser than -1 means it's first direction is Right.
 *  Find Second direction
 *      -> If path of inserted node from imbalanced node's next node is left means it is Left.
 *      -> If path of inserted node from imbalanced node's next node is right means it is Right.
 *
 * LL(Left Left) Rotation
 *  1. Move non-balanced node->left->right to tmp
 *  2. Move non-balanced node to non-balanced node->left->right
 *  3. Move tmp to non-balanced node->left
 *  4. Return non-balanced parent address
 *  5. Perform Balancing factor calculation
 *
 * LR(Left Right) Rotation
 *  1. Move non-balanced node->left->rigth to tmp
 *  2. Move non-balanced node->left->right->left to node->left->right
 *  3. Move non-balanced node->left to tmp->left
 *  4. Move tmp to non-balanced node->left
 *  5. Perform LL
 *
 * RR(Right Right) Rotation
 *  1. Move non-balanced node->right->left to tmp
 *  2. Move non-balanced node to non-balanced node->right->left
 *  3. Move tmp to non-balanced node->right
 *  4. Return non-balaned parent address
 *  5. Perform Balancing factor calculation
 *
 * RL(Right Left) Rotation
 *  1. Move non-balanced node->right->left to tmp
 *  2. Move non-balanced node->right->left->right to node->right->left
 *  3. Move non-balanced node->right to tmp->right
 *  4. Move tmp to non-balanced node->right
 *  5. Perform RR
 */