class A
{
    public void print()
    {
        System.out.println("hello world");
    }
}

class B
{
    public static A pa;
    public static void show()
    {
        if (pa == null)
        {
            pa = new A();
            System.out.println("generate");
        }
        pa.print();
    }
}

class J1
{
    public static void main(String[] args)
    {
        //B b=new B();
        B.show();

        B.show();
        B.show();
    }
};
