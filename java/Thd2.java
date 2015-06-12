class A implements Runnable
{
    public void run()
    {
        //while (true)
            System.out.println("hello world");
        Thread.currentThread().setName("A thread");
            System.out.println(Thread.currentThread().getName());

    }
}


public class Thd2
{
    public static void main(String [] args)
    {
        
        new Thread(new A()).start();
        Thread.currentThread().setName("main thread");
        System.out.println(Thread.currentThread().getName());
    }
}
