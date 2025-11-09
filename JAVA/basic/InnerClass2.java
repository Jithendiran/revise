/**
 * Author: E.K.Jithendiran
 * Date: 09.11.2025
 */
/*
This program shows the static and inner class

Static block will execute only when class is loaded for 1st time, if class is simply present it didn't used it won't create static memory
eg: InnerNonStat2

Static will always have only one reference 
eg: InnerNonStat is not a static inner class, meaning it's object will be changed for each outer class object
but InnerNonStat's static variable is remains same no matter how many outer object is created

all the used static block is used only once

From Outer.InnerNonStat static block we can't access Outer class's data member, even though Outer.InnerNonStat is non static

Inner static class can access only outer class static properties 
*/
class Outer {
    public int a = 1;
    public static final int os = 2;
    
    static {
        System.out.println("----------Outer---------------");
    }
    
    public class InnerNonStat {
        static {// from java 16 only, inner class static is allowed
            System.out.println("----------InnerNonStat");
            // System.out.println(Outer.a); // error: non-static variable a cannot be referenced from a static context
        }
        public int a = 3;
        public static  int Is = 4; // This will fail in earlier version of java 16

        /*
        **Java 16 earlier only**
        If you want to compile earlier java 16, static block has to be removed
        static is supported only with the final

        public int a = 3;
        public static final int Is = 4;

        why static block not supported?
        A class can have multiple static blocks, so it is possible to change object created in static block from other static block, to eliminate this they might blocked static block (This is my assumption)
        */
    }
    
    public class InnerNonStat2 {
        static { 
            System.out.println("----------InnerNonStat2"); // It didn't called because we haven't used in the program, This is getting initialized once when we use the class
        }
    }
    
    static class InnerStat {
        static {
            System.out.println("InnerStat----------");
        }
        public int a = 5;
        public static  int Iss = 6;
    }
    
}


class InnerClass2 {
    public static void main(String[] args) {
        Outer Oobj = new Outer();
        Outer.InnerNonStat inn =  Oobj.new InnerNonStat(); // object creation for non static
        System.out.println("Outer : "+Oobj);
        
        System.out.println("Obj  inner static" + inn.Is);
        inn.Is += 1;
        
        Outer Oobj1 = new Outer();
        Outer.InnerNonStat inn1 = Oobj1.new InnerNonStat(); // new Oobj1.InnerNonStat(); is not valid because new applied to Oobj1, which is already a object
        // InnerNonStat is tied to object of outer class

        System.out.println("Outer : "+Oobj1); // different object compared to above one
        
        // even though InnerNonStat intialized 2 times, it's class is loaded only once and it's static will have only one refrence 
        
        System.out.println("Obj 1 inner static" + inn1.Is);
        
        // inn1.Is and inn1.Is points to the same variable
        
        Outer.InnerStat instat0 = new Outer.InnerStat(); // object creation for static
        // Outer.new InnerStat(); is not valid because InnerStat is not a data member like InnerNonStat, it is static class, it has to create using new Outer.InnerStat
        instat0.a = 11;
        System.out.println("Outer.InnerStat 0 " + instat0);
        System.out.println("Outer.InnerStat 0 .a : "+ instat0.a);
        System.out.println("Outer.InnerStat.Iss : "+ Outer.InnerStat.Iss);
        Outer.InnerStat.Iss = 9;
        
        Outer.InnerStat instat1 = new Outer.InnerStat();
        System.out.println("Outer.InnerStat 1 " + instat1);
        System.out.println("Outer.InnerStat 1 .a : "+ instat1.a);
        System.out.println("Outer.InnerStat.Iss : "+ Outer.InnerStat.Iss);
    }
}

/*
----------Outer---------------
----------InnerNonStat
Outer : Outer@15db9742
Obj  inner static4
Outer : Outer@5cad8086
Obj 1 inner static5
InnerStat----------
Outer.InnerStat 0 Outer$InnerStat@61bbe9ba
Outer.InnerStat 0 .a : 11
Outer.InnerStat.Iss : 6
Outer.InnerStat 1 Outer$InnerStat@610455d6
Outer.InnerStat 1 .a : 5
Outer.InnerStat.Iss : 9
*/