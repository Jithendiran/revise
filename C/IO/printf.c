#include<stdio.h>// // without this line program will execute but show's an warning for printf is not decleared
// try run without the #include<stdio.h>
void main(){
    printf("Test 1 \n"); // Test 1 
    printf("%d Test 2 \n",2); // 2 Test 2 
    printf("%d Test 3 \n",'a');// 97 Test 3
    printf("%d Test 4 : %d \n",2,3); // 2 Test 4 : 3
    printf("%d Test 5 : %d \n",2); //2 Test 5 : 0
    printf("%d Test 5 \n",2,3); // 2 Test 5
    printf("%d \n",printf("hello")); //hello5
    printf("%d \n",printf("%d ji",2)); //2 ji4
}