/**
 * @author E.K.Jithendiran
 * Date: 15.03.2023
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
    3.\tPreOrder\n\
    4.\tInOrder\n\
    5.\tPostOrder\n\
    -1.\tExit\n\n\
Enter Option: ");
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
 * Traverse Binary tree in Post-Order
 * Left, Right, Root
 * @param head root node
 * @returns node
 */
void postorder(node *head)
{
    if (head->lft)
        postorder(head->lft);
    if (head->rit)
        postorder(head->rit);
    printf("%d ", head->value);
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
        printf("%p", head);
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
            return head;
        }
        else if (head->lft == NULL || head->rit == NULL)
        {
            // has one child
            node *tmp = head->lft ? head->lft : head->rit;
            free(head);
            return tmp;
        }
        else
        {
            // has two child
            node *tmp = findPre(head->lft); // replaceable node
            head->value = tmp->value;
            // delete tmp node
            head->lft = deletenode(head->lft, tmp->value);
            return head;
        }
    }
    else
    {
        if (head->lft)
            head->lft = deletenode(head->lft, value);
        if (head->rit)
            head->rit = deletenode(head->rit, value);
        return head;
    }
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
    //     do
    //     {
    //         print();
    //         scanf("%d", &option);
    //         switch (option)
    //         {
    //         case 1:
    //             printf("Enter Insert value : \n");
    //             int n = -1;
    //             scanf("%d", &n);
    //             head = insert(head, n);
    //             break;
    //         case 2:
    //             printf("Delete\n");
    //             break;
    //         case 3:
    //             printf("Pre Order\n");
    //             preorder(head);
    //             break;
    //         case 4:
    //             printf("In Order\n");
    //             inorder(head);
    //             break;
    //         case 5:
    //             printf("Post Order\n");
    //             postorder(head);
    //             break;
    //         case -1:
    //             goto exit;
    //             break;
    //         default:
    //             print();
    //             break;
    //         }
    //     } while (option != -1);

    // exit:
    //     printf("Thank you\n");
}