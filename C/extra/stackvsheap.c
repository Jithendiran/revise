/*
* Author: E.K.Jithendiran
* Date: 17 Sep 2025
*/

/*
How data is stored in stack and heap
How it is accessed
Visualize with address, if impossible in address check the asm, have to prove all
*/

void func() {
    int x = 10;
    char arr[3] = "hi";
    /*
    (Stack, High → Low)
    |------------| ← SP (top of stack)
    |   arr[2]   |
    |   arr[1]   |
    |   arr[0]   |
    |     x      |
    |------------|

    */

    char *p = malloc(10);
    strcpy(p, "hi");

/*
    (Stack)
    |------------|
    |     p      | → points to heap addr
    |------------|

    (Heap)
    |------------| ← malloc returns this addr
    |  'h'       | ← p[0]
    |  'i'       | ← p[1]
    |  '\0'      | ← p[2]
    |------------|

*/

strcat(p, " man"); // how this will work?
}


/*
Memory
High Address
+---------------------+ ← Stack Top (main, then func frames)
|    Local vars       |
|    arr[3]           |
|    x = 10           |
+---------------------+
|     Free space      |
+---------------------+ ← Heap Start
|  malloc(3) → "hi\0" |
+---------------------+
|  .data / .rodata    |
|  "hi" (string literal) |
+---------------------+
|  .text (code)       |
+---------------------+
Low Address

*/

int main(){
    // stack
    int x = 4; // 4 is stored in stack
    char *y="hi"; // Pointer stored in Stack, string literal stored in Read-Only Data
    y[0] = "H"; // error
    /*
    * It is not modifiable.
    * Often shared among functions if same literal is reused.
    */

    strcat(y, " there"); // what will happen

    char a = 'a'; // how this will be stored, directly in stack
    char a1[3] = {'h', 'i', '\0'};
    char a2[3] = "hi"; // initially it is string literal then content will be copied to stack
    a2[0] = "H";


    // heap

    int *x1 = malloc(sizeof(int));
    *x1 = 4;

    char *y1 = malloc(3);  // For "hi" + '\0'
    strcpy(y1, "hi");

}