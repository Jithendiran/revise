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
#include<stdio.h>
#include<stdlib.h>
struct node {
    int value;
    struct *pre,*next;
};
typedef struct node node;

// This function will print options
void print(){
    printf("Select Option:\n1.\tInsert\n2.\tDelete\n3.\tPrint\n-1.\tExit\n");
}

/**
 * This function will allocate new memory in heap and init values
 * @returns created node
*/
node* allocateMemory() {
    node* new_node =  (node *)malloc(sizeof(node));
    new_node->value = 0;
    new_node->pre = NULL;
    new_node->next = NULL;
    return new_node;
}


/**
 * This Function is responsible for return last node
 * @param head starting node
 * @returns last node
*/
node* getLastNode(node* head){
    while (head->next){
        head = head->next;
    }
    return head;    
}

void main() {
 node* head=NULL;
    int option;
    do
    {
        print();
        scanf("%d",&option);
        switch (option)
        {
        case 1:
            printf("Insert\n");
            node* new_node = allocateMemory();
            if(head == NULL)
                head = new_node; 
            node* last_node = getLastNode(head);
            insert(last_node, new_node);
            break;
        case 2:
            printf("Enter Delete index greater that negative values:\n");
            int index=-1;
            scanf("%d",&index);
            if(index == 0) {
                node* tmp = head->next; 
                free(head);
                head = tmp;
            } else {
                node* tmp = get_node_at_index(index,head);
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