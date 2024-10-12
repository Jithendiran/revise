#include<stdio.h>
#include<string.h>
#include<stdarg.h> // it is var length arguments

int max(int n, ...);
// n is num of arguments, ... indicates it is variable length arguments

void print(char *place, ...);

void main() {
    /**
     * From printf we know in c we can use var length arguments
    */
   int res = max(2,1,2); // 1st argument 2 is num of var length, from second arg it is var length arguments
   printf("Max = %d\n",res);

    res = max(3,5,7,2);
   printf("Max = %d\n",res);

   res = max(3,5,7,2,9,5,10); // here i specified only 3 as no.of.arg other than 3 will be neglated
   printf("Max = %d\n",res);

   res = max(10,5,7,2); // giving num values is greater than actual
   printf("Max = %d\n",res); // Max = 100 here 100 is garbage

   print("ififf",1,6.7,3,4.4,7.9);
   /**
    * Op
    int = 1
    float = 6.700000
    int = 3
    float = 4.400000
    float = 7.900000
   */

/**
 * For printf it accepts different types of arguments and it's num.of.arg is also char*
*/
}

int max(int n, ...) {
    va_list args; // it has teh information to work with var length arguments
    va_start(args, n); // here 2nd argument n has the information of length about var 

   int val = 0;
    for (int i = 0; i < n; i++)
    {
      int cv = va_arg(args, int); // args is the info and 2nd argument is the type to pull from the list of arguments
      ((i == 0) && (val = cv)) || ((cv > val) && (val = cv));
    }
    va_end(args); // start and end is required
    return val;
}

void print(char *place, ...){
    // i -> int, f-> float
    int n = strlen(place);
    va_list args;
    va_start(args, place);
    for (int i = 0; i < n; i++)
    {
       if(place[i] == 'i') {
        int x = va_arg(args, int);
        printf("int = %d\n",x);
       } else if(place[i] == 'f') {
        double x = va_arg(args, double);
        printf("float = %f\n",x);
       }
    }
    
}