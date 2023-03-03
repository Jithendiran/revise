/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package inheritance;

public class Wrapper {
    public static void main(String[] args) {
        // in java every thing is object other than primitive data types
        // primitive data types are int, float, double, char, boolean
        int i = 5; // it is not an object
        // in stack the value of primitive data types are dirctly stores

        Integer iobj2 = new Integer(25); // it is depricated

        Integer iobj = 15; // It is auto-boxing now compiler will write Integer.valueOf(15) internally  
        Integer iobj3 = Integer.valueOf(35);// boxing

        System.out.println(i);// 5
        System.out.println(iobj);// 15
        System.out.println(iobj2);// 25
        System.out.println(iobj3);// 35
        
        if(i == Integer.valueOf(5)){
            System.out.println("Same");// Same
        } else {
            System.out.println("Not same");
        }
        int j = iobj2; // //auto-unboxing, now compiler will write iobj2.intValue() internally    
        System.out.println(j);//25
        j = 4;
        System.out.println(j);//4
        System.out.println(iobj2);//25

        // to explicitily unbox
        j = iobj3.intValue();//unboxing
        System.out.println(j);// 35

        //for every primitive data type there is a wrapper class
        byte b=10;  
        short s=20;  
        long l=40;  
        float f=50.0F;  
        double d=60.0D;  
        char c='a';  
        boolean b2=true;  
        
        //Autoboxing: Converting primitives into objects  
        Byte byteobj=b;  
        Short shortobj=s;  
        Long longobj=l;  
        Float floatobj=f;  
        Double doubleobj=d;  
        Character charobj=c;  
        Boolean boolobj=b2;  
    }
}
