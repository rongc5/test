class A extends Thread
{
    public A(String name){
    
    super(name);
    
    
    }
    public void run()
    {
        while (true)
        {
            System.out.println("hello world");

        }
    }
}

public class Thd1
{
    public static void main(String [] str) throws Exception
    {
        Thread tid = null;

        //while (true)
        //{
            if (Thread.currentThread().getName().equals("upload"))
            {
                System.out.println("isAlive=====>!");
            }else {
                tid = new A("upload");
                tid.start();
            }


            //Thread.sleep(1000);
        //}
    }
}
