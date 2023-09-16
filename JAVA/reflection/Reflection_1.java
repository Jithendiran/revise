package reflection;

import java.lang.reflect.Method;

class su {

}
class Math1 {
    Math1(int i){

    }
    // Math1() {}
    private int add(int i, int j) {
        return i + j;
    }

    public int sub(int a, int b){
        return a - b;
    }
}

class Detail extends su {
    private int a;
    public int b;
    int c;
    protected int d;

    {
        System.out.println("Instance block 1");
    }
    static {
        System.out.println("Static 1");
    }

    Detail() {
        System.out.println("Default cons");
    }

    Detail(int a) {

    }

    private Detail(int a, int b) {
        System.out.println("Private cons");
    }

    private void privateMethod() {

    }

    void defaultMethod() {

    }

    public int publicMethod() {
        return 0;
    }

    protected float protectedMethod() {
        return 0f;
    }

    {
        System.out.println("Instance block 2");
    }

    // Instance block will execute when a new instance or object is creating it will
    // execute before constructor
    // If you see decompiled part of a class file instance will be in the 1st line
    // of all constructors

}

public class Reflection_1 {
    public static void main(String[] args) throws Exception {
        // Reflection will give you information about a class

        Class c = Class.forName("reflection.Detail"); // It will load the class
        // Op: Static 1

        System.out.println(c);// class reflection.Detail

        // Detail detail = (Detail) c.newInstance(); // depricated

        Detail detail = (Detail) c.getDeclaredConstructor().newInstance(); // It will create new instance
        /**
         * Op
         * Instance block 1
         * Instance block 2
         * Default cons
         */

        // to get methods
        Method methods[] = c.getMethods(); // return only public methods of it's method and it's super methods
        for (Method method : methods) {
            System.out.println(method);
        }
        /**
         * Op
         * public int reflection.Detail.publicMethod()
         * public final void java.lang.Object.wait(long,int) throws
         * java.lang.InterruptedException
         * public final void java.lang.Object.wait() throws
         * java.lang.InterruptedException
         * public final native void java.lang.Object.wait(long) throws
         * java.lang.InterruptedException
         * public boolean java.lang.Object.equals(java.lang.Object)
         * public java.lang.String java.lang.Object.toString()
         * public native int java.lang.Object.hashCode()
         * public final native java.lang.Class java.lang.Object.getClass()
         * public final native void java.lang.Object.notify()
         * public final native void java.lang.Object.notifyAll()
         */

        methods = c.getDeclaredMethods(); // it return that particular class all method methods

        for (Method method : methods) {
            System.out.println(method);
        }

        /**
         * Op
         * private void reflection.Detail.privateMethod()
         * void reflection.Detail.defaultMethod()
         * public int reflection.Detail.publicMethod()
         * protected float reflection.Detail.protectedMethod()
         */

        // to get super class
        System.out.println(c.getSuperclass()); // class reflection.su
        // If that class extends nothing it will return
        // class java.lang.Object

        // invoke a method

       Math1 math = (Math1) Class.forName("reflection.Math1").getDeclaredConstructor(int.class).newInstance(1);
       // create instance with parameter

        c = Class.forName("reflection.Math1");
        
       Method m = c.getDeclaredMethod("sub", int.class, int.class);
       int r = (int) m.invoke(math,4, 2);
       System.out.println("Sub : "+r);// Sub : 2

       // calling private method
        m = c.getDeclaredMethod("add", int.class, int.class);
        System.out.println(m);// private int reflection.Math1.add(int,int)

        // m.invoke(math, 1,2); // err: cannot access a member of class reflection.Math1 with modifiers "private"

        m.setAccessible(true);
        r = (int)m.invoke(math, 1,2);
        System.out.println("Add : "+r); //Add : 3
       
        // Class loader will load class into JVM
        ClassLoader cls = ClassLoader.getSystemClassLoader();
        Class c2 = cls.loadClass("reflection.Detail");
        System.out.println(c2);

         c = Class.forName("reflection.Detail");
         // It uses class loader
         System.out.println(c == c2); // true
         // class loader will not load a class multiple times see `Static 1` is print 1 time only 
    }
}