class Test extends Exception
//class Test extends Throwable
{

}


public class TryCatch
{
    public static void main(String [] args) throws Test
    {
        try 
        {
            throw new Test();
        }
        catch (Exception e)
        {
            System.out.printf("err:\n");

            e.printStackTrace();
        }
        finally
        {
            System.out.printf("hello world\n");
        }
    }
}
