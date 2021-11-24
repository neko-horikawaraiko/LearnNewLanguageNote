//////////////////////////////////////////////////////////////
//
//	学习《C++ Primer》笔记，函数与标准库、容器部分
//	edit by 猫耳堀川雷鼓/neko-horikawaraiko
//
//////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <list>
#include <forward_list>
#include <stack>
#include <queue>
#include <numeric>
#include <functional>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>
#include <tuple>
#include <bitset>
#include <regex>
#include <random>
#include <iomanip>
#pragma warning(disable:4996)

//指针、引用
void fun1()
{
	int* p1, p2;			//p1指针，p2int
	int* p3 = &p2, * p4;    //p1、p2都是指针
	int** p5 = &p3;         //p3指针的指针
	int*& r = p3;           //r指针的引用
}

//const、常量表达式
extern const int bufSize = 512;			//多文件共享const对象
void fun2()
{
	int i = 42;
	const int& r1 = i;                  //不可用r1改变i值，底层const
	const int& r2 = r1 * 2;

	//顶层const，本身是const；底层const，指向的对象是const
	const int& ri = 42;                 //const引用可直接绑定字面值，底层const

	double pi = 3.14;
	const double cd = 9.8;				//顶层const，cd是常量表达式
	const double* cptr = &pi;           //不可用cptr改变pi值，底层const
	double* const ptrc = &pi;           //不可改变ptrc，顶层const
	const double* const p = cptr;
}

//别名、auto、decltype
void fun3()
{
	typedef double wages;		//等同 using wages = double;

	auto i = 0, * p = &i;			//√
	//auto sz = 0, pi = 3.14;		//×

	const int ci = i, & cr = ci;
	auto b = ci;					//int b
	auto c = cr;					//int c
	auto d = &i;					//int *d
	auto e = &ci;					//const int *e
	const auto f = ci;				//cosnt int f

	decltype(ci) x = 0;				//const int x
	int i1 = 42, * p1 = &i1, & r1 = i1;
	decltype(r1 + 0) b1;			//int b1
	decltype(*p1) c1 = i;			//int &c1
	decltype((i1)) d1 = i;				//int &d1
	decltype(r1) e1 = i;				//int &e1
}

//string、cctype、范围for、C风格字符串
void fun4()
{
	using std::string;
	char cp[] = { 'a','b','c' };//不是以空字符结束的数组
	const char* cp2 = "Hidden";	//C风格以空字符结束的数组/字符串(此处简称C-string)，等同const char cp2[] = "Hidden"

	string s1;
	string s2(s1);				//等同 string s2 = s1;
	string s3("value");			//等同 string s3 = "value";
	string s4(4, 'c');
	string s5(cp, 2);			//从cp数组拷贝前2个元素生成string
	string s6(s3, 2);			//从string s3下标2处到末尾拷贝生成string
	string s7(s3, 2, 3);		//从string s3下标2处到末尾拷贝3字符生成string，最多拷贝到s3末尾
	string s8(cp2);				//√
//	string s8(cp);				//×
	s1.empty();
	s1.size();
	s1.substr(/*pos, n*/);		//返回从pos开始n个字符的string拷贝

	//添加删除：args参数 pos位置下标 iter位置迭代器 len长度 b、e头尾迭代器
	//args							s.append(args)	s.assign(args)	s.insert(pos, args)	s.insert(iter, args)	s.replace(pos, len, args)	s.replace(b, e, args)
	//str（字符串且不等于s）					√				√				√					×						√							√
	//str, pos, len（str从pos开始最多len个字符）√				√				√					×						√							×
	//cp, len（cp指向数组的前最多len个字符）		√				√				√					×						√							√
	//cp（以空字符结束的字符数组）				√				√				×					×						√							√
	//n, c（n个c）							√				√				√					√						√							√
	//b, e（迭代器（不指向s）范围内字符）			√				√				×					√						×							√
	//il（字符列表）							√				√				×					√						×							√
	s1.append("a");				//末尾插入
	s3.replace(1, 2, "jkl");	//下标1处删除2字符并插入"jkl"，等同	s3.erase(1, 2);s3.insert(1, "jkl");

	//查找。实参必须为以下之一：	c, pos(=0)		从pos开始找char c
	//						s2, pos(=0)		从pos开始找string s2
	//						cp, pos(=0)		从pos开始查找以空字符结束的C-string cp
	//						cp, pos, n		从pos开始查找cp指向数组的前n个字符
	s3.find("ab");				//在s3中查找"ab"第一次出现位置
	s3.rfind("ab");				//在s3中查找"ab"最后一次出现位置
	s3.find_first_of("ab");		//在s3中查找"ab"任一字符第一次出现位置
	s3.find_last_of("ab");		//在s3中查找"ab"任一字符最后一次出现位置
	s3.find_first_not_of("ab");	//在s3中查找第一个不在"ab"中的字符
	s3.find_last_not_of("ab");	//在s3中查找最后一个不在"ab"中的字符

	s1 + s2;
	s1 == s2;
	s1 < s2;
	getline(std::cin, s1);
	string::size_type len;
	s1.compare(s2);
	std::to_string(1);			//任意数→字符串
	//size_t sz;
	std::stoi(s1/*, &sz, 10*/);	//s1转换为int，sz返回是s1中第一个非数值字符下标，转换为10进制
// 	std::stol(s1);
// 	std::stoul(s1);
// 	std::stoll(s1);
// 	std::stoull(s1);
// 	std::stof(s1);
// 	std::stod(s1);
// 	std::stold(s1);

	//<cctype>
	char c = 'a';
	c = isalnum(c);				//字母数字
	c = isalpha(c);				//字母
	c = iscntrl(c);				//控制字符
	c = isdigit(c);				//数字
	c = isgraph(c);				//非空格且可打印
	c = islower(c);				//小写
	c = isprint(c);				//可打印
	c = ispunct(c);				//标点
	c = isspace(c);				//空白字符（空格、制表符、回车、换行、进纸符）
	c = isupper(c);				//大写
	c = isxdigit(c);			//十六进制数
	c = tolower(c);
	c = toupper(c);

	for (auto c : s1)
		std::cout << c << std::endl;
	for (auto& c : s1)
		c = toupper(c);

	const char* ca1 = "A string";
	const char ca2[] = "A diff string";
	char ct = '-';
	strlen(ca1);				//长度
	strcmp(ca1, ca2);			//比较
	strcat(&ct, ca2);			//ca2加到ct后，返回ct
	strcpy(&ct, ca2);			//ca2拷贝到ct，返回ct
}

//数组（固定大小，不可拷贝，不可赋值）
void fun6()
{
	int arr[10] = { 1 };
	int* prts[10] = { &arr[0], &arr[1] };		//成一个数组的10个指针
	int(*Parray)[10] = &arr;					//Parray是指向arr数组的指针，等同arr
	int(&arrRef)[10] = arr;						//arrRef是arr别名，与arr同样用
	int* (&arry)[10] = prts;					//prts的别名

	auto p = std::begin(arr);					//首尾指针
	std::end(arr);
	*p++;										//先取值，后p++，返回*p，等同(*p)++
	*++p;										//先++p，后取值，返回*(++p)，等同*(++p)
	++* p;										//先取值，后值++，返回后来值，等同++(*p)
	(*p)++;										//先取值，后值++，返回先前值

	int ia[3][4];
	auto pia = ia;								// = int(*pia)[4]，指向第一行数组
	pia++;										//移到下一行
	using int_array = int[4];					//等同 typedef int int_array[4];
}

//强制转换
void fun7()
{
	int i = 42;
	const int ci = 42;
	static_cast<double>(i);
	static_cast<double>(ci);

	void* p = &i;
	int* pi = static_cast<int*>(p);

	//const_cast：去掉或加上const性质
	const char* pc = "";
	auto pp = const_cast<char*>(pc);			//char *p
	//auto p = static_cast<char*>(pc);			//×
	auto ps = static_cast<std::string>(pc);
	//const_cast<std::string>(pc);				//×
	std::string s = "a";
	std::string& s2 = s;
	const_cast<const std::string&>(s2);

	//reinterpret_cast：较低层次重新解释（尽量不使用）
	int* ip = &i;
	char* cp = reinterpret_cast<char*>(ip);

	//dynamic_cast见后面
}

//返回数组指针或引用
using arrT = int[10];
arrT* arrFun(int i);				//等同 int(*arrFun(int i))[10];   auto arrFun2(int i) -> int(*)[10];

//内联函数：调用处直接展开；constexpr函数：返回值形参都是字面值类型，有且只有一条return语句 （这两种函数通常放在头文件）；常量表达式、constexpr
//字面值类型：算数类型，引用、指针（由于编译时必须可确定值，所以一般为nullptr或0或内存中固定位置）；IO库、string类等不是字面值类型
inline const std::string& shorterStr(const std::string& s1, const std::string& s2) { return s1.size() <= s2.size() ? s1 : s2; }
constexpr int mf = 20;										//constexpr变量mf是一个常量，必须用常量表达式初始化，20是常量表达式
constexpr size_t new_sz(size_t sz) { return 2 * sz; }		//所以可以声明 int a[new_sz(2)];

//assert  <cassert>，预处理宏
void fun8()
{
	std::string s = "asd";
	assert(s.size() > 1);			//不满足时报错
	if (s.size() < 10)
	{
		std::cerr << "Error: " << __FILE__
			<< " : in funcion " << __func__		//__FUNCTION__
			<< " at line " << __LINE__ << std::endl
			<< "\tCompiled on " << __DATE__
			<< " at " << __TIME__ << std::endl;
	}
}

//函数指针
typedef bool aFunBool(int, int);							//等同 typedef decltype(funBool) Func;   using Func = bool(int, int);  typedef bool (*Func)(int, int);  typedef decltype(funBool) *Func;
using bFunBool = bool(int, int);
using bpFunBool = bool(*)(int, int);
bool				funBool(int a, int b);
void				useFunBool(bool(int, int));				//函数指针形参，等同 void useFunBool(bool (*)(int, int)); void useFunBool(aFunBool); ，实际上将函数类型转换为函数指针
bpFunBool			retFunBool1(int);						//√，可以返回函数指针
//bFunBool			retFunBool2(int);						//×，不能返回函数
bFunBool*			retFunBool3(int);						//√
bool				(*retFunBool4(int))(int, int);			//√
auto				retFunBool5(int) -> bool(*)(int, int);	//√
decltype(funBool)*	retFunBool6(int);						//√
void fun9()
{
	bool (*pFunbool)(int a, int b);							//pFunBool是函数指针
	pFunbool = funBool;										//等同 pFunBool = &funBool;
	bool b1 = pFunbool(2, 3);								//等同 bool b1 = (*pFunbool)(2, 3);
}

//class示例，friend，const，静态成员，可变成员，显式转换
class Sales_data
{
	friend class friendSales_data;							//friendSales_data可以访问Sales_data的所有成员
	friend void friendSales_data::fun();					//friendSales_data::fun()可以访问Sales_data的所有成员
	friend Sales_data add(const Sales_data&, const Sales_data&);
	friend std::istream& read(std::istream&, Sales_data&);
	friend std::ostream& print(std::ostream&, const Sales_data&);
public:
	Sales_data() = default;									//合成默认构造函数，未定义其他构造函数时编译器会默认生成一个
	Sales_data(const std::string& s, unsigned n, double p) : bookNo(s), units_sold(n), revenue(p * n) {}//初始值顺序与成员声明顺序一致
	Sales_data(const std::string& s) :Sales_data(s, 0, 0) {}//委托构造函数
	explicit Sales_data(std::istream& is);					//要求显式转换，explicit只对一个实参的构造函数有效

	std::string isbn() { return bookNo; }					//隐式内联，可供非常量对象使用
	const std::string isbn() const { return bookNo; }		//const重载，const表示不改变对象本身，可供常量/非常量对象使用

	Sales_data& combine(const Sales_data&) {}
	void some_mem() const;

	static double staticFun();										//静态成员函数，不能声明为const的，不能再函数体内用this指针
	void useStaticData() { double d = staticdata; /*...*/ }			//成员函数可以不通过作用域运算符直接使用静态成员
	void useStaticChar(char = c) {}									//可以使用静态成员作为默认实参
private:
	double avg_price() const { return units_sold ? revenue / units_sold : 0; }
	std::string bookNo;
	unsigned units_sold = 0;										//类内初始值
	double revenue = 0.0;											//类内初始值
	mutable size_t ctr = 0;											//可变成员
	Sales_data* nextbook;											//可以定义指向自身类型的不完全类型的指针
	static Sales_data sd;											//可以定义不完全类型的静态成员
	static double staticdata;										//静态成员
	static constexpr int anoStatic = 30;							//通常情况下不能类内初始化静态成员，但可以提供const int类内初始值，且为constexpr的
	static const char c;
};
Sales_data::Sales_data(std::istream& is) { /*...*/ }				//此处不应再使用explicit关键字，explicit只在类内标明
std::istream&	read(std::istream&, Sales_data&) { /*...*/ }
inline void		Sales_data::some_mem() const { ++ctr; }				//显式声明inline，const函数可改变可变成员
double			Sales_data::staticdata = 0.0;						//类外部定义静态成员
double			Sales_data::staticFun() { return staticdata; }		//类外部定义时不能重复static关键字
void fun10()
{
	double r = Sales_data::staticFun();
	Sales_data salesdata;											//等同 Sales_data* salesdata;
	r = salesdata.staticFun();										//	  r = salesdata->staticFun();
}
class friendSales_data
{
	void fun() { Sales_data s; s.units_sold = 1;/*...*/ }
};

//聚合类：所有成员public，未定义任何构造函数，无类内初始值，无基类，无虚函数
struct AggregateClass
{
	int ival;
	std::string s;
};
AggregateClass val1 = { 0,"Anna" };						//√
//AggregateClass val2 = { "Anna",0 };					//×，必须按顺序初始化

//字面值常量类
struct ConstClass								//数据成员都是字面值类型的聚合类
{
	int ival;
};
class ConstClass2								//非聚合类但：①数据成员为字面值类型；②至少一个constexpr构造函数；
{												//③数据成员有内类初始值则初始值为常量表达式，成员为某类类型则使用成员自己的constexpr构造函数；④必须使用默认析构函数销毁类对象
public:
	constexpr ConstClass2(bool h) :hw(h) {}		//constexpr构造函数体为空
	void set_hw(bool b) { hw = b; }
private:
	bool hw;
};

//IO类、输入输出格式化、未格式化IO、流随机访问、文件IO、string IO
void fun11()
{
	//以流cin和IO类istream为例
	using std::cin;
	using std::istream;
	//条件状态位
	istream::iostate;								//整个io状态
	istream::badbit;								//流崩溃位
	istream::failbit;								//io操作失败位
	istream::eofbit;								//文件结束位
	istream::goodbit;								//流未处于错误状态位，固定为0

	cin.eof();										//eofbit置位则返回true
	cin.fail();										//failbit或badbit置位则返回true
	cin.bad();										//badbit置位则返回true
	cin.good();										//流有效则返回true
	cin.setstate(cin.badbit);						//将某（些）位置位
	cin.clear(cin.badbit);							//将某（些）位复位
	cin.clear();									//条件状态位复位且将流置为有效
	cin.rdstate();									//返回流状态iostate
	//cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);	//failbit和badbit复位，eofbit不变

	//输入输出格式化
	using std::cout;
	char c;
	//带Δ的组表示只改变一次流状态，不带表示永久改变
	// boolalpha/noboolalpha	将bool值打印为true和false/将bool值打印还原为1和0
	// oct/hex/dec				将整型值显示为8/16/还原10进制
	// showbase/noshowbase		打印整型值时显示/还原不显示进制（前导0x表示16进制，0表示8进制，无前导则10进制）
	// uppercase/nouppercase	打印16进制数时使用大写/还原小写其中的所有字母（包含0X)，打印科学计数法时打印E/e
	// showpoint/noshowpoint	浮点值不含小数时显示/还原不显示小数点
	// showpos/noshowpos		对非负数显示/还原不显示"+"号
	// unitbuf/nounitbuf		每次输出后缓冲区立即刷新/还原原刷新方式
	// fixed/scientific/hexfloat/defaultfloat	使用定点十进制/科学记数法/定点十六进制与科学记数法/还原智能选择十进制浮点或科学记数法打印浮点值（注：使用这些操纵符后，精度值改为控制小数点后的位数而非小数点前后总位数）
	// left/right/internal		输出时左对齐/还原右对齐/符号左对齐而数字右对齐，中间用空格填充
	//Δsetw						<iomanip>打印下一个数字或字符串值时的最小空间
	// setprecision				<iomanip>设定精度
	// setbase					<iomanip>设定输出进制
	// setfill					<iomanip>设定填充空白使用的字符
	// skipws/noskipws			输入运算符还原跳过/不跳过空白符（空格、制表、换行、换页、回车）
	//Δflush/ends/endl			直接刷新缓冲区/添加空字符，刷新缓冲区/添加换行，刷新缓冲区
	cout << std::boolalpha << true << " " << false << std::noboolalpha << std::endl;
	cout << 20 << " " << std::oct << 20 << " " << std::hex << 20 << " " << std::dec << 20 << std::endl;
	cout << std::showbase << 20 << " " << std::oct << 20 << " " << std::hex << 20 << " " << std::dec << 20 << std::noshowbase << std::endl;
	cout << std::uppercase << std::showbase << std::hex << 30 << std::nouppercase << std::noshowbase << std::dec << std::endl;
	std::streamsize s = cout.precision();			//无参数时返回当前精度（默认浮点值6位精度，即最多打印6个数字，超过则用科学记数法）
	cout.precision(12);								//含参数时设定精度（此处设为12位数字，含整数部分与小数部分），返回旧精度
	cout << std::setprecision(12);					//等同cout.precision(12);
	cout << sqrt(2.0) << std::setprecision(s) << std::endl;
	cout << std::showpoint << 30.0 << std::noshowpoint << std::endl;
	cout << std::showpos << 30 << std::noshowpos << std::endl;
	cout << std::unitbuf << std::nounitbuf;
	cout << std::fixed << 100 * sqrt(2.0) << std::scientific << 100 * sqrt(2.0) << std::hexfloat << 100 * sqrt(2.0) << std::defaultfloat << std::endl;
	cout << std::left << -1.2 << std::internal << -1.2 << std::right << std::endl;
	cout << std::setw(12) << std::internal << std::setfill('#') << -1.2 << std::setfill(' ') << std::right << std::endl;
	cin >> std::noskipws;
	while (cin >> c) cout << c;
	cin >> std::skipws;

	int ch;
	char buf[128];
	//单字节底层IO操作（未格式化IO操作）
	cin.get(c);					//读取下一个字节放入字符c中，返回istream对象
	cin.get();					//读取下一个字节作为int返回（便于处理文件尾标记EOF）
	cin.putback(c);				//输入流后移一位，将最后读取的某类型对象（此处为一个char，读取的类型必须与参数类型匹配）返回输入流中，返回istream对象（相当于unget特别版本）
	cin.unget();				//输入流后移一位，将最后读取的值返回输入流中，返回istream对象
	cin.peek();					//读取下一个字节作为int返回，但不从流中删除它（相当于复制了一份再转为int）
	cout.put(c);				//原样输出c中的字节
	(ch = cin.get()) != EOF;	//常用此检测文件尾

	//多字节底层IO操作（未格式化IO操作）
	cin.get(buf, 60, '#');		//从cin读取最多60字节保存在字符数组buf中，读取过程遇到字符'#'或文件尾或已经读取59字节后（自动加'\0'）停止，返回cin。遇到'#'时不从输入流取出而留在输入流
	cin.get(buf, 60);			//从cin读取最多60字节保存在字符数组buf中，读取过程遇到默认字符'\n'或文件尾或已经读取59字节后停止，返回cin。遇到'\n'时不从输入流取出而留在输入流
	cin.getline(buf, 60, '#');	//从cin读取最多60字节保存在字符数组buf中，读取过程遇到字符'#'或文件尾或已经读取59字节后停止，返回cin。遇到'#'时从流中取出并丢弃
	cin.read(buf, 60);			//从cin读取最多60字节保存在字符数组buf中，返回cin
	cin.gcount();				//返回上一个未格式化读取操作从cin读取的字节数（特：若之前调用过peek、unget、putback则会返回0）
	cin.ignore(/*1, EOF*/);		//读取并忽略字符/*最多1个，包括EOF*/
	cout.write(buf, 30);		//将buf中30字节写入cout，并返回cout

	//流随机访问，只适用fstream和sstream，IO类此时维护一个标记记录下一个读写操作的位置（读写共享一个标记，所以输入输出切换时需要用seek重定位标记），空白符同样占据一个位置
	std::istringstream iss;
	std::ostringstream oss;
	iss.tellg();									//返回输入流中标记的当前位置
	std::ostringstream::pos_type mark = oss.tellp();//返回输出流中标记的当前位置
	iss.seekg(2);									//在输入流中将标记重定位到给定位置，常使用前一个tellg或tellp返回值
	oss.seekp(3);									//在输出流中将标记重定位到给定位置，常使用前一个tellg或tellp返回值
	iss.seekg(4, std::stringstream::beg);			//将标记重定位到开头往后4的位置
	iss.seekg(-4, std::stringstream::end);			//将标记重定位到尾部往前4的位置
	oss.seekp(4, std::stringstream::cur);			//将标记重定位到当前往后4的位置

	//关联输入输出流，输入操作会先刷新输出流
	std::ostream* old_tie = cin.tie(nullptr);		//断开cin与输出流（默认cout）关联
	cin.tie(&std::cerr);							//cin与cerr关联，cin读取会先刷新cerr
	cin.tie(old_tie);								//cin与cout关联

	//<fstream>
	using std::ifstream;
	using std::ofstream;
	std::string ifile = "";
	ifstream inf(ifile/*, ifstream::in*/);			//输入流inf关联到ifile
	ofstream outf;									//输出流outf未关联文件
	outf.open(ifile/*, ofstream::out*/);			//outf关联到ifile，返回void
	outf.is_open();									//是否成功打开且尚未关闭，返回bool
	outf.close();									//关闭绑定的文件
	//fstream销毁时自动调用close()
	//文件模式in读/out写/app写到文件尾/trunc截断/ate打开定位到文件尾/binary二进制
	//out = out | trunc;截断  app = out | app;追加到末尾  in | out;不截断，同时输入输出  ate和binary可与其他任意组合  inf默认in  outf默认out  fstream默认in | out

	//<sstream>
	using std::stringstream;
	std::string s;
	stringstream strm;
	stringstream strm2(s);							//strm2保存s的拷贝
	strm2.str();									//返回strm2保存的string的拷贝
	strm.str("s");									//字符串拷贝到strm中，返回void
}

//顺序容器、容器适配器
void fun12()
{
	//                  |  <vector>  |  <deque>	  |  <list>  |  <forward_list>  |  <array>   |  <string>
	//                  |  vector    |  deque	  |  list    |  forward_list    |  array     |  string(相当于字符vector)
	//大小              |    可变    |    可变    |   可变   |       可变       |    固定    |   可变
	//访问              |    随机    |    随机    |   顺序   |       顺序       |    随机    |   随机
	//访问方向          |            |            |   双向   |       单向       |            |
	//快速添删元素位置  |    尾部    |    头尾    |   任何   |       任何       |  不能填删	 |   尾部
	//
	//选择原则：(1)默认vector；(2)元素小且空间额外开销重要，则不用list、forward_list；(3)随机访问则vector或deque；(4)中间插入删除则list或forward_list；
	//(5)头尾插入删除则deque；(6)输入时中间插入，随后随机访问：是否可用vector添加然后sort排序；否则可用list插入，再拷贝到vector

	//以vector和list为例
	using std::vector;
	vector<int>::iterator p;					//迭代器
	vector<int>::const_iterator;				//常量迭代器
	vector<int>::size_type;						//大小
	vector<int>::difference_type;				//迭代器距离
	vector<int>::value_type;					//元素类型
	vector<int>::reference;						//元素左值
	vector<int>::const_reference;				//元素常量左值

	vector<int> vi1;							//空容器
	vector<int> vi2(vi1);						//拷贝，等同 vector<int> vi2 = vi1;  ，新旧容器类型、元素类型必须相同
	vector<int> vi3(vi1.begin(), vi1.end());	//拷贝迭代器范围（不支持array），但是新旧容器类型可不同，元素类型可不同（但需要可类型转换）
	vector<int> vi4{ 1,2,3 };					//列表初始化，等同 vector<int> vi4 = { 1,2,3 };
	vector<int> vi5(3/*, 5*/);					//3个元素的容器，默认int初始值（0）/*，全部初始化为5*/
	vector<int> v6(10);							//10个0
	vector<int> v7{ 10 };						//1个10
	vector<std::string> v8{ 10,"hi" };			//10个hi，等同 vector<std::string> v8(10, "hi");
	std::list<int> li;
	vi1.cbegin();								//vi1.rbegin();  vi1.crbegin();

	//赋值
	vi1 = vi2;									//vi1替换为vi2的拷贝
	vi1 = { 1,2,3 };							//vi1列表赋值（不适用array）
	vi1.assign(vi4.begin(), vi4.end());			//替换为另一序列两迭代器之间元素
	vi1.assign({ 1,2 });						//替换为列表
	vi1.assign(3, 2);							//替换为3个2
	vi1.swap(vi2);								//等同 swap(vi1, vi2);

	//大小，resize缩小时，指向被删掉的元素的迭代器/引用/指针失效，resize若导致重分配存储空间则所有迭代器/引用/指针失效
	vi1.size();									//保存的元素数（不支持forward_list)
	vi1.max_size();								//可保存的最大元素数
	vi1.resize(10/*, 3*/);						//改变大小为10/*并所有赋值3*/
	vi1.empty();								//容器是否为空

	//添加删除（全部不适用array，forward_list见下面）
	vi1.insert(p, 2);							//p之前拷贝添加元素，返回指向添加的第一个元素的迭代器
// 	vi1.insert(p, 2, 3);						//拷贝添加2个3
// 	vi1.insert(p, vi4.begin(), vi4.end());		//拷贝添加迭代器之间元素
// 	vi1.insert(p, { 1,2 });						//拷贝添加一个列表
	vi1.emplace(p, 3);							//构造添加一个3
	li.push_back(1);							//在尾部拷贝添加元素（不适用forward_list）
	li.push_front(2);							//在头部拷贝添加元素（不适用vector、string）
	li.emplace_back(3);							//在尾部构造添加元素（不适用forward_list）
	li.emplace_front(4);						//在头部构造添加元素（不适用vector、string）
	vi1.erase(p);								//删除p指定元素，返回被删元素的后一元素的迭代器
//	vi1.erase(vi1.begin(), vi1.end());			//删除迭代器之间元素，返回最后一个被删元素的后一元素的迭代器
	li.pop_front();								//删除首元素，返回void（不适用vector、string）
	li.pop_back();								//删除尾元素，返回void（不适用forward_list）
	vi1.clear();								//删除所有元素，返回void
	//vector、string添加若导致重分配存储空间：使所有指向容器迭代器/引用/指针失效，未重分配：插入位置后的迭代器/引用/指针失效，插入位置前的迭代器/引用/指针有效
	//vector、string删除使指向容器删除点后迭代器/引用/指针失效，deque删除首尾外任何元素使所有指向容器迭代器/引用/指针失效
	//deque添加到首尾外任何位置：所有指向容器迭代器/引用/指针失效，添加到首尾：迭代器失效，指向已存在元素的引用/指针有效
	//deque删除首尾外任何位置：所有指向容器迭代器/引用/指针失效，删除尾：尾后迭代器失效，其他迭代器/引用/指针有效，删除首：删除尾：所有迭代器/引用/指针有效

	//访问
	vi1.front();								//返回头元素引用
	vi1.back();									//返回尾元素引用（不适用forward_list）
	vi1[3];										//返回下标为3元素引用，不检查下标越界（不适用list、forward_list）
	vi1.at(3);									//返回下标为3元素引用，可检查下标越界（不适用list、forward_list）

	//容量（vector、string）
	vi1.shrink_to_fit();						//将capacity()减小为size()大小，但不保证一定可退回内存空间，也可对deque使用
	vi1.capacity();								//当前可保存多少元素
	vi1.reserve(10);							//分配至少容纳10元素的空间，若小于当前使用空间则什么也不做

	//比较
	vi1 == vi2;									//同容器类型同元素类型
	vi1 < vi2;

	//迭代器使用
	auto b = v8.begin(), e = v8.end();
	*b;											//返回元素的引用
	b->c_str();									//等同 (*b).c_str()
	++b;
	b + 2;										//向前移动2位置
	b += 2;
	auto a1 = b - e;							//iterator::difference_type
	b == e;
	b < e;

	//<array>
	std::array<int, 3> ai1;						//默认初始化
	std::array<int, 4> ai2 = { 1,2,3 };			//列表初始化

	//<forward_list>
	std::forward_list<int> fli;
	std::forward_list<int>::iterator pfli;
	fli.before_begin();							//fli.cbefore_begin();	首前迭代器，不可解引用
	fli.insert_after(pfli, 2);					//返回添加的最后一个元素的迭代器
// 	fli.insert_after(pfli, 2, 3);
// 	fli.insert_after(pfli, vi4.begin(), vi4.end());
// 	fli.insert_after(pfli, { 1,2 });
	fli.emplace_after(pfli, 3);					//返回添加的元素的迭代器
	fli.erase_after(pfli);						//返回被删元素的后一元素的迭代器
//	fli.erase_after(fli.begin(), fli.end());

	//list/forward_list
	std::list<int> li2;
	li.merge(li2/*, comp*/);					// /*使用给定的comp比较操作*/将li2合并入li，li和li2必须已经有序
	li.remove(1);								//删除所有的1
	li.remove_if([](int i) { return i == 1; });	//删除所有的使谓词为真（此处为等于1）的元素
	li.reverse();								//反转顺序
	li.sort(/*comp*/);							//使用</*或使用给定的comp比较操作*/将li排序
	li.unique(/*pred*/);						//使用==/*或给定的二元谓词pred*/删除同一个值的连续拷贝
	li.splice(li.end(), li2);											//将li2元素移到li中，且删除li2中元素
//	fli.splice_after(fli.before_begin(), fli2);							//将fli2元素移到li中，且删除fli2中元素，此处必须用fli的首前位置
//	li.splice(li.begin(), li2, li2.begin());							//移动指向的一个元素
//	fli.splice_after(fli.before_begin(), li2, li2.begin());				//移动指向之后的一个元素
//	li.splice(li.begin(), li2, fli2.begin(), fli2.end());				//移动范围元素
//	fli.splice_after(fli.before_begin(), li2, li2.begin(), li2.end());	//移动范围元素

	//容器适配器	都是基于某容器实现
	//<stack> stack默认基于deque，也可在list或vector实现 <queue> queue默认基于deque，也可在list或vector实现；priority_queue默认基于vector，也可在deque实现
	//<stack>
	std::stack<int> si, si2;					//等同 stack<int, deque<int>> si;
	std::stack<int, vector<int>> si3(vi1);		//重载默认容器类型，拷贝vi1（需在同一种容器上实现）
	si.empty();
	si.size();
	si.swap(si2);								//等同 std::swap(si, si2); ，注意不能与si3交换
	si.push(1);									//拷贝压入栈顶
	si.emplace(2);								//构造压入栈顶
	si.pop();									//删除栈顶元素但不返回值
	si.top();									//返回栈顶元素

	//<queue>
	std::queue<int> qi, qi2;
	qi.empty();
	qi.size();
	qi.swap(qi2);
	qi.push(1);
	qi.emplace(2);
	qi.front();									//返回queue首元素（只适用queue）
	qi.back();									//返回queue尾元素（只适用queue）
	qi.pop();									//删除queue首元素或priority_queue最高优先级元素，但不返回值
	std::priority_queue<int> pqi;
	pqi.top();									//返回priority_queue最高优先级元素（只适用priority_queue）
}

//泛型算法、谓词（一元谓词：接受一个参数，二元谓词：接受两个参数）、bind函数
bool			isShorter(const std::string& s1, const std::string& s2)		{ return s1.size() < s2.size(); }
bool			checkSize(const std::string& s, std::string::size_type sz)	{ return s.size() >= sz; }
std::ostream&	refPrint(std::ostream& os, const std::string& s, char c)	{ return os << s << c; }
void fun13()
{
	std::vector<int> vi(10), vi2(11), vi3;
	std::vector<std::string> vs(5);
	std::vector<int>::iterator vibe = vi.begin(), vien = vi.end();
	std::vector<std::string>::iterator vsbe = vs.begin(), vsen = vs.end();
	//placeholders：<functional>
	using namespace std::placeholders;
	//<numeric>
	int sz = 10;
	int isum = accumulate(vibe, vien, 0);								//需要定义operator+、序列元素类型可以匹配第三个参数类型
	std::string ssum = accumulate(vsbe, vsen, std::string(""));
	//<algorithm>
	equal(vibe, vien, vi2.begin());										//需要vi与vi2元素可用==比较，且vi2.size() >= vi.size()
	fill(vibe, vien, 1);												//区间所有元素重置为1
	fill_n(vibe, vi.size(), 2);											//vibe开始，size()个元素重置为2
	fill_n(back_inserter(vi3), 10, 3);									//<iterator>，后插入迭代器，此时可以给空容器加元素
	copy(vibe, vien, vi2.begin());										//前两个参数的范围的值，拷贝给第三个参数开始的目的序列，要求vi2.size() >= vi.size()，返回拷贝目的范围后一位置迭代器
	replace(vibe, vien, 1, 43);											//范围内的1全改为43
	replace_copy(vibe, vien, back_inserter(vi3), 1, 43);				//范围拷贝到目标位置且把其中的1全改为43
	sort(vsbe, vsen);													//默认从小到大排序
	sort(vsbe, vsen, isShorter);										//按照谓词（string长度从小到大）排序，等同sort(vsbe, vsen, [](const strin& a, const string& b) { return a.size() < b.size(); });
	sort(vsbe, vsen, bind(isShorter, _2, _1));							//bind函数将sort作用改为string长度从大到小排序
	unique(vsbe, vsen);													//重排范围，使每个相同元素只出现一次且排在范围前部，返回不重复区域的后一元素位置的迭代器
	find(vibe, vien, 1);												//返回第三参数第一次出现位置
	find_if(vsbe, vsen, [sz](const std::string& s)						//返回第一个满足谓词的元素的迭代器
		{ return s.size() > sz; });										//等同 ↓
	find_if(vsbe, vsen, bind(checkSize, _1, sz));						//bind函数，传入的参数依次拷贝传入_1,_2,...位置
	for_each(vibe, vien, [](int i) { std::cout << i << " "; });			//对范围内每个元素调用谓词
	for_each(vsbe, vsen, bind(refPrint, std::ref(std::cout), _1, ' '));	//ref/cref：<functional>，ref返回一个包含给定引用的对象，此对象可以提供bind的拷贝需要
}

//lambda表达式： [捕获列表](参数列表) —> 返回类型 { 函数体 }
void fun14()
{
	int sz = 10;
	std::ostream& os = std::cout;
	char c = ' ';
	auto lambda1 = [] { return 42; };									//参数列表和返回类型可省略
	auto lambda2 = [sz](const std::string& s) { return s.size() > sz; };//值捕获，捕获了局部变量才可在函数体中使用
	auto lambda3 = [&sz] { return sz; };								//引用捕获
	auto lambda4 = [=] { return sz; };									//隐式值捕获
	auto lambda5 = [&] { return sz; };									//隐式引用捕获
	auto lambda6 = [&, c](const std::string& s) { os << s << c; };		//隐式引用捕获其他，显式值捕获c
	auto lambda7 = [=, &os](const std::string& s) { os << s << c; };	//隐式值捕获其他，显式引用捕获os
	auto lambda8 = [sz]() mutable { return ++sz; };						//mutable可改变被值捕获变量的值，等同auto lambda8 = [&sz] { return ++sz; };
	auto lambda9 = [](int i) -> int { if (i < 0) return i; else return -i; };	//lambda函数包含任何return外语句时推断返回void，需指定尾置返回类型
}

//<iterator> 其他类型迭代器
void fun15()
{
	//插入迭代器，接受容器参数
	std::list<int> listi;
	back_inserter(listi);								//创建使用push_back的迭代器，始终插入到容器新的最后一个元素之后
	front_inserter(listi);								//创建使用push_front的迭代器，始终插入到容器新的第一个元素之前
	inserter(listi, listi.begin());						//创建使用insert的迭代器，插入到给定迭代器所表示元素之前

	//流迭代器
	using std::istream_iterator;
	istream_iterator<int> int_eof;						//指向尾后的输入流迭代器
	istream_iterator<int> int_it(std::cin);				//从cin读取的输入流迭代器，输入迭代器使用>>读取流，所以被读取类型需定义>>
	int_it != int_eof;									//遇到文件尾或不是int的数据为止
	std::vector<int> vi(int_it, int_eof);				//示例，从cin读取并构造vector
	accumulate(int_it, int_eof, 0);						//示例，读取求和
	int sum = 0;
	sum += *int_it++;									//示例，读取求和（先读取cin并解引用得到数据值，加到sum上，移到下一位置（等待cin））

	using std::ostream_iterator;
	ostream_iterator<int> out_it(std::cout);			//写到cout的输出流迭代器
	ostream_iterator<int> out_it2(std::cout, " ");		//写到cout的输出流迭代器，且每个值后面都输出" "空字符结尾的字符数组
	for (auto e : vi)									//示例，将每个元素输出至cout
		*out_it2++ = e;									//等同 out_it2 = e;
	copy(vi.begin(), vi.end(), out_it2);				//示例，将每个元素输出至cout，等同 ↑

	//反向迭代器
	std::reverse_iterator<int> i_iter = vi.crbegin();
	++i_iter;											//实际上往前移动，所以不支持forward_list和流迭代器上创建反向迭代器
	i_iter.base();										//转换为普通迭代器，实际上指向原来序列的正向下一个元素（此处在上一句代码基础上，得到的普通迭代器就是指向最后一元素）

	//移动迭代器
}

//关联容器
size_t hasher(const Sales_data& sd) { return std::hash<std::string>()(sd.isbn()); }
bool eqOp(const Sales_data& lhs, const Sales_data& rhs) { return lhs.isbn() == rhs.isbn(); }
bool cmprShorterStr(const std::string& s1, const std::string& s2) { return s1.size() < s2.size(); }
void fun16()
{
	//map/multimap <map>，set/multiset <set> 关键字有序且不可重复/可重复的map/set
	//基础用法：构造、赋值、大小、添加删除、比较、迭代器
	std::map<std::string, size_t> map1;
	std::map<std::string, size_t> map2(map1);
	std::map<std::string, size_t> map3(map1.begin(),map1.end());
	std::map<std::string, size_t> map4{ { "A",3 }, { "B",2 } };
	std::map<std::string, size_t, decltype(cmprShorterStr)*> map5(cmprShorterStr);	//使用尖括号内第三参数来对关键字比较，必须是函数指针或 重载调用运算符（需要const后缀）进行比较的类的类名（仿函数），变量的实参可用圆括号或花括号提供实际比较的函数（仿函数不需要）
	map1 = map2;
	map1 = { { "C",1 } };
	map1.swap(map2);											//等同 swap(map1, map2);
	map1.size();
	map1.max_size();
	map1.empty();
	map1.insert({ "D",1 });										//等同 map1.insert(make_pair("D", 1));  map1.insert(pair<string, size_t>("D", 1));  map1.insert(map<string, size_t>::value_type("D", 1));
	map1.emplace("E", 2);										//与上一句，map/set返回<指定关键字的元素的迭代器, 是否插入成功的bool值>的pair，multimap/multiset返回新元素的迭代器
	map1.insert(map2.begin(), map2.end());						//返回void，map/set插入非重复关键字元素，multimap/multiset插入所有元素
	map1.insert({ { "D",1 },{ "E",2 } });						//返回void，map/set插入非重复关键字元素，multimap/multiset插入所有元素
	map1.insert(map1.begin(), { "D",2 });						//提示从迭代器处开始搜索新元素的存储位置，返回具有给定关键字的元素的迭代器
	map1.emplace(map1.begin(), "D", 2);							//提示从迭代器处开始搜索新元素的存储位置，返回具有给定关键字的元素的迭代器
	map1.erase("A");											//删除所有指定关键字的元素，返回size_t表明删除了几个
	map1.erase(map1.begin());									//删除迭代器指定的元素，故迭代器不能为end()且必须指向一个存在的元素，返回指向下一个元素的迭代器
	map1.erase(map1.begin(), map1.end());						//删除范围元素
	map1.clear();
	map1 == map2;
	map1 < map2;
	map1.begin();

	//值类型
	std::map<std::string, size_t>::value_type;					//map的键值对类型，此处为pair<string, size_t>
	std::map<std::string, size_t>::key_type;					//map的关键字类型，此处为string
	std::map<std::string, size_t>::mapped_type;					//map的关键字对应值类型，此处为size_t
//	std::set<int>::value_type;									// = std::set<int>::key_type
	auto map_it = map1.begin();
	map_it->first;												//map/set的关键字是const的
	map_it->second;												//map关键字的值是非const的

	//（非const的）map/unordered_map下标
	map1["A"];													//返回关键字为"A"的元素的值，若不存在则创建该关键字的元素，并值初始化
	map1.at("A");												//返回关键字为"A"的元素的值，若不存在则异常

	//访问元素
	map1.find("A");												//返回第一个关键字为"A"的元素的迭代器，找不到则返回end()
	map1.count("A");											//返回关键字为"A"的元素数量
	map1.lower_bound("A");										//返回第一个关键字不小于"A"的元素（不适用无序容器）
	map1.upper_bound("D");										//返回第一个关键字大于"D"的元素（不适用无序容器）
	map1.equal_range("D");										//返回关键字为"D"的元素的范围(pair)，不存在则返回(end(), end())

	//unordered_map/unordered_multimap <unordered_map>，unordered_set/unordered_multiset <unordered_set> 关键字无序且不可重复/可重复的map/set
	//无序容器使用 桶 保存元素，计算哈希函数值相同的元素保存在同一个桶中
	std::unordered_map<std::string, size_t> unmap;
	using sd_multiset = std::unordered_set<Sales_data, decltype(hasher)*, decltype(eqOp)*>;
	sd_multiset sdset(10, hasher, eqOp);						//自定义类型需自定义哈希函数和相等运算符
//	unordered_set<Foo, decltype(FooHash)*> fooset(10, FooHash);	//类中已经定义==运算符时，可只重载哈希函数
	//桶接口
	unmap.bucket("A");											//关键字为"A"的元素在哪个桶
	unmap.bucket_count();										//正在使用的桶的数目
	unmap.bucket_size(2);										//第2个桶有多少元素
	unmap.max_bucket_count();									//容器容纳的最多的桶的数量

	//桶迭代
	std::unordered_map<std::string, size_t>::local_iterator;	//访问桶中元素的迭代器类型
	std::unordered_map<std::string, size_t>::const_local_iterator;//const版本
	unmap.begin(1);												//桶1的迭代器
//	unmap.begin();												//unordered_map的迭代器

	//哈希策略
	unmap.load_factor();										//(float)平均每个桶的元素数量
	unmap.max_load_factor();									//(float)平均桶的大小，容器在需要时添加桶来保证load_factor() <= max_load_factor()
	unmap.rehash(5);											//重组存储，使得bucket_count() >= 5（参数值），且bucket_count() > size()/max_load_factor()
	unmap.reserve(10);											//重组存储，使得容器保存10（参数值）个元素且不必rehash
}

//pair <utility>
void fun17()
{
	std::pair<std::string, int> pair1;
	std::pair<std::string, int> pair2("Ac", 2);	//等同 std::pair<std::string, int> pair2/* = */{ "Ac", 2 };
	std::make_pair("Ba", 3);					//返回一个pair，自动推断类型
	pair2.first;								//返回pair的第一个数据成员
	pair2.second;								//返回pair的第二个数据成员
	pair1 == pair2;
	pair1 < pair2;
}

//new delete 管理动态内存
void fun18()
{
	//有自己构造函数的类类型 加不加括号都会默认构造函数来初始化，内置类型不加括号时值未定义，加括号则值初始化
	int* pi = new int;									//默认初始化，值未定义，空间耗尽时返回bad_alloc异常
//	int* pi = new int();								//值初始化为0
//	std::string* ps = new std::string;					//默认初始化，空string
	std::string* ps = new std::string(10, '9');			//值初始化为"9999999999"
	const int* pci = new const int(1024);				//必须初始化，有默认构造函数的可隐式初始化，其他的需要显式初始化
	int* pia = new int[42];								//默认初始化，值未定义，返回的是指向第一个元素类型的指针，不能begin()和end()
	typedef int arrT2[42];
	int* pia2 = new arrT2;
	std::string* pia3 = new std::string[10]{ "A","ab" };//值初始化
	char* pc = new char[0];								//可创建大小为0的动态分配数组（静态数组大小不能为0），但是pc不可解引用
	//nothrow <new>
//	int* pi2 = new (nothrow) int;						//空间耗尽时返回空指针

	delete pi;											//销毁对象，释放内存。给delete的指针只能指向new分配的动态内存，或者nullptr。忘记会造成内存泄漏
	delete ps;
	delete pci;
	delete[] pia;
	delete[] pia2;
	delete[] pia3;
	delete[] pc;
	pi = nullptr;										//清楚指出不再指向任何对象
	ps = nullptr;
	pci = nullptr;
}

//智能指针，管理动态内存 <memory>
//①不使用内置指针初始化/reset多个智能指针；②不delete get()返回的指针；③不使用get()初始化或reset()另一智能指针；
//④最后一个相应智能指针销毁后，之前get()的指针变为无效；⑤智能指针管理的资源不是new分配的内存，需要传递一个删除器d
void fun19()
{
	//shared_ptr 销毁最后一个时，同时释放所关联内存
	using std::shared_ptr;
	std::string* q;								//尽量不要混合使用智能指针与普通指针
	shared_ptr<std::string> sp1;				//空指针，可以指向一个对象
	shared_ptr<std::string> sp2(sp1);			//sp2拷贝sp1，并递增sp1计数器，sp1中指针必须可转换为sp2模板类的指针，即shared_ptr<T> sp2(sp1);的sp1中指针可转换为T*
//	shared_ptr<std::string> sp2(sp1, d);		//sp2拷贝sp1，使用可调用对象（可使用调用运算符的对象，如函数、某些类对象等）d代替delete
//	shared_ptr<T> ip1(u);						//从unique_ptr u接管对象所有权，并将u置空
//	shared_ptr<T> ip1(q, d);					//从内置指针p接管对象所有权，q指针可转换为T*，使用可调用对象d代替delete
	shared_ptr<int> ip2(new int(1024));			//√
//	shared_ptr<int> ip1 = new int(1024);		//× 需要显式初始化

	sp1;										//判断sp1是否指向一个对象
	*sp1;										//解引用，获得对象
	sp1->c_str();								//获取成员
	sp1.get();									//返回其中保存的普通指针，不要使用返回的指针初始化另一个智能指针
	swap(sp1, sp2);								//等同 sp1.swap(sp2);
	std::make_shared<std::string>("s");			//使用相应类的可选参数形式，初始化返回shared_ptr并指向动态分配的对象
	sp2 = sp1;									//必须同为shared_ptr，所保存的指针也能相互转换，增加sp1计数，减小sp2计数（减为0会释放内存）
	sp1.use_count();							//计数器数值
	sp1.unique();								//计数器数值是否为1
	sp1.reset();								//若sp1是unique的，则释放所指向的对象，且sp1置空
	sp1.reset(q);								//sp1指向内置指针q
//	sp1.reset(q, d);							//sp1指向内置指针q并用d释放q

	//unique_ptr
	using std::unique_ptr;
	unique_ptr<int> up1;
	unique_ptr<std::string> up2(new std::string("SS"));
//	unique_ptr<int> up3(up1);					//× 不支持拷贝
	unique_ptr<std::string> up4;
//	up4 = up2;									//× 不支持赋值
//	unique_ptr<T, D> up5;						//空指针，用D类型对象释放指针
//	unique_ptr<T, D> up5(d);					//空指针，用D类型对象d代替delete释放指针
//	unique_ptr<T, D> up5(new T, d);				//指向一个T对象的指针，用D类型（指针类型如函数指针）对象d代替delete释放指针

	up1;										//判断sp1是否指向一个对象
	*up1;										//解引用，获得对象
	up2->c_str();								//获取成员
	up1.get();									//返回其中保存的普通指针，不要使用返回的指针初始化另一个智能指针
	swap(up2, up4);								//等同 up2.swap(up4);
	up1 = nullptr;								//释放指向的对象，将up1置空
	up1.release();								//放弃控制权，up1置空，返回普通动态内存指针（所以需要普通指针来释放内存）
	up1.reset();								//释放指向的对象，并置空，等同 up1.reset(nullptr);
	up4.reset(q);								//释放指向的对象，并指向q

	//weak_ptr
	std::weak_ptr<int> wi1;
	std::weak_ptr<std::string> wi2(sp1);		//与shared_ptr指向相同对象的指针，T（此处为string）必须能转换为sp1指向的类型
	wi1 = wi2; wi1 = sp1;						//weak_ptr可与weak_ptr或shared_ptr共享对象
	wi1.reset();								//置空
	wi1.use_count();							//相应unique_ptr的数量
	wi1.expired();								//use_count()是否为0
	wi1.lock();									//若无相应unique_ptr则返回一个空unique_ptr，否则返回一个wi1指向对象的unique_ptr

	//指向动态数组的unique_ptr，不支持成员访问的点和箭头运算符，其他操作不变
	unique_ptr<int[]> upa;						//空指针，可以指向一个对象
	unique_ptr<int[]> upa2(new int[10]);
//	unique_ptr<T[]> upa3(p);					//指向内置指针p指向的动态数组，p必须能转换为T*
	upa2[1];									//指针必须指向一个数组，返回下标处对象
	upa.release();								//销毁它管理的指针时自动使用delete[]

	//指向动态数组的shared_ptr，不直接支持管理动态数组，需要自己提供删除器，
	shared_ptr<int[]> spa(new int[10], [](int* p) { delete[] p; });
	spa.get() + 1;								//shared_ptr未定义下标运算符，且不支持指针算术运算，需要此方式访问数组元素
	spa.reset();								//使用自定义删除器释放数组
}
//使用动态内存：①不知道要用多少对象（如容器类）；②不知道所需对象准确类型；③需要在多个对象间共享数据

//allocator <memory>，分配原始的未构造的内存
void fun20()
{
	using std::allocator;
	allocator<std::string> alloc;
	auto const p = alloc.allocate(2);							//为4个string分配内存，未初始化，返回指向分配的第一个元素的指针
	auto q = p;
	alloc.construct(q, 10, 'c');								//在q处构造一个对象（"cccccccccc"）
	alloc.destroy(q);											//对指向的构造的对象调用析构函数来销毁，必须对每个构造的对象都销毁
	alloc.deallocate(p, 2);										//释放分配的内存，大小与分配时必须相同

	std::vector<std::string> vs{ "A","b" };
	auto const p2 = alloc.allocate(8);
	//<memory>
	auto q2 = uninitialized_copy(vs.begin(), vs.end(), p);		//拷贝范围到未构造的原始内存中，必须足够大，返回下一元素指针
	auto q3 = uninitialized_copy_n(vs.begin(), vs.size(), q2);	//拷贝n和元素到q2开始的未构造的原始内存中，返回下一元素指针
	uninitialized_fill(q3, q3 + 2, "c");						//在未构造的原始内存中指定范围内创建对象，值均为"c"的拷贝，返回void
	uninitialized_fill_n(q3 + 2, vs.size(), "c");				//从开始位置在未构造的原始内存中创建n个对象，值均为"c"的拷贝，返回下一元素指针
}

//拷贝初始化、直接初始化
void fun21()
{
	std::string dots(10, '.');				//直接初始化，构造函数
	std::string s(dots);					//直接初始化，拷贝构造函数
	std::string s2 = dots;					//拷贝初始化，拷贝构造函数
	std::string null_book = "9-99";			//拷贝初始化
	std::string s3 = std::string(100, '9');	//拷贝初始化
}

//右值引用：必须绑定到右值（要求转换/计算的表达式、字面常量、返回右值的表达式）的引用（特：但是可以将左值引用传递到右值模板形参T&&），move
void fun22()
{
	int i = 42;
	int& r = i;					//√，左值引用可绑定到左值
//	int&& rr = i;				//×，右值引用不可绑定到左值
//	int& r2 = i * 2;			//×，左值引用不可绑定到要求转换的表达式
	const int& r3 = i * 2;		//√，左值const引用可绑定到要求转换的表达式
	int&& rr2 = i * 2;			//√，右值引用不可绑定到要求转换的表达式
//	int&& rr3 = rr2;			//×，变量是左值
	//<utility> move，使用时最好不使用using std::move;声明，否则若自己定义了单参数版本，会与标准库move相冲突（标准库move使用右值引用模板形参，可匹配任意类型）
	int&& rr4 = std::move(rr2);	//√，move返回给定对象的右值引用，此时rr1中的值不定（使用会产生未知后果），只可以对rr1进行销毁或重赋值处理
}

//运算符重载
//不可重载 ::(作用域) .*(指向成员选择的指针) .(成员选择) ?:(三元条件)
//通常不重载 ,(逗号) &(取地址) &&(逻辑与) ||(逻辑或)
//除了函数调用运算符operator()外，其他重载运算符不能有默认实参
//有operator==，则一般也有operator!=；若类要一个单序比较操作，则有operator<，则一般也有其他关系操作；
//逻辑运算符和关系运算符返回bool，算术运算符返回类类型的值，赋值运算符和复合赋值运算符应返回左侧运算对象的引用
//赋值(=)、下标([])、调用(())、成员访问箭头(->)必须为成员
//复合赋值运算符一般为成员，但并非必须
//递增(++)递减(--)解引用(*)通常为成员
//有对称性的运算符如算数、相等性、关系、位运算符通常为非成员
//输入(>>)输出(<<)必须为非成员
class OverloadOperator
{
	friend std::ostream& operator<<(std::ostream& os, const OverloadOperator& oo);	//第一个参数通常为流引用，第二个参数通常为常量引用
	friend std::istream& operator>>(std::istream& is, OverloadOperator& oo);		//第一个参数通常为流引用，第二个参数通常为非常量引用
	friend bool operator==(const OverloadOperator& lhs, const OverloadOperator& rhs);
	friend bool operator<(const OverloadOperator& lhs, const OverloadOperator& rhs);

public:
	OverloadOperator& operator=(const OverloadOperator&);							//赋值运算符
	OverloadOperator& operator+=(const OverloadOperator&);
private:
	int m_i;
	std::string m_s;
};
std::ostream& operator<<(std::ostream& os, const OverloadOperator& oo)
{
	os << oo.m_i << ": " << oo.m_s;		//输出运算符一般不附加打印换行符
	return os;
}
std::istream& operator>>(std::istream& is, OverloadOperator& oo)
{
	is >> oo.m_i >> oo.m_s;
	if (is)								//输入运算符一般需要检查输入是否成功
	{
		/**/
	}
	else
		oo = OverloadOperator();		//输入发生错误时需要将对象置为合法状态（如默认状态）
	return is;
}
bool operator==(const OverloadOperator& lhs, const OverloadOperator& rhs)
{
	return lhs.m_i == rhs.m_i && lhs.m_s == rhs.m_s;
}
bool operator!=(const OverloadOperator& lhs, const OverloadOperator& rhs)			//通常用==或!=中一个来定义另一个
{
	return !(lhs == rhs);
}
bool operator<(const OverloadOperator& lhs, const OverloadOperator& rhs)			//类中含有==时，如果两个对象时!=的，则一个对象应该可靠地<另外一个
{
	if (lhs.m_i < rhs.m_i)
		return true;
	else if (lhs.m_i == rhs.m_i)
	{
		if (lhs.m_s < lhs.m_s)
			return true;
		else
			return false;
	}
	else
		return false;
}
OverloadOperator operator+(const OverloadOperator& lhs, const OverloadOperator& rhs)//形参通常为常量引用，在有相关复合赋值运算符情况下常用复合赋值来实现算术运算符
{
	OverloadOperator sum = lhs;
	sum += rhs;
	return sum;
}
OverloadOperator& OverloadOperator::operator+=(const OverloadOperator& rhs)
{
	m_i += rhs.m_i;
	m_s += rhs.m_s;
	return *this;
}
class OverloadOperator2
{
public:
	OverloadOperator2& operator=(std::initializer_list<std::string> il);		//initializer_list的赋值运算符
	std::string operator[](size_t n) { return (*strData)[n]; }
	const std::string operator[](size_t n) const { return (*strData)[n]; }

private:
	std::shared_ptr<std::vector<std::string>> strData;
};
OverloadOperator2& OverloadOperator2::operator=(std::initializer_list<std::string> il)
{
	for (auto p = il.begin(); p != il.end(); ++p)
		strData->push_back(*p);
	return *this;
}
//递增递减元素使得类能在元素序列前后移动
class OO_ptr
{
public:
	std::string& deref() const;
	OO_ptr& operator++();									//前置运算符，返回递增后对象的引用
	OO_ptr& operator--();
	OO_ptr operator++(int);									//后置运算符，与前置进行区别，参数增加一个不被使用的形参，返回的是递增前的值
	OO_ptr operator--(int);
	std::string& operator*() const							//解引用运算符，返回元素的引用（尽管可以被定义为如固定返回42等各种操作）
		{ auto p = check(curr, "out of range"); return(*p)[curr]; }
	std::string* operator->() const							//箭头运算符，通过解引用运算符返回元素然后取地址，从而可以从元素对象中获取成员
		{ return &this->operator*(); }
private:
	std::shared_ptr<std::vector<std::string>> check(size_t, const std::string) const;
	std::weak_ptr<std::vector<std::string>> wptr;
	size_t curr = 0;
};
std::shared_ptr<std::vector<std::string>> OO_ptr::check(size_t i, const std::string msg) const
{
	auto ret = wptr.lock();
	if (!ret)
		throw std::runtime_error("unbound OOBlob");
	if (i >= ret->size())
		throw std::out_of_range(msg);
	return ret;
}
OO_ptr& OO_ptr::operator++()
{
	check(curr, "increament out of range");					//若已经指向尾后，则不能再递增，报错
	++curr;
	return *this;
}
OO_ptr OO_ptr::operator++(int)								//形参无需命名。显式调用：p.operator++(0);
{
	OO_ptr ret = *this;										//此处无需检查有效性
	++* this;												//这里再用前置++来检查有效性
	return ret;
}
class PrintStr
{
public:
	PrintStr(std::ostream& o = std::cout, char c = ' ') :os(o), sep(c) {}
	void operator()(const std::string& s) const { os << s << sep; }
private:
	std::ostream& os;
	char sep;
};
void fun23()
{
	PrintStr errors(std::cerr, '\n');
	errors("wrong");		//在cerr流写入"wrong"并输出一个换行符
}

//标准库表示运算符的函数
void fun24()
{
	std::plus<int> intAdd;				//执行int加法的函数对象
	int sum = intAdd(10, 30);			//等同 int sum = 10 + 30;
//	minus<T>  multiplies<T>  divides<T>  modulus<T>  negate<T>取反
//	equal_to<T>  not_equal_to<T>  greater<T>  greater_equal<T>  less<T>  less_equal<T>
//	logical_and<T>  logical_or<T>  logical_not<T>
}

//function模板类 <functional>
int addfun(int i, int j) { return i + j; }
class dividefun
{
public:
	int operator()(int i, int j) { return i / j; }
};
auto modfun = [](int i, int j) {return i % j; };
void fun25()
{
	std::map<std::string, std::function<int(int, int)>> binops = {
		{"+", addfun},								//函数指针（若出现重载函数名会造成二义性）
		{"-",std::minus<int>()},					//标准库函数对象
		{"/",dividefun()},							//自定义函数对象
		{"*",[](int i, int j) {return i * j; }},	//未命名lambda对象
		{"%", modfun}								//已命名lambda对象
	};
	binops["/"](10, 5);
}

//类类型转换
class SmallInt
{
	SmallInt(int i = 0) :val(i) { if (i < 0 || i > 255) throw std::out_of_range("out of range"); }
	operator int() const { return val; }		//类型转换为int
private:
	std::size_t val;
};

//模板函数、显式实例化、显式模板实参、引用折叠、forward、模板特例化
//原则：形参尽量用const引用，函数体中条件判断仅使用<比较运算（或者less<T>()）
//函数模板和类模板成员函数经常定义在头文件
template <typename T>									//模板参数列表，其中关键字typename可换成class；不能<typename T, U>，只能<typename T, typename U>
int compare_a(const T& v1, const T& v2)
{
	if (v1 < v2) return -1;
	if (v1 > v2) return 1;
	return 0;
}
template <unsigned M, unsigned N>				//非类型模板参数，表示了一个值，可为整型、指针、左值引用，实例化时被用户提供的或编译器推断的值代替，绑定到整型的为常量表达式，绑定到指针或引用的需要有静态生存期（static或者nullptr等）
int compare_a(const char(&p1)[M], const char(&p2)[N])
{
	return strcmp(p1, p2);
}
template <>												//模板特例化，用于处理内部实现不同的情况，尖括号内为空
int compare_a(const char* const& p1, const char* const& p2)//特例化版本的T = const char*（指向常量的指针），对T的常量引用（const T&）表明是指向常量的const指针（常量）的引用
{
	return strcmp(p1, p2);
}
template <typename T, class F = std::less<T>>			//默认模板实参，也是从右往左开始有默认实参
int compare_b(const T& v1, const T& v2, F f = F())		//默认函数实参f，绑定到一个可调用对象
{
	if (f(v1 < v2)) return -1;
	if (f(v1 > v2)) return 1;
	return 0;
}
template <typename T>
typename T::value_type top_a(const T& c)				//默认使用作用域运算符访问的名字不是一个类型，所以需要再加一个typename指出该名字（此处value_type）是一个类型
{
	if (!c.empty())
		return c.back();
	else
		return typename T::value_type();
}
template <typename T> void f1(T&);
template <typename T> void f2(T&&);						//可以给T&&传递任何类型参数，但是会造成T推断出左值引用（如int&），在函数体内编写代码进行T类型变量赋值时可能出问题（赋值还是绑定到引用上）
//template <typename T> void f2(const T&);				//为了避免问题，通常用const T&重载，使得这个定义绑定到左值和const右值，上一定义绑定到非const右值
//extern template class TpltCls1<std::string>;			//extern template，模板显式实例化声明语句，extern说明外部文件中已经有一个定义
template int compare_a(const int& v1, const int& v2);	//template，模板显式实例化定义语句（无需再写函数体）
//无法推断T1类型（T2T3可以通过传入的实参推断），所以调用时需要显式指定。调用时传入的第一个模板实参会匹配第一个模板形参，以此类推
template <typename T1, typename T2, typename T3>
T1 sum_a(T2, T3);
template <typename T1, typename T2, typename T3>
T3 sum_b(T1, T2);										//bad function，需要用户指定所有模板参数才能使用
template <typename It>
auto fcn_a(It beg, It end) -> decltype(*beg)			//尾置返回类型只能decltype参数列表中出现的形参，不能使用只在函数体中出现的变量。*beg返回的是元素引用
{
	/*...*/
	return *beg;
}
template <typename It>
auto fcn_b(It beg, It end) -> typename std::remove_reference<decltype(*beg)>::type	//<type_traits> remove_reference移除引用返回元素本身，type是类内类型成员，需要加typename说明
{
	/*...*/
	return *beg;
}
int (*pf1)(const int&, const int&) = compare_a;			//pf1函数指针，指向int版本实例
void func_a(int(*)(const std::string&, const std::string&));//重载函数
void func_a(int(*)(const int&, const int&));				//重载函数
template <typename F, typename T1, typename T2>
void flip_a(F f, T1&& v1, T2&& v2)
{
	f(std::forward<T2>(v2), std::forward<T1>(v1));		//使用时最好不使用using std::forward;声明而直接在使用时完整使用；forward<T>返回T&&，这里会保持实参v1v2所有细节（const、&、&&等）传递给函数f
}
template <typename T> std::string debug_rep(const T&);	//模板函数与非模板函数的重载（函数(1)），打印任何类型
template <typename T> std::string debug_rep(T*);		//模板函数与非模板函数的重载（函数(2)），打印指针类型
std::string debug_rep(const std::string&);				//模板函数与非模板函数的重载（函数(3)），打印string类型
void fun26()
{
	sum_a<long>(1, 2);									//调用时显式指定，等同 sum_a<long, int, int>(1, 2);
	sum_b<int, int, long>(1, 2);						//必须显式指定所有模板实参
	compare_a<long>(12, 1024);							//显式指定模板实参
//	func_a(compare_a);									//×
	func_a(compare_a<int>);								//√，需要显式提供模板实参消除func_a调用歧义
	const char* p1 = "hi", * p2 = "hello";
	compare_a("hi", "hello");							//匹配compare_a(const char(&p1)[M], const char(&p2)[N])（比compare_a(const T& v1, const T& v2)更精确）
	compare_a(p1, p2);									//匹配compare_a(const char* const& p1, const char* const& p2)（模板特例化）

	int i;
	const int ci = 5;
	f1(i);												//推断T为int
	f1(ci);												//推断T为const int
//	f1(5);												//×，不能给左值引用传右值
	//右值引用模板与左值引用模板推断T类型不太相同，函数参数为模板参数类型的右值引用（T&&）时，推断出的T类型的const和引用（&、&&）属性得到保持。
	//引用折叠只能用于简介创建的引用的引用，如类型别名或模板参数。X& &、X& &&、X&& &折叠为X&，X&& &&折叠为X&&
	f2(5);												//推断T为int
	f2(i);												//引用折叠，i（变量）为左值，调用时推断T为int&，引用折叠为int&
	f2(ci);												//引用折叠，ci（变量）为左值，调用时推断T为const int&，引用折叠为const int&

	std::string s("hi");
	const std::string* sp = &s;
	debug_rep("hi");									//函数(1)（T推断char[3]）(2)（T推断const char）(3)（需要const char*到string转换）匹配，调用T*这个特例版本（数组到指针被认为是精确匹配，相比于(3)的转换更加精确）
	debug_rep(s);										//函数(1)（T推断string）(3)匹配，调用非模板函数
	debug_rep(&s);										//函数(1)（T推断string*）(2)（T推断string）匹配，调用T*这个精确匹配版本
	debug_rep(sp);										//函数(1)（T推断string*）(2)（T推断const string）匹配，调用T*这个更特例的版本（相对于const T&版本）
}

//<type_traits>中的标准类型转换
//	对Mod<T>，其中Mod为		若T为					则Mod<T>::type为		若T不为前面类型则Mod<T>::type为
//remove_reference			X& 或 X&&				X					T
//add_const					X& 或 const X 或 函数		T					const T
//add_lvalue_reference		X& 或 X&&				X&					T&
//add_rvalue_reference		X& 或 X&&				T					T&&
//remove_pointer			X*						X					T
//add_pointer				X& 或 X&&				X*					T*
//make_signed				unsigned X				X					T
//make_unsigned				带符号类型				unsigned X			T
//remove_extent				X[n]					X					T
//remove_all_extents		X[n1][n2]...			X					T

//可变参数模板
template <typename T, typename ... Args>			//模板参数包Args
void foo_a(const T& t, const Args& ... rest)		//函数参数包rest（若参数类型为模板参数包，参数则为函数参数包）
{
	std::cout << sizeof...(Args) << " " << sizeof...(rest) << std::endl;//sizeof...返回参数的数目
}
template <typename T>
std::ostream& print_a(std::ostream& os, const T& t) { return os << t; }
template <typename T, typename ... Args>
std::ostream& print_a(std::ostream& os, const T& t, const Args& ... rest)	//...称为包扩展，此处扩展Args
{
	os << t << ", ";
	return print_a(os, rest...);											//递归调用，...称为包扩展，此处扩展rest
}
template <typename ... Args>
std::ostream& errorMsg(std::ostream& os, const Args& ... rest)
{
	return print_a(os, debug_rep(rest)...);									//相当于对rest包内每个元素调用debug_rep
}

//<tuple> tuple
void fun27()
{
	using std::tuple;
	tuple<size_t, size_t, size_t> threeD;							//成员的类型可相同。没设定初始值，则值初始化（有良好定义的值）
	tuple<std::string, std::vector<double>, int, std::list<int>>
		someVal("constants", { 3.14, 2.718 }, 42, { 0,1,2,3,4,5 });	//对应初始化
	tuple<size_t, size_t, size_t> threeD2/* = */{ 1,2,3 };			//初始化

	threeD = { 4,5,6 };												//赋值，不能省略等号
	auto smpl_item = std::make_tuple("as", 3, 20.00);				//生成tuple，根据初始值推断tuple类型
	std::get<1>(smpl_item);											//返回第1项的引用（排列为类似数列，第0、1、2...项），smpl_item为左值，返回的是左值引用
	std::get<1>(std::make_tuple("as", 2));							//make_tuple("as", 2)为右值，返回右值引用
	threeD == threeD2;												//关系运算必须有相同数量成员，对应项类型可以相互转换
	threeD != threeD2;
	threeD < threeD2;
	std::tuple_size<decltype(threeD)>::value;						//返回给定tuple 类型 中成员的数量
	std::tuple_element<1, decltype(threeD)>::type;					//返回指定项（排列为类似数列，第0、1、2...项）成员的类型
}

//<bitset> bitset，每个元素只能为0或1，每个元素占1 bit
void fun28()
{
	using std::bitset;
	bitset<32> bitvec;							//尖括号内为大小，必须常量表达式，说明有多少二进制位；默认值初始化每一位均为0
	bitset<20> bitset2(1U);						//使用值1U的低20位拷贝初始化bitset2（初始化低位为1，其他位为0），若尖括号给出的位数太多，则多的位置0，太少则高位被丢弃
	bitset<32> bitvec3("1100");					//string或字符数组指针初始化，字符串中下标小的字符在高位，所以为00...001100（前28位为0）
	std::string str0("1111111000000011001101");
	bitset<32> bitvec4(str0, 5, 4);				//从str0[5]开始拷贝4个二进制位初始化（最低的4位）
	bitset<32> bitvec5(str0, str0.size() - 4);	//使用最后四个字符初始化
	bitset<10> bitvec6(std::string("aababba"), 2, 4, 'b', 'a');//使用string第2下标开始拷贝4字符初始化bitvec6，其中b被初始化为0，a被初始化为1

	bitvec |= 1UL << 27;						//第27位置位1
	bitvec.any();								//是否有置位（为1）的位
	bitvec.all();								//是否所有位都置位
	bitvec.none();								//是否不存在置位的位（是否都为0）
	bitvec.count();								//置位的位数
	bitvec.size();								//bitvec的位数（也就是尖括号内设定的位数）
	bitvec.test(2);								//（从低位往高位的）第2位是否置位，检查下标越界。置位返回true，否则false
	bitvec.set(3/*, true*/);					//把第3位设置为某bool值，默认bool值为true
	bitvec.set();								//把所有位置为true
	bitvec.reset(3);							//把第3位复位为false
	bitvec.reset();								//把所有位复位为false
	bitvec.flip(3);								//反转第3位的值
	bitvec.flip();								//反转所有位的值
	bitvec[3];									//const版本在第3位置位时返回true，否则false。不检查下标越界。非const版本可以设置指定位的值为特定bool值（或转化为bool的值）
	bitvec.to_ulong();							//转换为unsigned long，不能放入时抛出overflow_error
	bitvec.to_ullong();							//转换为unsigned long long，不能放入时抛出overflow_error
	bitvec.to_string<char>('a', 'b');			//转换为string，其中0用字符'a'表示，1用字符'b'表示
	std::cout << bitvec;						//二进制位打印为0或1输出到cout
	std::cin >> bitvec;							//读取字符存入bitvec。字符非0非1，或者读取位数已经到达设定位数时停止
}

//<regex> 正则表达式(regular expression)，默认使用ECMAScript语言（JavaScript的一个标准），正则在运行时“编译”，故避免创建不必要regex
//正则由元字符和普通字符组成。普通字符就代表它原本的含义；元字符的意义不同于该字符本来的含义，而是有特殊的意义和功能
//正则表达式可以是一个string，迭代器范围、指向空字符结尾的字符数组的指针、一个字符指针和一个计数器、花括号包围的字符列表
//	元字符			含义
//	\				改变字符原本的含义，如\{表示一个{符号，而\d表示一个数字
//	^				^某字符 要求 字符串 以某字符开头，不占位。\^表示一个真正的^符号。
//	$				某字符$ 要求 字符串 以某字符结尾，不占位。\$表示一个真正的$符号。
//	()				分组，改变默认的优先级。\(表示一个真正的(符号。在模式中可以使用\1来表示第1组已然捕获到的东西。
//	\1,\2,\3		第n个组，第一组索引为1
//	\b				指示字符串的边界（也就是指单词和空格间的位置），某字符\b 要求边界的左边是某字符，\b某字符 要求边界的右边是某字符。如"er\b"匹配"never"的"er"而不匹配"verb"的"er"
//	\B				指示字符串的非边界。如"er\B"匹配"verb"的"er"而不匹配"never"的"er"
//	\d				表示一个字符，该字符是0、1、2……9中的某一个
//	\D				表示一个字符，该字符不是0、1、2……9中的任何一个
//	\s				表示一个字符，该字符是空白符（空格、制表符、换页符、回车符、换行符）
//	\S				表示一个字符，该字符不是空白符（空格、制表符、换页符、回车符、换行符）中任意一个
//	\w				表示一个字符，该字符是数字、字母、下划线中的某一个。等价于[(0-9)(a-z)(A-Z)(_)]
//	\W				表示一个字符，该字符不是数字、字母、下划线中的任何一个。等价于[]
//	\n,\t,\f,\r,\v	一个换行符，制表符，换页符，回车符，垂直制表符
//	\xhh,\uhhh		一个十六进制字符或Unicode字符
//	.				表示一个除了\n以外的任意一个字符。\.表示一个真正的.符号。
//	|				字符串1 | 字符串2 表示一个字符串，该字符串是字符串1、字符串2中的一个
//	[]				[字符1字符2字符3...]表示一个字符，该字符是字符1、字符2、字符3……中的某一个。中括号中出现的所有字符都是代表本身意思的字符（没有特殊含义），如[.]只能匹配.符号，而不能匹配任意符号。
//					[^字符1字符2字符3...]表示一个字符，该字符不是字符1、字符2、字符3……中的任何一个
//					[a-z]表示一个字符，该字符是a、b、c...z中的某一个（必须从小到大），[^A-Z]表示一个字符，该字符不是A、B、C...Z中的任何一个，[0-9]表示一个字符，该字符是0、1、2...9中的某一个
//	*				某字符* 要求某字符出现0到多次
//	+				某字符+ 要求某字符出现1到多次
//	?				某字符? 要求某字符出现0次或1次
//	{n}				某字符{n} 要求某字符出现n次
//	{n,}			某字符{n,} 要求某字符出现n到多次
//	{n,m}			某字符{n,m} 要求某字符出现n到m次
//	[[:alnum:]]	 	一个字母或者数字
//	[[:alpha:]]		一个字母
//	[[:blank:]]		一个space或者tab
//	[[:cntrl:]]		一个控制字符
//	[[:digit:]]		一个数字，等于\d
//	[[:d:]]			一个数字，等于\d
//	[[:graph:]]		可打印非空白字符，相当于 [[:alnum:][:punct:]]
//	[[:lower:]]		一个小写字母
//	[[:print:]]		一个可打印字符，包括空白字符
//	[[:punct:]]		一个标点符号字符，但非space, digit, letter
//	[[:space:]]		一个空白字符，等于\s
//	[[:upper:]]		一个大写字母
//	[[:xdigit:]]	一个十六进制数字

//必须使用与输入类型匹配的正则库，即
//	输入序列类型			使用正则表达式类
//string			regex	smatch	ssub_match	sregex_iterator
//const char*		regex	cmatch	csub_match	cregex_iterator
//wstring			regex	wsmatch	wssub_match	wsregex_iterator
//cosnt wchar_t*	regex	wcmatch	wcsub_match	wcregex_iterator

//匹配标志			含义							格式标志				含义
//match_default		等价与format_default			format_default		用ECMAScript规则替换字符串
//match_not_bol		不将首字符作为行首处理			format_sed			用POSIX sed规则替换字符串
//match_not_eol		不将尾字符作为行尾处理			format_no_copy		不输出输入序列中未匹配的部分
//match_not_bow		不将首字符作为单词首处理			format_first_only	只替换子表达式的第一次出现
//match_not_eow		不将尾字符作为单词尾处理
//match_any			多于一个匹配时返回任一匹配
//match_not_null	不匹配空序列
//match_continuous	匹配必须从输入的首字母开始
//match_prev_avail	输入序列包含第一个匹配之前的内容

void fun29()
{
	using std::regex;

	std::string pattern("[^c]ei");							//匹配模式为开头不是c，但是后面接ei的三个字符的字符串
	pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";	//零个或多个字母+匹配模式为开头不是c，但是后面接ei的三个字符+零个或多个字母的模式
	std::string test_str = "receipt freind thief receive";
	std::string number = "(908) 555-1800";
	//格式字符串一些格式：$2 表示第2个子表达式；$` 表示匹配之前的字符；$& 表示整个匹配；$' 表示后随匹配的字符
	std::string fmt = "$2.$5.$7";							//格式，$2表示第2个子表达式（故此处为257三个表达式通过点连接起来形成string）
	std::string dest, temp;
	//定义regex时的标志，使用|隔开，默认添加ECMAScript（namespace regex）
	//icase忽略大小写 nosubs不保存匹配的子表达式 optimize执行速度优先于构造速度 ECMAScript使用ECMA-262语法 （剩余标志为POSIX语法不列举）
	regex r1(pattern, regex::icase);						//正则表达式初始化对象r，regex::icase为对象r如何处理的标志，此处为匹配忽略大小写
	regex r2("([[:alnum:]]+)\\.(cpp|cxx|cc)\\b", regex::icase);	// \\. 前两个\\在c++解释为一个反斜杠\，生成的\.在正则里解释为一个点.。一个或多个字母+.+cpp cxx cc其中之一为结尾的模式
															//两个圆括号，就有两个子表达式，分别为点之前的文件名部分，点之后的文件扩展名部分（本例表示搜索符合要求的C++文件名）
	regex r3("^(13)[4-9]\\d{8}$");							//字符串以13开头，后面跟4-9中的一个，然后跟任意8位数字并结束
	regex r4("(\\()?(\\d{3})(\\))?([-. ])?(\\d{3})([-. ])?(\\d{4})");//7个子表达式：有0或1个(，有三个数字，有0或1个)，有0或1个-. （横杠、点、空格）之一，有三个数字，有0或1个-. 之一，有四个数字
	r2 = r3;
	r2.assign(r3);
	r1.mark_count();										//r1中子表达式数目
	r1.flags();												//返回r1的标志集

	std::smatch results;									//查找结果对象，包含1（整个匹配的string）+n（子表达式也就是分组数）个ssub_match类型的元素，可通过[]下标访问
	//regex_match与regex_search的smatch对象和匹配格式标志可选，标志见前面
	regex_match(test_str,/* results,*/ r1);					//返回bool，整个序列test_str（string/迭代器范围/空字符结尾的字符数组指针）是否与正则表达式r匹配/*并将结果存在smatch类型的results里（results类型需要与seq兼容）*/
	regex_search(test_str, results, r1/*, regex_constants::match_default*/);	//寻找第一个匹配子序列，并将结果存在results里/*，匹配标志指明匹配规则（此处为默认ECMAScript规则）*/，返回bool是否匹配上
	//regex_replace与format的匹配和格式标志可选，标志见前面
	regex_replace(dest.begin(), temp.begin(), temp.end(), r4, fmt/*, std::regex_constants::match_default*/);// /*根据默认匹配模式*/从temp迭代器范围内搜索所有与r4匹配的子串，并将结果按格式fmt写入迭代器位置dest
	regex_replace(number, r4, fmt);							//（根据默认匹配模式）从string number（或者空字符结尾的字符数组指针）搜索所有与r4匹配的子串，并将结果按格式fmt返回
	results.format("$1 - find"/*, std::regex_constants::format_default*/);	//（根据默认格式）从string number（或者空字符结尾的字符数组指针）搜索与r4匹配的子串，并将结果按格式fmt返回
	results.ready();										//是否已经调用regex_match和regex_search设置过smatch对象（results），设置过则返回true。对返回false的results进行操作是未定义的
	results.size();											//返回最近一次匹配的正则表达式中子表达式（包含整个匹配即第0子表达式）的数目，匹配失败则返回0
	results.empty();										//若results.size()为0则返回true
	results.prefix();										//一个ssub_match对象，表示当前匹配之前的序列
	results.suffix();										//一个ssub_match对象，表示当前匹配之后的序列
	//以下length position str省略实参时默认实参为0，实参为n就是要求第n个子表达式，实参为0表示要求整个匹配
	results.length(/*0*/);									//第0个匹配的子表达式（即整个匹配）的大小
	results.length(1);										//第1个匹配的子表达式的大小（此处指string长度）
	results.position(1);									//第1个子表达式距离被查找序列开始的位置
	results.str(1);											//第1个子表达式匹配的string部分
	results[1];												//对应第1个子表达式的ssub_match对象
	results.begin();										//results中sub_match元素范围的迭代器（没有逆向迭代器）

	//ssub_match对象results[1]
	results[1].matched;										//如果子表达式是匹配上的（也就是匹配结果string中含有此表达式，特殊情况如?在字符出现0次时也匹配，但是子表达式为空，是未匹配伤的），则返回true，否则false
	results[1].first; results[1].second;					//指向子表达式首元素和尾后位置的迭代器，若未匹配则这两个迭代器相等
	results[1].length();									//返回匹配串的长度，未匹配则返回0
	results[1].str();										//返回匹配串string，未匹配则返回空string
	std::string s;
	s = results[1];											//ssub_match对象转化为string s，等价于 s = results[1].str();

	std::sregex_iterator it(test_str.begin(), test_str.end(), r1);	//定位到 迭代器范围内符合正则r1要求的字串位置 的迭代器（即调用regex_search）
	std::sregex_iterator end;								//默认初始化为尾后迭代器
	*it;													//返回最近一次搜索（regex_search）结果的smatch对象
	it->str();												//指针调用
	it++;													//查找下一匹配，返回递增后迭代器
	++it;													//查找下一匹配，返回旧值
	it == end;												//都是尾后时相等，非尾后时构造序列和regex对象相等则相等
}

//<random>随机数
//随机数引擎类生成随机unsigned整数序列，随机数分布类生使用引擎返回服从特定概率分布的随机数
//每次运行随机数发生器（引擎对象+分布对象的组合）时，都会生成相同随机数序列。可以通过设置 函数内引擎对象分布对象为static的 或者循环外声明对象 来保持发生器状态（为从之前的基础上运行），以便生成不同序列随机数
void fun30()
{
	std::default_random_engine e;						//生成随机无符号数
	std::default_random_engine e2(2);					//使用整型值2作为种子生成随机无符号数，种子相同会生成相同的随机数序列
	e();												//生成下一个随机数
	std::default_random_engine::result_type;			//引擎生成的unsigned整型类型
	e.seed(time(0));									//使用种子time(0)重置引擎状态（time(0)表示特定时刻到当前经过的时间（秒））
	e.min();											//引擎可生成的最小值
	e.max();											//引擎可生成的最大值
	e.discard(2);										//引擎推进2步

	std::uniform_int_distribution<unsigned> u(0, 9);	//生成[0,9]之间的均匀分布的unsigned随机数（尖括号内表示希望生成的值类型，uniform_int_distribution默认int）
	std::uniform_real_distribution</*double*/> u2(0, 1);//生成[0,1]之间的均匀分布的double随机数（uniform_real_distribution默认double）
	std::normal_distribution<> n(4, 1.5);				//正态分布，均值4，标准差1.5（normal_distribution默认double）
	std::bernoulli_distribution b(/*0.5*/);				//伯努利分布，返回bool值/*，有50%概率返回true*/
	u(e);												//使用引擎e生成随机数并映射到指定分布
	u.max();											//返回u能生成的最大值
	u.min();											//返回u能生成的最小值
	u.reset();											//重建u的状态，使得随后对u的使用不依赖u已经生成的值
}

//异常处理
//通过throw一个表达式引发异常，抛出异常后当前块内throw后面代码不再被执行，程序寻找catch块并一层一层往外找到main，这其中局部对象被自动销毁，找不到对应异常处理的代码时程序退出
//throw表达式对异常对象拷贝初始化，（表达式为类类型，则类须含有可访问的析构函数和拷贝或移动构造函数；表达式为数组或函数类型，则被转换为指针）如果抛出指针，则在任何对应处理代码存在的地方，指针所指对象都必须存在
//catch中的异常声明类型必须是完全类型（所有成员都已经在前面定义），可以使左值引用但不能是右值引用。
//异常按照catch出现顺序逐一匹配，越专门的catch越应靠前出现。异常要与catch声明类型精确匹配，除了①非const向const类型转换②派生类向基类转换③数组/函数向指向数组/函数的指针转换外，其他所有转换规则都不在匹配过程中使用
//当catch无法完整处理异常时，在catch内或catch语句调用的函数内使用throw; （空throw语句）重新抛出异常，让上一层函数处理异常。若catch引用类型，catch内对原来异常对象进行改变的改变对象被传播出去，否则还是原来异常对象被传播出去
class UseExceptCls
{
public:
	UseExceptCls(std::initializer_list<int>);
	virtual void f1() noexcept;						//虚函数不抛出异常
	virtual void f2();								//虚函数可能抛出异常
private:
	std::shared_ptr<std::vector<int>> data;
};
UseExceptCls::UseExceptCls(std::initializer_list<int> il) try :		//函数try语句块，try位置初始值列表冒号前，形参列表右括号后，
	data(std::make_shared<std::vector<int>>(il))
{
}
catch (const std::bad_alloc& e)										//函数try语句块，使得catch可以处理初始值列表和函数内两部分的异常
{/*...*/
}
//合成拷贝控制成员时，若对所有成员和基类所有操作都noexcept，则合成的成员noexcept，否则为noexcept(false)的；自己定义的析构函数无异常说明时，编译器合成异常说明，生成一个与 若编译器为类合成析构函数将得到的异常说明 一致的异常说明
class DeriExCls :public UseExceptCls
{
public:
	void f1() noexcept;								//派生类相应虚函数必须不抛出异常
	void f2() noexcept;								//派生类相应虚函数可以抛出异常也可以不抛出异常
};
void exceptfun() noexcept;							//noexcept不抛出异常。声明和定义必须都出现或都不出现。非成员函数在后置返回类型之前。成员函数const/引用说明符后，final/override/纯虚函数=0之前
void exceptfun2() noexcept(true);					//等同void exceptfun2() noexcept;，不会抛出异常
void exceptfun3() noexcept(false);					//等同void exceptfun3();，可能抛出异常
void exceptfun4() noexcept
{
	throw std::exception();								//noexcept函数中抛出异常会使程序直接结束
}
void exceptfun5() noexcept(noexcept(exceptfun())) {}//exceptfun5函数与exceptfun函数的异常说明相同
void fun31()
{
	int i;
	try
	{
		if (i > 256)
			throw std::overflow_error("too big");//引发异常
	}
	catch (std::overflow_error e)				//异常声明，此处精确匹配
	{
		std::cout << e.what() << std::endl;
	}
	catch (const std::runtime_error& e)			//异常声明，左值引用
	{
		/*...*/
	}
	catch (...)									//捕获所有异常
	{
		/*...*/
		throw;									//catch(...)常与空throw语句一起使用，重新抛出异常
	}
	noexcept(exceptfun());						//返回bool值，exceptfun函数是否不抛出异常（故返回true）
	noexcept(exceptfun3());						//返回false
	void (*pf1)() noexcept = exceptfun;			//√，指针承诺不抛出异常则指向的函数必须也不抛出异常
//	void (*pf2)() noexcept = exceptfun3;		//×，指针承诺不抛出异常则指向的函数必须也不抛出异常
	void (*pf3)() = exceptfun2;					//√，指针可能抛出异常则指向的函数可能抛出也可能不抛出异常
	void (*pf4)() = exceptfun3;					//√，指针可能抛出异常则指向的函数可能抛出也可能不抛出异常
}

//命名空间、using
//using声明引入同名函数时，隐藏相应外层作用域相关声明，若形参列表也相同则报错
//using指示引入同名函数时，发生重载，若形参列表也相同不会报错，在调用时使用作用域运算符指定调用的版本
int globalint = 1;
namespace mynamespace			//namespace后跟命名空间名。命名空间可以定义在不同部分，也可以在不同文件
{
	//只要能出现在全局作用域中的声明就能置于命名空间内，包括：类、变量、函数、模板、其他命名空间等
	class myclass {
	public:
		void func();
	};
	class D :public myclass
	{
		using myclass::func;	//√，using声明语句只能指向基类成员，不能声明特定函数而只能声明名字
//		using std::cout;		//×
//		using namespace std;	//×，using指示不能出现在类作用域中
	};
	double mydouble;
	int myint = 9;
	void f1();
	void f2() { std::cout << "fun" << std::endl; }
	template <typename T>
	void f3(T& v);
	namespace innerns
	{
	}
	inline namespace inlinensp	//内联命名空间，必须在第一次出现时标明inline，后面再出现可以不标
	{
		int testint;
	}
}								//后面不加分号
namespace {						//未命名的命名空间，其中成员有静态声明周期，第一次使用前创建，程序结束时销毁。一般使用未命名的命名空间代替本文件变量静态声明
								//只能在一个文件内定义本文件的未命名的命名空间，不可跨越文件；头文件有未命名的命名空间时，包含该头文件的文件产生不同的未命名的命名空间
	int unnameint;
}
void fun32()
{
	::globalint; 				//使用全局命名空间中的成员
	mynamespace::testint;		//使用内联命名空间内的成员可以不加内联命名空间名，而直接使用外部命名空间名
	unnameint;					//使用未命名的命名空间的成员，一定要与全局作用域内的名字有区别，否则歧义
	namespace my = mynamespace;	//命名空间别名
	my::f1();
	{//using使得名字可以简写，指定了有效范围内使用的是哪个名字
		using std::cout;		//using声明，有效范围从声明开始到所在作用域结束为止
		using namespace my;		//√，using指示，使命名空间内所有名字都可见，有效范围从声明开始到所在作用域结束为止。不能出现在类作用域中
		int myint = 50;			//隐藏了mynamespace::myint
	}
}

//重载new delete
//operator new返回类型必须为void*，第一个形参类型必须为size_t且不能有默认实参，可以有其他形参。void* operator new(size_t, void*)不能被重载。
//operator delete必须声明noexcept，返回类型必须为void，第一个形参类型必须为void*。作为成员函数时可以有第二个形参为size_t类型。
//new和delete运算符可以定义在全局或者类里。作为类成员时，不管是否写static，这两个操作符都是静态的（且不会操纵类的任何数据成员）。重载只是改变内存分配方式，而无法改变基本含义
namespace special
{
	void* operator new(size_t size)
	{
		if (void* mem = malloc(size))		//malloc接受待分配字节数size_t，成功则返回分配空间的指针，失败则返回0
			return mem;
		else
			throw std::bad_alloc();
	}

	void operator delete(void* mem) noexcept
	{
		free(mem);							//free接受malloc返回的void*指针的副本
	}

	void fun()
	{
		std::string s;
		new(&s) std::string("a");			//定位new，可以在某个位置（此处为s的地址）构造对象，某个位置可以是普通内存也可以是动态内存
	}
}

//dynamic_cast、typeid
class runtimeBase
{
public:
	virtual void fun() {}
};
class runtimeDeri :public runtimeBase
{
public:
	void fun() override {}
};
class typeidBase
{
	friend bool operator==(const typeidBase&, const typeidBase&);
protected:
	virtual bool equal(const typeidBase&) const;
};
class typeidDeri : public typeidBase
{
protected:
	bool equal(const typeidBase&) const;
};
bool typeidBase::equal(const typeidBase& rhs) const
{
	/*比较typeidBase对象*/
	return false;
}
bool typeidDeri::equal(const typeidBase& rhs) const
{
	auto r = dynamic_cast<const typeidDeri&>(rhs);			//由于相等才能调用equal，当两个对象都是派生类对象时调用这个，所以可以放心将基类转换为派生类
	/*比较typeidDeri对象*/
	return false;
}
bool operator==(const typeidBase& lhs, const typeidBase& rhs)
{
	return typeid(lhs) == typeid(rhs) && lhs.equal(rhs);	//typeid用法示例。先判断传入的对象动态类型是否一致，然后调用equal实现真正的比较
}
void fun33()
{
	//dynamic_cast 要转换到的目标类型需要有虚函数。被转换的对象必须为目标类型的共有派生类、公有基类、目标类本身之一
	runtimeBase* bp;
	runtimeBase& br = *bp;
	runtimeBase&& brr = runtimeBase();
	runtimeDeri* dp = dynamic_cast<runtimeDeri*>(bp);		//指针转换为指针。类型转换失败则结果为0。对空指针执行dynamic_cast则得到所需类型的空指针
	runtimeDeri& dr = dynamic_cast<runtimeDeri&>(br);		//左值转换为左值。类型转换失败则抛出bad_cast异常
	runtimeDeri&& drr = dynamic_cast<runtimeDeri&&>(brr);	//非左值转换为右值。类型转换失败则抛出bad_cast异常

	int n = 100;
	const type_info& nInfo = typeid(n);						//typeid括号内可以是变量
	nInfo.name();											//类型的名字（名字生成方式依赖于系统）
	nInfo.before(typeid(double));							//一个类的typeid是否在另一个之前（顺序关系依赖于编译器）
	typeid(100) == typeid(int);								//typeid括号内可以是表达式、类型名
	typeid(100) != typeid(int);
	typeidBase obj1;
	typeidBase* p1;
	typeidDeri obj2;
	typeidDeri* p2 = new typeidDeri;
	p1 = p2;
	typeid(obj1) == typeid(p1);								//false
	typeid(obj1) == typeid(*p1);							//true
	typeid(&obj1) == typeid(p1);							//true
	typeid(obj1) == typeid(obj2);							//false
	typeid(obj1) == typeid(typeidBase);						//true
	typeid(*p1) == typeid(typeidBase);						//true
	typeid(p1) == typeid(typeidBase*);						//true
	typeid(p1) == typeid(typeidDeri*);						//false
	runtimeDeri p3;
	runtimeBase* p4 = &p3;
	typeid(*p4) == typeid(runtimeBase);						//false
	typeid(*p4) == typeid(runtimeDeri);						//true
}

//enum
enum class enum8 { VI };			//枚举的前置声明，限定作用域的枚举可以不指定成员大小，默认int
void enumfun(int) {}
void enumfun(enum8) {}
void fun34()
{
	enum enum7 :unsigned long;		//枚举的前置声明，不限定作用域的枚举必须指定成员大小
	enum class enum1				//限定作用域的枚举类型，使用enum class或enum struct（没有访问限定区别），后跟枚举类型名
	{
		one, two, three				//默认情况下枚举值从0开始，依次+1
	};								//结束时有分号
	enum enum2						//不限定作用域的枚举类型，去掉class或struct
	{
		red = 1, yellow, green		//指定一个枚举值之后，后面没指定的依次+1
	};
	enum { floatPrec = 6, doublePrec = 10, double_doublePrec = 10 };//未命名的不限定作用域的枚举类型
//	enum enum4 { red };				//×，重复定义（与enum2::red）。不限定作用域枚举类型的枚举成员 作用域与枚举类型本身相同
	enum class enum5 { red };		//√，限定作用域的枚举类型 与 不限定作用域的枚举类型 相同成员不算重复
	enum enum6 :unsigned long long	//对 限定作用域的枚举类型 指定enum中整数类型，内部枚举值超限会报错（默认int）。（不限定作用域枚举类型的潜在类型足够大，可以容纳枚举值）
	{
		charTyp = 255, shortTyp = 65535, intTyp = 65535, longTyp = 4294967295UL, longlongTyp = 18446744073709551615ULL
	};
	//	enum class enum7 {};			//×，与前置声明的 是否限定作用域 不一致
	//	enum enum7 :long {};			//×，与前置声明的 成员大小 不一致
	//	enum enum8 {};					//×，与前置声明的 是否限定作用域 不一致

	enum2 eyes = green;				//enum2::green
//	enum5 peppers = green;			//×，enum5枚举成员不在作用域中，在作用域中的enum2::green类型错误
	enum5 hair = enum5::red;		//√，使用枚举成员或者另一个同类枚举对象给枚举对象赋值
	constexpr enum1 e = enum1::one;	//枚举成员是const的
	int i = enum2::red;				//√，不限定作用域枚举类型的枚举成员或对象可以隐式转换为整型
//	int j = enum1::one;				//×，限定作用域枚举类型的枚举成员或对象不可以隐式转换为整型
	enumfun(1);						//匹配enumfun(int)
	enumfun(enum8::VI);				//匹配enumfun(enum8)
}

//类成员指针，指向类的非静态成员
class MmbPtrCls
{
public:
	int mmbint;
	void fun();
	static const std::string  MmbPtrCls::* data()				//返回数据成员指针的函数，需要是静态的
	{
		return &MmbPtrCls::str;
	}
	char get_cursor() const;
	char get();
	char get(int ht, int wd) const;
	const int fun2();

	using Action2 = MmbPtrCls & (MmbPtrCls::*)();
	MmbPtrCls& up();
	MmbPtrCls& down();
	enum Directions { UP, DOWN };
	MmbPtrCls& move(Directions);
private:
	static Action2 Menu[];										//静态成员，指向函数的指针的数组
	std::string str;
};
using Action = char (MmbPtrCls::*)(int, int) const;				//成员指针的类型别名
MmbPtrCls& action(MmbPtrCls&, Action = &MmbPtrCls::get);		//使用成员指针的类型别名，指向成员函数指针的形参也可以有默认实参
MmbPtrCls::Action2 MmbPtrCls::Menu[] = {						//定义并初始化指向函数的指针的数组
	&MmbPtrCls::up, &MmbPtrCls::down
};
MmbPtrCls& MmbPtrCls::move(Directions cm) { return (this->*Menu[cm])(); }
void fun35()
{
	const int MmbPtrCls::* pi;									//指向常量或非常量MmbPtrCls内int类型成员的成员指针
	pi = &MmbPtrCls::mmbint;									//指向成员时需在当前有可访问性

	MmbPtrCls obj, * p = &obj;
	auto i = obj.*pi;											//成员指针访问运算符
	i = p->*pi;													//成员指针访问运算符

	const std::string MmbPtrCls::* pdata = MmbPtrCls::data();	//返回成员指针函数的使用
	auto s = obj.*pdata;										//等同 s = obj.*MmbPtrCls::data();

	auto pmf = &MmbPtrCls::get_cursor;							//指向类的成员函数的指针，必须显式使用取地址运算符
	const int (MmbPtrCls:: * pmfi)() = &MmbPtrCls::fun2;			//函数有const则需要加const
	char (MmbPtrCls:: * pmf2)(int, int) const = &MmbPtrCls::get;//指向char get(int ht, int wd) const;，注意指针pmf2的括号，重载函数需要写上形参表示重载的哪个函数
	char c1 = (obj.*pmf)();										//使用指向成员函数的指针时只能通过.*或->*调用，需要加括号，成员指针本身不是可调用对象
	c1 = (p->*pmf2)(0, 0);

	Action fget = &MmbPtrCls::get;								//使用成员指针的类型别名
	action(obj);												//调用 含指向成员函数指针形参 的函数
	action(obj, fget);											//调用 含指向成员函数指针形参 的函数
	action(obj, &MmbPtrCls::get);								//调用 含指向成员函数指针形参 的函数

	obj.move(MmbPtrCls::UP);

	std::vector<std::string> svec;
	std::vector<std::string*> pvec;
	auto fp = &std::string::empty;
	std::function<bool(const std::string&)> fcn = &std::string::empty;//使用function生成成员函数指针的可调用对象。调用成员函数时this对象被隐式传进来，这里括号内是将隐式形参转为显式形参，括号外bool是成员函数返回bool值
	std::function<bool(const std::string*)> pcn = &std::string::empty;
	//	find_if(svec.begin(), svec.end(), fp);						//×，指向成员函数的指针fp不是可调用对象，而find_if需要的是可调用对象
	find_if(svec.begin(), svec.end(), fcn);						//√，function类将成员函数调用转换为正确形式
	find_if(pvec.begin(), pvec.end(), pcn);						//√，function类将成员函数调用转换为正确形式

	//<functional> mem_fn
	find_if(svec.begin(), svec.end(), mem_fn(&std::string::empty));//使用mem_fn自动生成成员函数指针的可调用对象
	auto fn = mem_fn(&std::string::empty);
	fn(*svec.begin());											//可通过对象或者指针调用mem_fn生成的对象
	fn(&svec[0]);												//可通过对象或者指针调用mem_fn生成的对象

	using std::placeholders::_1;
	find_if(svec.begin(), svec.end(), bind(&std::string::empty, _1));//使用bind生成成员函数指针的可调用对象，需要将隐式传入的形参this转为显式传入
	auto fn2 = bind(&std::string::empty, _1);
	fn2(*svec.begin());											//可通过对象或者指针调用bind生成的对象
	fn2(&svec[0]);												//可通过对象或者指针调用bind生成的对象
}

//union
//可以有多个数据成员，但同一时刻只能有一个成员有值，此时其他成员变为未定义状态。除了引用类型之外，数据成员可以是绝大多数类型，包含类类型
//使用含有类类型的union要更复杂，若类类型（如string）成员自定义了默认构造函数或拷贝控制成员，在union中相应函数的合成版本是被删除的。若类中定义了含有删除的拷贝控制成员的union，类的相应成员也是删除的
//union成员默认public，也可以指定protected和private。union空间大小为至少是最大的数据成员的大小
//union可以定义成员函数，包括构造、析构函数，但是不能有虚函数，不能作为基类和派生类
void fun36()
{
	union Token					//union声明
	{
		char cval;
		int ival;
		double dval;
	};

	union						//匿名union，不能包含protected和private，也不能定义成员函数
	{
		char c2;
		int i2;
	};

	Token token1 = { 'a' };		//初始化一个union，初始值默认初始化第一个成员（即此处初始化了其中的cval成员）
	Token token2;				//未初始化的union
	Token* pt = &token1;		//指向union的指针
	token2.dval = 42.0;
	pt->ival = 42;
	c2 = 5;						//匿名union的成员可以直接访问
}
//通过类管理union，通常应用在union中含有类类型成员情况
class Token2
{
public:
	Token2() :tok(INT), ival(0) {}
	Token2(const Token2& t) :tok(t.tok) { copyUnion(t); }//进入函数体内执行copyUnion时，拷贝者（相对于被拷贝者）已经被默认初始化了，按照顺序是初始化的第一个成员，不是string，所以不需要进行特殊处理（如销毁string）
	Token2& operator=(const Token2&);
	~Token2() { if (tok == STR) sval.~basic_string(); }//若存的是string，则需要调用string析构函数手动析构，否则为内置类型可以什么也不做
	Token2& operator=(const std::string&);
	Token2& operator=(char);
	Token2& operator=(int);
	Token2& operator=(double);
private:
	enum { INT, CHAR, DBL, STR } tok;		//直接在最后写变量名，表明该变量的类型是前面这个未命名enum类型
	union {
		char cval;
		int ival;
		double dval;
		std::string sval;					//string有析构函数，所以需要类自定义析构函数
	};
	void copyUnion(const Token2&);
};
void Token2::copyUnion(const Token2& t)
{
	switch (t.tok)
	{
	case INT:  ival = t.ival; break;
	case CHAR: cval = t.cval; break;
	case DBL:  dval = t.dval; break;
	case STR:  new(&sval) std::string(t.sval); break;
	}
}
Token2& Token2::operator=(const Token2& t)
{//在赋值运算符调用时，左侧Token2对象可能已经存储了string值，所以在右侧值赋值过来左侧之前，先把左侧string析构掉
	if (tok == STR && t.tok != STR) sval.~basic_string();
	if (tok == STR && t.tok == STR)			//左右都存了string，可以直接复制过来，不用构造string
		sval = t.sval;
	else									//左右都不是string、左string右不string（此时string已经在之前被析构了）、左不string右string，都可走这条线直接copy
		copyUnion(t);
	tok = t.tok;
	return *this;
}
Token2& Token2::operator=(int i)
{
	if (tok == STR) sval.~basic_string();
	ival = i;
	tok = INT;
	return *this;
}
Token2& Token2::operator=(const std::string& s)
{
	if (tok == STR)
		sval = s;
	else
		new(&sval) std::string(s);			//定位new，本来不是string的时候先构造一个string
	tok = STR;
	return *this;
}

//volatile（尽量不用！也不能解决多线程中先后顺序问题！）
//程序中的某个值可能会被程序的控制或检测之外因素（如被系统时钟定期刷新）改变时，需要声明成volatile。volatile告诉编译器不要对这样的对象进行优化
//volatile具体含义与编译器有关，要使使用了volatile的程序在移植后有效，需要进行某些改变
class Vo
{
public:
	Vo() = default;
	Vo(const volatile Vo&);	//volatile可以与const共存，顺序随意。从volatile进行拷贝的函数
	Vo& operator=(volatile const Vo&);//volatile赋值给非volatile对象
	Vo& operator=(volatile const Vo&) volatile;//volatile赋值给volatile对象
	volatile void fun();	//只有volatile成员函数能被volatile对象调用
};
void fun37()
{
	int i;
	volatile int dis_reg;	//该值可能发生改变
	volatile int* ivp;		//指向一个volatile对象
	int* volatile vip;		//volatile指针，指向一个int
	volatile int* volatile vivp;//指向一个volatile int对象的volatile指针
//	int* ip = &dis_reg;		//×，只能用指向volatile的指针去指向volatile对象，引用类似
	ivp = &dis_reg;			//√
	vip = &i;				//√
	vivp = &dis_reg;		//√

	volatile int iax[10];	//每个元素都是volatile的
	volatile Vo vo;			//类的每个成员都是volatile的
}

//链接指示
//使用链接指示指出非C++函数所用的语言，该语言的编译器需要有权访问且与C++编译器兼容
//若连接到的语言支持重载，则可用链接指示包含进所有重载函数。如C不支持重载，所以只能包含进一个。可以定义与C包含进来的函数相同名字的函数来重载。
extern "C" size_t strlen(const char*);		//单语句链接指示，extern 表示语言的字符串 函数声明
extern "C"									//复合语句链接指示，一次性建立多个链接
{
	int strcmp(const char*, const char*);
	char* strcat(char*, const char*);
}
extern "C"
{
#include <string.h>							//可以包含头文件
}
extern "C" void (*pf)(int);					//指向其他语言的函数的指针
void (*pf2)(int);
//pf = pf2;									//×，类型不同
extern "C" void exf1(void(*)(int));			//给exf1传递的函数指针也必须是C语言函数
extern "C" typedef void FC(int);			//类型别名，FC是指向C函数的指针
void exf2(FC*);								//exf2是C++函数，传进来的形参是要C函数指针
extern "C" double exf3(int i) { /*...*/ }	//使用链接指示进行函数定义，可以令C++函数在其他语言中可用
