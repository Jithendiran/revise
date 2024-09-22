// run: gcc container_of.c && ./a.out 
// reference: https://embetronicx.com/tutorials/p_language/c/understanding-of-container_of-macro-in-linux-kernel/
#include <stdio.h>

/**
* what is container_of ?
*       I know the address of one member in the structure. But I don’t know the address of that structure. 
*  That structure may have many members. So we can find the address of the structure using this container_of macro in Linux Kernel.
*  container_of(ptr, type, member)
*  pointer -the pointer to the member.
*  type – the type of container struct this is embedded in.
* 
* */

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({         \
    const typeof( ((type *)0)->member ) *__mptr = (ptr); \
    (type *)( (char *)__mptr - offsetof(type,member) );})

// This is to give demonstrate how container_of is working in kernel

int main(void)
{
    struct sample {
        int mem1;
        char mem2;
    };
    
    struct sample sample1;
    
    printf("Address of Structure sample1 (Normal Method) = %p\n", &sample1); // Address of Structure sample1 (Normal Method) = 0x7ffd2ceff3fc
    
    printf("Address of Structure sample1 (container_of Method) = %p\n", 
                            container_of(&sample1.mem2, struct sample, mem2)); // Address of Structure sample1 (container_of Method) = 0x7ffd2ceff3fc

    // working

    // typeof()
    /**
     * Since typeof is a compiler extension, there is not really a definition for it, 
     * but in the tradition of C it would be an operator like sizeof.
     * It takes one argument and returns the static type of the operand
    */
    char name[10] = "aa";
    typeof(name) role = "bb";
    printf("Type of : %s\n",role); // Type of : bb

    //Zero Point Reference

    //NULL and 0 are same here. It represent start of the address
    //1st null is typecasted to pointer
    //2nd get the value of specific member
    //3rd it will get the address of the specific member. It get address because it uses '&'
    //in our case int address is start from 0 and end in 3 and char is start from 4 and end in 4
    printf("Offset of the member = %d\n", &((struct sample*)NULL)->mem2); // Offset of the member = 4
    printf("Offset of the member = %d\n", &((struct sample*)0)->mem2); // Offset of the member = 4
    printf("Offset of the member = %d\n", &((struct sample*)0)->mem1); // Offset of the member = 0

    // typeof(((struct sample *)0)->mem2) * __mptr; -> char * __mptr;
    printf("Offset  = %d\n", offsetof(struct sample, mem2)); // Offset  = 4

    return 0;
}