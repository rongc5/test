/*从1输到100*/
//方法之间调用的应用.
class ebc
{
	private int i=0;
	void a()
	{
		System.out.print(" "+i);
		if(i<100)
		b();
	}

	void b()
	{
		i++;
		a();
	}

public static void main(String[]ar)
	{
		new ebc().b();
    }
}
