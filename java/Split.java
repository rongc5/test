public class Split
{
    public static void main(String [] args)
    {
        //String str = "hello=world";
        String str = "";

        String a[] = str.split("=");

        System.out.println(a[1]+"=========>"+a[0]);
    }
}
