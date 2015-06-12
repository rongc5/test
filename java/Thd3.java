class A implements Runnable
{
    public void run() 
    {
        Thread.currentThread().setName("A thread");
        while (true){
            System.out.println("hello world");
            System.out.println(Thread.currentThread().getName());

            try {
                Thread.sleep(1000);
            }
            catch (Exception e)
            {
                e.printStackTrace(); 
            }
        }

    }
}


public class Thd3
{
    public static void main(String [] args)
    {
        
        new Thread(new A()).start();
        Thread.currentThread().setName("main thread");
        System.out.println(Thread.currentThread().getName());
    }
}
