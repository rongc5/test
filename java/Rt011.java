//构造方法之间的调用
class Gwo
{
	int i=0;
	String s="unkown";
	char c='n';
    public Gwo(int i)
    {
        this("姚方浪");
        System.out.println("AaAaAa");
    }

    public Gwo(String s)
    {
        this('好');//在构造方法里调用其他构造方法的this语句必须是所属构造方法的第一句.
        System.out.println("BaBaBa");
    }

    public Gwo(char h)
    {	
        System.out.println("CaCaCa");
    }
};

class Rt011
{
	public static void main(String[] args)
	{
		new Gwo(1986);
	}
};
