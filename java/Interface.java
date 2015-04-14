interface It
{
    public static final int a = 10086;
    public void display();
}

class A implements It
{
    public void display()
    {
        System.out.printf("help:%d\n", a);
    }
}

public class Interface
{
    public static void main(String [] args)
    {
        A a = new A();

        a.display();
    }
}
