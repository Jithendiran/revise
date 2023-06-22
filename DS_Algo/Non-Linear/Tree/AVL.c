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
 * This function will get node's height
 * @param head root node
 * @returns height of the node
 */
int node_height(node *head)
{
    int lh = 0, rh = 0;
    if (head && head->lft)
    {
        lh = node_height(head->lft) + 1;
    }
    if (head && head->rit)
    {
        rh = node_height(head->rit) + 1;
    }
    if (head != NULL && !head->lft && !head->rit)
    {
        return 1;
    }

    return lh > rh ? lh : rh;
}

/**
 * This function will calculate node's balancing factor
 * Calculation: height_of_left_sub_tree - height_of_right_sub_tree
 * @param head root node
 * @return balancing factor value
 */
int get_balance(node *head)
{
    if (head == NULL)
        return 0;
    return node_height(head->lft) - node_height(head->rit);
}

/**
 * This Function will rotate in left direction
 * @param root node
 * @returns updated root node
 */
node *leftRotate(node *head)
{
    node *tmp = head->lft->rit;
    head->lft->rit = head;
    head = head->lft;
    head->rit->lft = tmp;
    return head;
}

/**
 * This Function will rotate in right direction
 * @param root node
 * @returns updated root node
 */
node *rightRotate(node *head)
{
    node *tmp = head->rit->lft;
    head->rit->lft = head;
    head = head->rit;
    head->lft->rit = tmp;
    return head;
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
    // find balance factor
    int balance = get_balance(head);
    if (balance < -1 || balance > 1)
    {
        // node not balanced

        if (balance > 0)
        {
            // left rotate

            if (head->lft->value > value)
            {
                // left left rotate
                //  if balancing factor is +ve and value is less than node->left->value
                return leftRotate(head);
            }
            else
            {
                // left right rotate
                // if balancing factor is +ve and value is greater than node->left->value
                head->lft = rightRotate(head->lft);
                return leftRotate(head);
            }
        }
        else if (balance < 0)
        {
            // right rotate

            if (head->rit->value < value)
            {
                // right right rotate
                // if balancing factor is -ve and value is greater than node->right->value
                return rightRotate(head);
            }
            else
            {
                // right left rotate
                // if balancing factor is -ve and value is lessaer than node->right->value
                head->rit = leftRotate(head->rit);
                return rightRotate(head);
            }
        }
    }
    return head;
}

/**
 * This function find predecessor node
 * @param head node
 * @returns predecessor node
 */
node *findPre(node *head)
{
    if (head->rit)
    {
        findPre(head->rit);
    }
    if (head->rit == NULL)
    {
        return head;
    }
}

/**
 * This function will delete give value's node
 * @param head root node
 * @param value value to be delete
 * @returns root node
 */
node *deletenode(node *head, int value)
{
    if (head && head->value == value)
    {
        // delete case
        if (head->lft == NULL && head->rit == NULL)
        {
            // no children
            free(head);
            head = NULL;
        }
        else if (head->lft == NULL || head->rit == NULL)
        {
            // has one child
            node *tmp = head->lft ? head->lft : head->rit;
            free(head);
            head = tmp;
        }
        else
        {
            // has two child
            node *tmp = findPre(head->lft); // replaceable node
            head->value = tmp->value;
            // delete tmp node
            head->lft = deletenode(head->lft, tmp->value);
        }
    }
    else
    {
        if (value < head->value)
            head->lft = deletenode(head->lft, value);
        else if (value > head->value)
            head->rit = deletenode(head->rit, value);
    }
    return head;
}

/**
 * This Function will balance nodes in tree
 * @param head root node
 * @returns root address
 */
node *balance_node(node *head)
{
    if (head->lft)
    {
        head->lft = balance_node(head->lft);
    }

    if (head->rit)
    {
        head->rit = balance_node(head->rit);
    }

    int balance = get_balance(head);

    if (balance < -1 || balance > 1)
    {
        // node not balanced

        if (balance > 0)
        {
            // left rotate

            if (head->lft->lft)
            {
                // left left rotate
                //  if balancing factor is +ve and value is less than node->left->value
                return leftRotate(head);
            }
            else
            {
                // left right rotate
                // if balancing factor is +ve and value is greater than node->left->value
                head->lft = rightRotate(head->lft);
                return leftRotate(head);
            }
        }
        else if (balance < 0)
        {
            // right rotate

            if (head->rit->rit)
            {
                // right right rotate
                // if balancing factor is -ve and value is greater than node->right->value
                return rightRotate(head);
            }
            else
            {
                // right left rotate
                // if balancing factor is -ve and value is lessaer than node->right->value
                head->rit = leftRotate(head->rit);
                return rightRotate(head);
            }
        }
    }
    return head;
}

void main()
{
    node *head = NULL;
    int option = 0;
    // int values[] = {21, 26, 30, 9, 4, 14, 28, 18, 15, 10, 2, 3, 7};
    int values[] = {40, 20, 10, 30, 25, 60, 45, 42, 52, 50, 55, 75, 70, 80, 85};
    for (int i = 0; i < sizeof(values) / sizeof(int); i++)
    {
        head = insert(head, values[i]);
    }

    printf("In Order \n");
    inorder(head);

    int delete_value[] = {85, 40, 60, 30, 42, 45};
    for (int i = 0; i < sizeof(delete_value) / sizeof(int); i++)
    {
        head = deletenode(head, delete_value[i]);
        head = balance_node(head);
    }
    printf("\nIn Order \n");
    inorder(head);
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