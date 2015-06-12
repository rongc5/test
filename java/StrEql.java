public class StrEql
{
    public static void main(String [] args)
    {
        String a = new String("hello world");

        String b = new String("hello world");

        System.out.println(a.equals(b));

        if (a == b)
            System.out.println("true");
        else
            System.out.println("false");

    }
}
