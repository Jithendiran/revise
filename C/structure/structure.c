/**
 * Author: E.K.Jithendiran
 */
#include <stdio.h>
struct
{
    char *engine;
    char *model_name;
    float fuel_capacity;
} v1, v2; // v1,v2 are global scope

struct vechicle
{
    char *type;
    float price;
    /**
     * init of any variable will be error
     * char *type="xyz" // not a valid one
     */
};
void main()
{
    // structure is a collection of various data type and grouped to an single data type
    // structire is a user defined data type

    v1.fuel_capacity = 13.5;
    v1.engine = "BSIV engine";
    v1.model_name = "Classic 350 2017";

    v2.fuel_capacity = 12.5;
    v2.engine = "BSVI engine";
    v2.model_name = "Classic 350 2022";

    printf("%d %s %f\n", v1, v1, v1); // -378851688 Classic 350 2017 0.000000
    // access structure variables by using . operator
    printf("Engine :  %s\n", v1.engine);               // Engine :  BSIV engine
    printf("Engine :  %s\n", v2.engine);               // Engine :  BSVI engine
    printf("Name :  %s\n", v1.model_name);             // Name :  Classic 350 2017
    printf("Name :  %s\n", v2.model_name);             // Name :  Classic 350 2022
    printf("Fuel Capacity :  %f\n", v1.fuel_capacity); // Fuel Capacity :  13.500000
    printf("Fuel Capacity :  %f\n", v2.fuel_capacity); // Fuel Capacity :  12.500000

    struct vechicle v11, v22; // local scope
    v11.type = "Air";
    v11.price = 12345.2;

    v22.type = "Water";
    v22.price = 23145.2;

    printf("Tyep : %s, %s\n", v11.type, v22.type); // Tyep : Air, Water

    // initilize
    struct vechicle v12 = {"Land", 343.12};
    struct vechicle v13 = {"Hybrid"};
    // struct vechicle v14 = {123.21f}; // error

    printf("V12 : %s %f\n", v12.type, v12.price); // V12 : Land 343.119995
    printf("V13 : %s %f\n", v13.type, v13.price); // V13 : Hybrid 0.000000

    struct vechicle v14 = {.price = 1.234, .type = "aa"};
    struct vechicle v15 = {.price = 2334, "bb"};
    printf("V14 : %s %f\n", v14.type, v14.price); // V14 : aa 1.234000
    printf("V15 : %s %f\n", v15.type, v15.price); // V15 : (null) 2334.000000

    // structure size
    /**
     * struct a {
     * int a; // asume int 4byte
     * char b;
     * }
     *
     * structure will get highest memory size of it's members and assign no.memebers * max member datatype size
     * steps
     * max datatype : int (4bytes)
     * no.of.members: 2
     * size of struct is = 2 * 4 = 8 bytes
     */

    struct a
    {
        int a; // assume int 4byte
        char b;
    } aa1;
    printf("Sizeof : %d\n", sizeof(aa1));                            // Sizeof : 8
    printf("Sizeof a = %d, b = %d\n", sizeof(aa1.a), sizeof(aa1.b)); // Sizeof a = 4, b = 1
    // remaining 3 bytes are padding bytes
    // ref : https://www.youtube.com/watch?v=aROgtACPjjg&list=PLBlnK6fEyqRggZZgYpPMUxdY1CYkZtARR&index=157

    struct b
    {
        char a; // 1 byte
        char b;
    } bb1;
    printf("Sizeof : %d\n", sizeof(bb1)); // Sizeof : 2

    struct stchchi
    {
        char a;
        char b;
        int c;
    } stchchivar;

    struct stchchi2
    {
        char a;
        int c;
        char b;
    } stchchivar2;

    struct stchchi3
    {
        int c;
        char a;
        char b;
    } stchchivar3;

    printf("size of stchchi = %d, stchchi2 = %d, stchchi3 = %d\n", sizeof(stchchivar), sizeof(stchchivar2), sizeof(stchchivar3)); // size of stchchi = 8, stchchi2 = 12, stchchi3 = 8

    /**
     * stchchi, stchchi2 and stchchi3
     * all the 3 structure has same 2 char and 1 int but stchchi2 size is different
     * This is default padding settings in structure for cpu optmization
     * a cpu can access a 1 word at a time, 1 word is different fot 32 bit and 64 bit machine
     * 1 word of 32 bit is 4 byte and 1 word of 64 bit is 8 byte
     *
     *
     * char is 1 byte, int is 4 byte
     * in stchchi
     *      in 1st cycle char a, char b, 2 bytes of c is accessed in 1st cycle and in second cycle remaining 2 byte of int is accessed
     *      now char a and b is fully accessed in 1st cycle but int is hafly accessed to avoid this issue empty padding of 2 byte is added after char
     *      so in 1st cycle char a,b and 2 empty byte is accessed, in 2nd cycle int is accessed
     *      so size of stchchi is char a 1byte, char 1 1byte, 2 empty byte and int c 4 byte total 8 byte
     *
     * stchchi2
     *      in 1st cycle char a + 3 empty byte, in 2nd cycle int c andin 3rd cycle char b + 3empty byte is accessed
     *      so size is char a 1byte, 3 empty byte, int c 4 byte and char b 1byte, 3 empty byte total 12 byte
     *
     * stchchi3
     *      1st cycle int c is accessed and 2nd cycle char a,b and 2 empty byte is accessed
     *      so size is int c 4 byte, char a 1 byte, char b 1 byte and 2 empty byte total 8 byte
     */

// structure packing
// https://www.youtube.com/watch?v=VZBLCpQYchs&list=PLBlnK6fEyqRggZZgYpPMUxdY1CYkZtARR&index=158
#pragma pack(1)
    struct aa
    {
        int a; // assume int 4byte
        char b;
    } aaa1;
    printf("Size of : %d\n", sizeof(aaa1)); // Size of : 5
}