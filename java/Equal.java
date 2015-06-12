
class A
{
    private int i = 99;

    public A()
    {
    }

    public A(int i)
    {
        this.i = i;
    }

    public boolean equals(A a)
    {
        if (this.i == a.i)
            return true;
        else
            return false;
    }
}

public class Equal
{
    public static void main(String [] args)
    {
        A a = new A(100);
        A b = new A(100);

        boolean bool = a.equals(b);

        System.out.printf("%b\n", bool);
    }
}
