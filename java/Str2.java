public class Str2
{
    public static void main(String [] args)
    {
        int i = 99;
        
        StringBuffer str = new StringBuffer();
        str.append("hello world");

        System.out.println(str.toString());
    
        str.insert(0, "{");
        str.append("}");

        System.out.println(str.toString());
    }
}
