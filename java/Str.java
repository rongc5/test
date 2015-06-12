class A
{
    public String toString()
    {
        return "[" + "hello world" + "]";
    }
}


public class Str
{
    public static void main(String [] args)
    {
        String str = "hello world";

        System.out.printf("%s\n", str);
        A a = new A();
        System.out.printf("%s\n", a);
        System.out.println(a);
    }
}
