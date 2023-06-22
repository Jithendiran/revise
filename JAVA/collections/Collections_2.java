/**
 * Author: E.K.Jithendiran
 * Date: 07.03.2023
 */
package collections;

import java.util.ArrayList;

// Generics 
// require 1.5 or later
// Inside <> can be any letter
class Container<E> {
    // E is the type
    private E value;
    /**
     * This Fuction will return Type of the class
     * @return  type
     */
    String getType(){
        return  value.getClass().getName();
    }

    /**
     * This function is responsible for fetch value
     * @return
     */
    E get() {
        return value;
    }

    /**
     * This function is responsible for set value
     * @param value
     */
    void set(E value) {
        this.value = value;
    }
}

/**
 * using extend we can able to use super class and sub class type
 */
class NumberContainer<E extends Number> {
    // E is the type of only number like Interger, Float, Double
    // Integer, Float, Double all these class are extended by Number class
    private E value;

    /**
     * This function is responsible for fetch value
     * @return
     */
    E get() {
        return value;
    }

    /**
     * This function is responsible for set value
     * @param value
     */
    void set(E value) {
        this.value = value;
    }

    /**
     * This method only accept super class of E sub class of E is not supported
     * @param value
     */
    void superMethod(ArrayList <? super E> value) {
        for (Object object : value) {
            System.out.println(object);
        }
    }

    /**
     * This method only Sub class of E
     * @param value
     */
    void subMethod(ArrayList <? extends E> value) {
        for (Object object : value) {
            System.out.println(object);
        }
    }
}

class SuperSub<E> {
    /**
     * This method only accept super class of E sub class of E is not supported
     * @param value
     */
    void superMethod(ArrayList <? super E> value) {
        for (Object object : value) {
            System.out.println(object);
        }
    }

    /**
     * This method only Sub class of E
     * @param value
     */
    void subMethod(ArrayList <? extends E> value) {
        for (Object object : value) {
            System.out.println(object);
        }
    }
}

class SuperSuperClass {}

class SuperClass extends SuperSuperClass {}

class SubClass extends SuperClass {}

public class Collections_2 {
    public static void main(String[] args) {
        Container<String> cobj = new Container<>();
        cobj.set("Hi");
        System.out.println(cobj.getType());// java.lang.String

        //NumberContainer<String> cnobj = new NumberContainer<>(); // error
        NumberContainer<Integer> cnobji = new NumberContainer<>();
        NumberContainer<Float> cnobjf = new NumberContainer<>();
        NumberContainer<Double> cnobjd = new NumberContainer<>();
        NumberContainer<Number> cnobjn = new NumberContainer<>();

        cnobjn.superMethod(new ArrayList<Number>());
        //cnobjn.superMethod(new ArrayList<Integer>()); // not accepted
        cnobjn.subMethod(new ArrayList<Number>());
        cnobjn.subMethod(new ArrayList<Integer>()); // sub class of int is not supported


        SuperSub<SuperClass> sobj1 = new SuperSub();
        sobj1.superMethod(new ArrayList<SuperClass>());
        sobj1.superMethod(new ArrayList<SuperSuperClass>());
        //sobj1.superMethod(new ArrayList<SubClass>()); // not supported

        sobj1.subMethod(new ArrayList<SuperClass>());
        sobj1.subMethod(new ArrayList<SubClass>());
        //sobj1.subMethod(new ArrayList<SuperSuperClass>()); // not supported
    }
}
