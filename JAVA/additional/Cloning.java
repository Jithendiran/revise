/**
 * Author: E.K.Jithendiran
 * Date: 03.03.2023
 */
package additional;

class Aclone implements Cloneable{
    int i;
    public String toString() {
        return ""+i;
    }

    @Override// over ride from Object class
    // all class in java is extended by Object class by default
    public Object clone() throws CloneNotSupportedException{
        return super.clone();
    }
}

public class Cloning {
    /**
     * cloning a object is 3 types
     *  Shallow copy : means 2 or more variables point to single object
     *  Deep copy : manually create object and copy value
     *  Cloning : using inbuilt function seprate object is creatd 
     */
    public static void main(String[] args) throws CloneNotSupportedException{
        Aclone a = new Aclone();
        a.i = 1;

        //shallow copy
        Aclone a1 = a;
        a1.i = 2;
        System.out.println(a);//2

        // deep copy
        Aclone a2 = new Aclone();
        a2.i = a1.i;
        a2.i = 3;
        System.out.println("A1 : "+a1+" A2 : "+a2);//A1 : 2 A2 : 3

        // cloning
        Aclone a3 = (Aclone) a1.clone();
        a3.i = 4;
        System.out.println("A1 : "+a1+" A3 : "+a3); //A1 : 2 A3 : 4
    }
}
