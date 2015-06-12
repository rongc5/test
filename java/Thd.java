class A extends Thread
{
    public void run()
    {
        while (true)
        {
            System.out.println("hello world");
        }
    }
}

public class Thd
{
    public static void main(String [] str)
    {
        A aa =new A();
        aa.start();

        while (true)
        {
            System.out.println("zm");
        }

    }
}
