
class Test
{
	//private native static char* get_Q_str(char *src);
	private native static int get_Q_num(char *src);
//	private native static char* get_Q_value(char *src);
//	private native static char* get_ROE_value(char *src);
	private native static int judge_XT_num(char *src);
//	private native static char* get_TOTAL_value(char *src);
//	private native static char* trim(char *str);
//	private native static char* get_last2str(char *src);
//	private native static char* del_last2str(char *src);
//	private native static char* string_analyze(char *buf);
//	private native static char* get_json(char *url);

	public static void main(String[] args)
	{
		new Test().get_json("can/hkg/ca/");
	}

	static
	{
		System.loadlibrary("Test");
	}
}


