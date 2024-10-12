package reflection;

import java.lang.reflect.Proxy;
import java.lang.reflect.Method;
import java.lang.reflect.InvocationHandler;

interface log {
    int log(int num);
}

interface log2 {
    void log2();
}

class Something implements log {
    public int log(int num) {
        System.out.println("Inside num : "+num);
        return num + 1;
    }
}


/**
 * This is a dynamic proxy created at a run time instead of compile time
 * To create proxy needed an existing object and then it will add an wrapper around the object
 * We do this in order to intercept every single method call
 */
class Invocation implements InvocationHandler {
    private final Object target;
    Invocation(Object target) {
        this.target = target;
    }

    // In this we can add additional functionality before or after invocation of a method
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        System.out.println("Inside invoke bf");
        System.out.println("Proxy : "+proxy.getClass().getName());
        Object res = method.invoke(target, args);
        System.out.println("Inside invoke af");
        return res;
    }
    
}

public class InvocationHandler_2 {
    @SuppressWarnings("unchecked")
    /**
     * Create a proxy object
     * @param <T>
     * @param targetClass object for which the proxy is required
     * @param classInterface interface to receive as a output
     * @return
     */
    public static <T> T createProxy(T targetClass, Class<T> classInterface) {
        return (T) Proxy.newProxyInstance(classInterface.getClassLoader(), new Class<?>[] {classInterface}, new Invocation(targetClass));
    }

    public static void main(String[] args) {
        Something s = new Something();
        log l = createProxy(s, log.class);
        int i = l.log(1);
        /**
            Inside invoke bf
            Proxy : reflection.$Proxy0
            Inside num : 1
            Inside invoke af    
         */
        System.out.println("Returns : "+i);
        // Returns : 2
        // Something s2 =  createProxy(s, Something.class); // Error: Exception in thread "main" java.lang.IllegalArgumentException: reflection.Something is not an interface

        // createProxy(s, log2.class); // not possible

        log2 l2 = createProxy(() -> {
            System.out.println("inside Method");
        }, log2.class);
        l2.log2();
        /**
         * Op
            Inside invoke bf
            Proxy : reflection.$Proxy1
            inside Method
            Inside invoke af
        */
    }
}
