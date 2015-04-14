class J
{
    int j_1=100;
    void tet()
    {
        K k=new K();
        k.sit();
    }
    class K//用Static修饰,就相当是一个外部类
    {
        void sit()
        {
            System.out.println("j_1="+j_1);
        }
    };
    public static void main(String[] args)
    {
        J j=new J();
        j.tet();
    }
};
