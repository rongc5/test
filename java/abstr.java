abstract class A
{
    abstract public void f();
}

class B extends A
{
    public void f()
    {
        System.out.println("hello world");
    }
}

public class abstr
{
    public static void main(String[] args)
    {
        A a = new B();

        a.f();
    }
}
