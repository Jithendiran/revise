#include<stdio.h>
int main() {
    printf("Size of Float : %d\n",sizeof(float)); // Size of Float : 4
    printf("Size of Double : %d\n",sizeof(double)); //Size of Double : 8
    printf("Size of Long Double : %d\n",sizeof(long double)); // Size of Long Double : 16
    //printf("Size of Long Float : %d\n",sizeof(long float)); //  error: both 'long' and 'float' in declaration specifiers
    // printf("Size of Long Double : %d\n",sizeof(long long double)); //error: both 'long long' and 'double' in declaration specifiers

    // float follows IEEE 754 Single Precision Floating Point
    // double follows IEEE 754 Double Precision Floating Point
    // Long double follows Extended precision Floating Point

    // Floating point representation
    // 1st  is sign
    // 2nd and 3rd is exponent
    // from 4th it is mantissa 
    float fpi =  3.1415926535897932384626433832795028841971;
    double dpi =  3.1415926535897932384626433832795028841971;
    long double ldpi =  3.1415926535897932384626433832795028841971;

    printf("Float %f value \n",fpi);//Float 3.141593 value 
    printf("double %f value \n",dpi);//double 3.141593 value
    printf("long double %f value \n",ldpi); //long double 0.000000 value

    // float will precise upto 7 digit from 1st place
    // double will precise upto 16 digit from 1st place
    // Long double will precise upto 19 digit from 1st place

    printf("Float %.16f value \n",fpi);//Float 3.1415927410125732 value -> from 8th place after . value is changed
    printf("double %.16f value \n",dpi);//double 3.1415926535897931 value 
    printf("long double %.21Lf value \n",ldpi); //long double 3.141592653589793115998 value  


    int var = 4/9;
    printf("%d\n",var); // 0

    float var1 = 4/9;
    printf("%.2f\n",var1);// 0.00

    float var2 = 4.0/9.0;
    printf("%.2f\n",var2);// 0.44

    float var3 = 4.0/9;
    printf("%.2f\n",var3);// 0.44

    float var4 = 4/9.0;
    printf("%.2f\n",var4);// 0.44

    // default division of decimal value is double to get float use f in suffix
    float var5 = 4.0f/9.0f;
    printf("%.2f\n",var5);// 0.44
}