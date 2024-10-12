/**
 * Author: E.K. Jithendiran
*/

#include<stdio.h>
#include"inline.h"
/**
 * how actual function call work's 
 * When a function is called new stack area is created execute in that stack and return result
 * 
 * if a function is inline what compiler will  do is instead of creating a stack in run time, 
 * in the time of compile it will replace the actual content of the function code to where the function is being called
 * it will increase the running speed because there is no stack, but the compiling time will be slow because compiler need to replace the code every where it is called
*/

extern inline int add(int a, int b);

/**
 * Inline function need to declar with extern keyword or the function need to be static inline
*/

inline int mul (int a, int b) {
    return a * b;
}

extern  inline int mul(int, int); // without this will throw error:  undefined reference to `mul'
// explained in inline.h

/**
 * even this function also behave like inline if compiled as speed
*/
int op(int a, int b){
    return a + b;
}

void main() {

    /**
     * inline keyword is just a hint to compiler, it is not guarentee to compile the code as inline
    */


    printf("%d\n",add(2,2)); // 4

    printf("%d\n",sub(6,3)); // 3


    printf("%d\n", mul(3,4)); // 12

    printf("%d\n",divide(5,4)); // 1
    /**
     * divide also inline, it not needed extern because it is static inline
    */

   int r = op (1,2);
   /**
    * if compiled as speed option it will like this
    * 
    * r = 1 + 2
    * 
    * compile: gcc -O2 inline.c
    * 
    * To see the difference need to compile it in assembly code and see it
    * 
    * speend : gcc -O2 -S  inline.c
    * normal :  gcc -S  inline.c
   */

  /**
   * asm code: speed
   * movl	$3, %esi 
   * 
   * you will see this above line instead of call op 
   * the compiler will precalculate the value
  */

 /**
  * asm code: normal
  * 
  * call	op
  * 
  * // the above line calling the function stack
 */

}