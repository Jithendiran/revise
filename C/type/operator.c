/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
void main()
{
  // , operator it will have least precedence
  int a = (3, 5, 8); // () has highest precedence over all
  // , operator will return right most value and others are rejected
  printf("a is : %d\n", a);

  a = (printf("jithendiran\n"), 27); // here the expression is exected and then left values are rejected
  printf("a is %d \n", a);
  /**
   * Output of above 2 statement
   * jithendiran
     a is 27
  */
  a = 3, 4, 5, printf("Jith\n");
  // here 3,4,5 is not inside () so 3 is stored in a others are ignored bcz , operator has least precedence and = operator has higher precedence than , operator
  // so 1st 3 is assigned to a and then other values are executed but not store in a
  // a = 3, 4, 5, printf("Jith\n"); is like (a = 3), 4, 5, printf("Jith\n");
  printf("a is %d\n", a);
  /**
   * Output of above 2 statement
   * Jith
     a is 3
  */

  // int b = 3, 4, 5; error: expected identifier or '(' before numeric constant
  //  above expression is like int b = 3, int 4, int 5; variable name cannot start with number
  // int b, c, d = 3, 4, 5;  error: expected identifier or '(' before numeric constant

  int num, var;
  num = (var = 27, var + 3);
  printf("num : %d, var : %d\n", num, var); // num : 30, var : 27

  // operator precedence ref: http://www.eecs.northwestern.edu/~wkliao/op-prec.htm (./C Operator Precedence Table.html)
}