public class ArgsDemo
{
    public static void main(String [] args)
    {
        System.out.println("参数个数:"+args.length);

        for (int i = 0; i< args.length; i++)
            System.out.println("di参数:"+args[i]);
    }
}
