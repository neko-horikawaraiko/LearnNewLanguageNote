//////////////////////////////////////////////////////////////
//
//	学习《C++ Primer》笔记，类部分
//	edit by 猫耳堀川雷鼓/neko-horikawaraiko
//
//////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <set>
#include <vector>

//①若一个类需要自定义析构函数，则几乎同样需要自定义拷贝赋值运算符和拷贝赋值函数（基类除外，基类总需要一个虚析构函数，虚析构函数（不管是否= default）也同时阻止了合成移动操作）
//②若一个类需要自定义拷贝构造函数（拷贝赋值运算符），则几乎同样需要自定义拷贝赋值运算符（拷贝构造函数）
//③若一个类需要自定义拷贝构造函数与拷贝赋值运算符，不一定需要自定义析构函数

//若类的 ↓ ，则该类的 →										|  合成析构函数	|  合成拷贝构造函数	|  合成拷贝赋值运算	|  合成默认构造函数	|  合成移动构造函数	|  合成移动赋值运算符
//某成员的析构函数= delete / 不可访问（如private）				|  = delete		|  = delete			|  					|  = delete			|  					|
//某成员的拷贝构造函数= delete / 不可访问						|				|  = delete			|  					|  					|  					|
//某成员的拷贝赋值运算符= delete / 不可访问						|				|  					|  = delete			|  					|  					|
//有const/引用成员											|				|  					|  = delete			|  					|  					|  = delete
//引用成员无类内初始化器，const成员类内无初始化器且未定义默认构造函数	|				|  					|  					|  = delete			|  					|
//析构函数 = delete / 不可访问（如private）						|				|  					|  					|  					|  = delete			|
//某成员有拷贝构造且无移动构造，或未定义拷贝构造且不能合成移动构造		|				|  					|  					|  					|  = delete			|  = delete
//某成员的移动构造函数= delete									|				|  					|  					|  					|  = delete			|
//某成员的移动赋值运算符= delete								|				|  					|  					|  					|  					|  = delete
//有移动操作（移动函数和/或移动赋值运算符）						|				|  = delete			|  = delete			|  					|  					|

//若基类的 ↓ ，则派生类的 →			|  合成默认构造函数	|  合成拷贝构造函数	|  合成拷贝赋值运算符	|  合成析构函数	|  合成移动构造函数	|  合成移动赋值运算符
//默认构造函数= delete / 不可访问		|  = delete			|  					|  					|  				|  					|
//拷贝构造函数= delete / 不可访问		|  					|  = delete			|  					|  				|  					|
//拷贝赋值运算符= delete / 不可访问		|  					|  					|  = delete			|  				|  					|
//析构函数= delete / 不可访问			|  = delete			|  = delete			|  					|  = delete		|  = delete			|
//移动构造函数= delete				|  					|  					|  					|  				|  = delete			|
//移动赋值运算符= delete				|  					|  					|  					|  				|  					|  = delete
//所以基类显式定义拷贝后，还需要移动就需要也显式定义，否则会按照拷贝操作执行；显式定义移动后，必须显式定义拷贝，否则为拷贝为删除的

//拷贝构造函数、拷贝赋值运算符、移动构造函数、移动赋值运算符、析构函数
//赋值运算符需要较好处理自赋值能力，有着析构函数和拷贝构造函数的工作，所以一般先将右侧运算对象拷贝到局部临时对象（主要为指针指向的对象），再销毁左侧对象，再将临时对象拷贝到左侧对象
//一个类未定义拷贝控制成员时，编译器会合成相应的拷贝构造函数、拷贝赋值运算符、析构函数
//一个类未定义任何拷贝控制成员（拷贝构造、拷贝赋值运算符、析构），且类内非static成员都可以移动时，编译器才合成移动构造函数或移动赋值运算符；否则不会合成移动构造函数或移动赋值运算符
//移动操作不会被隐式定义为删除的函数，若通过= default要求编译器生成移动操作，但编译器不能移动所有成员，则会被改为= delete
template <class T> class std::hash;								//友元声明所需
class Sales_data2
{
	friend class std::hash<Sales_data2>;
	friend std::istream& operator>>(std::istream&, Sales_data2&);
	friend std::ostream& operator<<(std::ostream&, const Sales_data2&);
	friend bool operator==(const Sales_data2&, const Sales_data2&);
public:
	Sales_data2() = default;									//= default 显式要求编译器生成合成版本函数 （= default 只能给构造、拷贝、赋值、移动、析构函数）
	Sales_data2(const std::string& s) :bookNo(s) {}
	Sales_data2(const std::string& s, unsigned n, double p) : bookNo(s), units_sold(n), revenue(p* n) {}
	explicit Sales_data2(std::istream& is);
	~Sales_data2() {}											//析构函数，唯一，不能被重载，先执行函数体，然后按初始化逆序销毁成员（此处与合成的析构函数等价）

	Sales_data2(const Sales_data2&);							//拷贝构造函数，第一个参数是本身类型的引用，任何额外参数都有默认值
	Sales_data2& operator+=(const Sales_data2&);
	Sales_data2& operator=(const Sales_data2&);					//赋值运算符，必须定义为成员函数，参数为本类型引用，返回指向左侧运算对象的引用


	std::string isbn() const { return bookNo; }
	Sales_data2& combine(const Sales_data2&) {}

private:
	double avg_price() const { return units_sold ? revenue / units_sold : 0; }
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};
//自定义异常处理类
class out_of_stock :public std::runtime_error
{
public:
	explicit out_of_stock(const std::string& s) :std::runtime_error(s) {}
};

class isbn_mismatch :public std::logic_error
{
public:
	explicit isbn_mismatch(const std::string& s) :std::logic_error(s) {}
	isbn_mismatch(const std::string& s, const std::string& lhs, const std::string& rhs) :
		std::logic_error(s), left(lhs), right(rhs) {}
	const std::string left, right;
};

Sales_data2::Sales_data2(const Sales_data2& orig) :bookNo(orig.bookNo), units_sold(orig.units_sold), revenue(orig.revenue)//拷贝构造函数（此处与合成的拷贝构造函数等价）
{
}

Sales_data2 operator+(const Sales_data2& lhs, const Sales_data2& rhs)
{
	Sales_data2 sum = lhs;
	sum += rhs;
	return sum;
}

Sales_data2& Sales_data2::operator+=(const Sales_data2& rhs)
{
	if (isbn() != rhs.isbn())
		throw isbn_mismatch("wrong isbns", isbn(), rhs.isbn());

	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;
}

bool operator==(const Sales_data2& lhs, const Sales_data2& rhs)
{
	return lhs.isbn() == rhs.isbn() &&
		lhs.units_sold == rhs.units_sold &&
		lhs.revenue == lhs.revenue;
}

bool operator!=(const Sales_data2& lhs, const Sales_data2& rhs)
{
	return !(lhs == rhs);
}

Sales_data2& Sales_data2::operator=(const Sales_data2& rhs)//赋值运算符（此处与合成的赋值运算符等价）
{
	bookNo = rhs.bookNo;
	units_sold = rhs.units_sold;
	revenue = rhs.revenue;
	return *this;
}

namespace std											//打开std命名空间
{
	template <>											//模板特例化必须定义在原始模板命名空间中
	struct hash<Sales_data2>							//特例化的hash，必须定义以下三方面
	{
		typedef size_t result_type;						//(1)类型成员result_type（名字不能变），为调用运算符的返回类型
		typedef Sales_data2 argument_type;				//(1)类型成员argument_type（名字不能变），为调用运算符的参数类型，需要此类型有==
		size_t operator()(const Sales_data2& s) const;	//(2)重载的调用运算符，接受同期关键字类型的对象，返回size_t
		//合成拷贝控制成员和默认构造函数					//(3)默认构造函数和拷贝赋值运算符
	};

	size_t hash<Sales_data2>::operator()(const Sales_data2& s) const
	{
		return hash<string>()(s.bookNo) ^				//常定义哈希异或运算。定义三个成员的哈希值从而与operator==兼容
			hash<unsigned>()(s.units_sold) ^
			hash<double>()(s.revenue);
	}
}														//关闭std命名空间，后面无分号

//////////////////////////////////////////////////////////////////////////
class HasPtr_Ptr//类指针型HasPtr
{
public:
	HasPtr_Ptr(const std::string& s = std::string()) :ps(new std::string(s)), use(new size_t(1)) {}
	HasPtr_Ptr(const HasPtr_Ptr& p) :ps(p.ps), use(p.use) { ++*use; }
	HasPtr_Ptr& operator=(const HasPtr_Ptr&);
	~HasPtr_Ptr();			//需要自定析构函数释放动态内存

private:
	std::string* ps;
	size_t* use;
};

HasPtr_Ptr& HasPtr_Ptr::operator=(const HasPtr_Ptr& rhs)
{//此处HasPtr_Ptr类似shared_ptr，使用引用计数器标明指向同一对象的指针数量，所以不需要将ps指向的对象拷贝到临时对象
	++* rhs.use;			//递增引用计数，避免自赋值导致use递减时为0而销毁对象
	if (-- * use == 0)		//析构函数作用，销毁左侧对象
	{
		delete ps;
		delete use;
	}
	ps = rhs.ps;			//拷贝到左侧对象
	use = rhs.use;
	return *this;
}

HasPtr_Ptr::~HasPtr_Ptr()
{
	if (-- * use == 0)
	{
		delete ps;
		delete use;
	}
}

//////////////////////////////////////////////////////////////////////////
class HasPtr_Val//类值型HasPtr
{
	friend void swap(HasPtr_Val&, HasPtr_Val&);
public:
	HasPtr_Val(const std::string& s = std::string()) :ps(new std::string(s)) {}
	HasPtr_Val(const HasPtr_Val& p) :ps(new std::string(*p.ps)) {}
	HasPtr_Val(HasPtr_Val&& p) noexcept :ps(p.ps) { p.ps = 0; }
//	HasPtr_Val& operator=(const HasPtr_Val&);
	HasPtr_Val& operator=(HasPtr_Val);			//定义了swap的类常用swap实现赋值运算符
	~HasPtr_Val() { delete ps; }

private:
	std::string* ps;
	size_t* use;
};

inline void swap(HasPtr_Val& lhs, HasPtr_Val& rhs)
{
	using std::swap;			//使得下面调用若有特化版本就调用特化版本，不存在时才调用模板版本
	swap(lhs.ps, rhs.ps);		//只交换指针即可
}

/*
HasPtr_Val& HasPtr_Val::operator=(const HasPtr_Val& rhs)
{//此处HasPtr_Val类似值行为，需要将ps指向的对象拷贝到临时对象
	std::string* newp = new std::string(*rhs.ps);	//右侧运算对象拷贝到局部临时对象
	delete ps;
	ps = newp;
	return *this;
}*/

HasPtr_Val& HasPtr_Val::operator=(HasPtr_Val rhs)		//根据传递来的是左值/右值，分别拷贝/移动了rhs进来，所以同时有拷贝和移动赋值运算符的作用
{
	swap(*this, rhs);
	return *this;
}

class useHP_Val
{
public:
	HasPtr_Val h;
};

void swap(useHP_Val& lhs, useHP_Val& rhs)
{
	using std::swap;
	swap(lhs.h, rhs.h);		//虽然前面使用了std::swap，但是此处根据实参类型不仅查找std还查找实参类HasPtr_Val所在的命名空间，最终选择最佳匹配HasPtr_Val定义的swap函数来实现
}
//////////////////////////////////////////////////////////////////////////
class NoCopy
{
public:
	NoCopy() = default;
	NoCopy(const NoCopy&) = delete;				//= delete 显式要求编译器阻止拷贝 （= delete 适用除析构外任何函数）
	NoCopy& operator=(const NoCopy&) = delete;	//= delete 显式要求编译器阻止赋值
	~NoCopy() = default;

private:
	int i;
};

//////////////////////////////////////////////////////////////////////////
class StrVec //string的vector
{
public:
	StrVec() :elements(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec&);					//拷贝构造
	StrVec(StrVec&&) noexcept;				//移动构造函数，第一个参数为该类型的右值引用，额外参数都有默认实参，通常使用noexcept表示不会抛出异常，移动后源对象必须有效、可析构
	StrVec& operator=(const StrVec&);		//拷贝赋值运算符
	StrVec& operator=(StrVec&&) noexcept;	//移动赋值运算符，通常使用noexcept表示不会抛出异常，移动后源对象必须有效、可析构
	~StrVec();								//析构

	void								push_back(const std::string&);
	size_t								size() const		{ return first_free - elements; }
	size_t								capacity() const	{ return cap - elements; }
	std::string*						begin() const		{ return elements; }
	std::string*						end() const			{ return first_free; }
	template <typename ... Args> void	emplace_back(Args&&...);
private:
	std::string* elements;		//数组首指针
	std::string* first_free;	//第一个空闲元素指针
	std::string* cap;			//尾后指针
	static std::allocator<std::string> alloc;

	void									chk_n_alloc()	{ if (size() == capacity()) reallocate(); }	//是否容量够
	std::pair<std::string*, std::string*>	alloc_n_copy(const std::string*, const std::string*);		//将一个范围拷贝进来
	void									free();														//销毁元素，释放空间
	void									reallocate();												//重分配空间，并将原来元素拷贝进来
};

StrVec::StrVec(StrVec&& s) noexcept		//声明和定义中都要有noexcept，初始化列表在noexcept后面
	:elements(s.elements), first_free(s.first_free), cap(s.cap)
{
	s.elements = s.first_free = s.cap = nullptr;	//通过置空指针置于可析构状态
}

StrVec& StrVec::operator=(StrVec&& rhs) noexcept
{
	if (this != &rhs)
	{
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}

template<typename ...Args>
inline void StrVec::emplace_back(Args && ... args)
{
	chk_n_alloc();
	alloc.construct(first_free++, std::forward<Args>(args)...);				//Args包和args包都扩展
}

void StrVec::reallocate()
{
	auto newcapacity = size() ? 2 * size() : 1;
	auto first = alloc.allocate(newcapacity);
	auto last = uninitialized_copy(make_move_iterator(begin()),				//普通迭代器转为移动迭代器，由于uninitialized_copy使用construct“拷贝”元素
		make_move_iterator(end()), first);									//所以传入移动迭代器后解引用为右值，const将使用移动构造函数来构造元素
	free();
	elements = first;
	first_free = last;
	cap = elements + newcapacity;
}

//////////////////////////////////////////////////////////////////////////
class Foo
{
public:
	Foo& operator=(const Foo&)&;		//指出返回的*this为一个左值
	Foo sorted()&&;						//指出返回的*this为一个右值；重载时引用限定符要么都加要么都不加，&&通过右值调用本函数；引用限定符只能用于非static成员函数
	Foo sorted() const&;				//&在const后；重载时引用限定符要么都加要么都不加，&通过左值调用本函数
};

Foo& Foo::operator=(const Foo&)&		//&必须同时出现在声明和定义中
{
	/*do something*/
	return *this;
}

//////////////////////////////////////////////////////////////////////////
//虚函数、派生、继承、友元
//虚函数：任何除了构造函数之外的非静态函数都可以是虚函数
class Quote																	//派生类定义前基类必须已定义
{
public:
	Quote() = default;
	Quote(const std::string& book, double sale_price) :bookNo(book), price(sale_price) {}
	Quote(const Quote&) = default;
	Quote(Quote&&) = default;
	Quote& operator=(const Quote&) = default;
	Quote& operator=(Quote&&) = default;
	virtual ~Quote() = default;												//基类通常定义虚析构函数以保证执行正确的析构函数版本（基类的还是派生类的）

	std::string isbn() const { return bookNo; }
	virtual double net_price(size_t n) const {return n * price;}			//需要派生类自定义的函数定义为虚函数，virtual关键字只能出现在类内声明，不能用于类外部定义
	virtual Quote* clone() const& { return new Quote(*this); }
	virtual Quote* clone()&& { return new Quote(std::move(*this)); }
private:
	std::string bookNo;
protected:
	double price = 0.0;
};
//派生类不继承默认、拷贝、移动构造函数
//抽象基类，含有纯虚函数，不能创建抽象基类对象
class Disc_quote :public Quote
{
public:
	Disc_quote() = default;													//基类成员也会被默认初始化
	Disc_quote(const std::string & book, double price, size_t qty, double disc)//使用基类构造函数初始化基类部分
		:Quote(book, price), quantity(qty), discount(disc) {}
	Disc_quote(const Disc_quote& d) :Quote(d) {}							//需显式使用基类拷贝构造函数，否则基类部分会执行默认构造函数
	Disc_quote& operator=(const Disc_quote& d)								//需显式使用基类拷贝赋值运算符
		{ Quote::operator=(d);/**/ }
	~Disc_quote();															//无需显式调用基类析构函数，基类析构函数自动执行

	double net_price(size_t n) const = 0;									//纯虚函数，类内声明的最后写 = 0，可以进行定义，但只能在类外部定义
	std::pair<size_t,double> discount_policy() const
		{ return { quantity,discount }; }
protected:
	size_t quantity = 0;
	double discount = 0.0;
};
//派生列表的访问说明符表示派生类从基类继承的成员是否 对用户/对派生类的派生类 可见，派生类本身可访问基类public、protected成员
//基类中的 → 成员，经过派生列表 ↓ 的访问说明符，	public		protected	private
//变为对外/对派生类的派生类↘
//				public							public		protected	private
//				protected						protected	protected	private
//				private							private		private		private
//可将派生类对象绑定到基类引用/指针，基类引用/指针只绑定/指向派生类对象的基类部分上，可把派生类对象当做基类对象用（隐式转换，只适用 引用/指针 ），但不能 隐式 将基类转换成派生类
class Bulk_Quote :public Disc_quote											//派生类在前置声明时不加派生列表，即class Bulk_Quote;
{
public:
	//对构造函数的using保持基类来的访问级别不变（基类public这里也public，基类private这里也private），保持基类来的explicit和constexpr不变
	//基类某构造函数的形参含有默认实参时，派生类using继承该构造函数的多个版本，为从右往左每次减少一个形参
	//派生类定义了相同形参的构造函数的话，便不会从基类继承相应构造函数，而使用派生类自己的这一个构造函数
	using Disc_quote::Disc_quote;											//使用using显式注明继承直接基类的构造函数，此时派生类自己的初始成员默认初始化或使用类内初始值初始化

	//override好习惯，位置在形参列表、const、引用限定符后面，显式表明重写的哪个虚函数；若不重写虚函数，派生类直接继承基类版本，若基类的为纯虚函数，不重写则仍为纯虚函数，类成为抽象基类；
	//开头virtual可省略；重写函数必须相同类型形参，最好默认实参相一致，返回类型除非返回本类的指针（基类返回基类指针，派生类返回派生类指针），其他情况必须也相同
	double net_price(size_t n) const override;
	static void statemem() {}												//静态成员，在某基类及其派生类中只存在一个，可通过基类、派生类、对象访问
	Bulk_Quote* clone() const& { return new Bulk_Quote(*this); }
	Bulk_Quote* clone()&& { return new Bulk_Quote(std::move(*this)); }

private:
	size_t min_quantity = 0;
};

double Bulk_Quote::net_price(size_t n) const
{
	if (n >= min_quantity)
		return n * (1 - discount) * price;
	else
		return n * price;													//可以用return Quote::net_price(n);加作用域运算符强行指定虚函数版本
}

double print_total(std::ostream& os, const Quote& item, size_t n)
{
	double ret = item.net_price(n);
	os << "ISBN: " << item.isbn() << " # sold: " << n << " total due: " << ret << std::endl;
	return ret;
}

class Basket
{
public:
	//将调用shared_ptr隐藏进类代码中。若直接开写可能会添加shared_ptr<Quote>(new Quote(sale))，此时sale若为Bulk_Quote对象，分配动态内存只会截取Quote部分。为避免错误添加clone函数
	void add_item(const Quote& sale) { items.insert(std::shared_ptr<Quote>(sale.clone())); }
	void add_item(Quote&& sale) { items.insert(std::shared_ptr<Quote>(std::move(sale).clone())); }
	double total_receipt(std::ostream&) const;
private:
	static bool compare(const std::shared_ptr<Quote>& lhs, const std::shared_ptr<Quote>& rhs)
		{ return lhs->isbn() < rhs->isbn();	}
	std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{ compare };//使用花括号避免被解析为定义了一个函数
};

double Basket::total_receipt(std::ostream& os) const
{
	double sum = 0.0;
	for (auto iter = items.cbegin(); iter != items.cend(); iter = items.upper_bound(*iter))	//upper_bound返回一批相同元素的下一个位置
		sum += print_total(os, **iter, items.count(*iter));									//count返回相同元素数目（通过Basket::compare比较isbn，相同isbn算作相同元素）
	os << "Total Sale: " << sum << std::endl;
	return sum;
}

//类名后加final防止被继承，函数形参列表与尾置返回类型后 加final防止被其派生类重写
class NoDerived final {};			//加了final的类从其他基类继承
//class Bad :public NoDerived {};	//×，不可继承NoDerived类

//派生类的成员/友元只能通过派生类对象（而不能通过基类对象）访问基类的protected成员
//派生类D public继承基类B时，用户代码可使用派生类D到基类B的转换，protected或private继承时用户代码不能使用转换；
//派生类D的成员和友元永远可使用派生类D到基类B的转换；
//派生类D public或protected继承基类B时，派生类D的派生类的成员和友元可使用派生类D到基类B的转换，private继承时不能使用转换
//基类友元可访问基类与派生类继承自基类的成员，不可访问派生类protected/private成员，派生类友元可访问派生类自己的成员，不可访问基类protected/private成员
//友元（类）的基类和派生类不能访问原来的基类和派生类的protected/private成员
//使用using更改访问级别，也可以使用using基类的重载函数，并在派生类中覆盖其中几个的方法来定义需要的若干重载函数（其他未被覆盖的就会使用基类的重载版本）
class B
{
	friend class friendB;
public:
	B() :mem(0) {}
	void public_mem() {}
	void memfun() {}
	virtual int intfun();			//虚函数
protected:
	void protected_mem() {}
	int mem;
private:
	void private_mem() {}
};
class pubD :public B
{
public:
	pubD() = default;
	pubD(int i) :mem(i) {}			//初始化pubD::mem
	void memfun(int i) {}			//此处覆盖而不重载基类的memfun，即使形参列表或返回值不同
	int intfun(int);				//不是虚函数，新的函数
									//同时pubD继承B::intfun()定义
	virtual int intfun2();			//虚函数
protected:
	int mem;						//隐藏（覆盖）基类同名成员，但是除了虚函数最好不要这样做
private:
	void f(pubD d)					//因为是B的派生类
	{
		B* b = &d;
		b->public_mem();
		d.public_mem();
		/*B::*/protected_mem();		//所以可访问基类protected成员
//		b->protected_mem();			//×，不能通过基类对象访问
		d.protected_mem();			//√，可以通过派生类对象访问
	}
	int j;
};
class proD :protected B
{
	void f2(pubD d) { B* b = &d; }
};
class priD :private B
{
	void f3(pubD d) { B* b = &d; }
public:
	using B::protected_mem;			//在派生类中的public使用using可强制把基类的protected成员改为public的
};
class pubpubD :public pubD
{
public:
	int intfun();					//override B::intfun()
	int intfun(int);				//隐藏pubD::intfun(int)，并重载intfun
	int intfun2();					//override pubD::intfun2()
};
class pubpriD :public priD
{
	void f4(pubD d) { /* B* b = &d; */ }//×，B不可访问
};
class friendB
{
	void f5(B b) { b.private_mem(); }
	void f6(pubD d)
	{
		/* d.j; */					//×，派生类protected与private不可访问
		d.private_mem();			//√，基类protected与private可访问
	}
};
class pubfriendB :public friendB
{
	void f7(B b)
	{
		/* b.protected_mem(); */	//×，友元（类）的派生类与基类对本来的protected与private不可访问
		b.public_mem();
	}
};
struct D1 : /*public*/ B {};		//默认public继承
class D2 : /*private*/ B {};		//默认private继承
void fun()
{
	Bulk_Quote bulk;
	Bulk_Quote* bulkP = &bulk;
	Quote* itemP = &bulk;
	bulkP->discount_policy();		//√，Bulk_Quote本身没有该成员，但是在外层的Disc_quote中找到成员
//	itemP->discount_policy();		//×，Quote本身没有该成员，在外层也找不到找到成员，但是不会往内层（Disc_quote）中找

	B* pb;
	pubD pubd;
	proD prod;
	priD prid;
	pb = &pubd;		//√
//	pb = &prod;		//×
//	pb = &prid;		//×
	prid.protected_mem();
//	prod.protected_mem();			//×，只有私有继承类priD更改了访问级别
	{//对象调用函数时静态绑定，只从对象本类找函数名字；指针调用函数时若不是虚函数则按照在声明指针的类找，虚函数则动态绑定，运行时在被指对象类找函数名字调用，找不到往基类找
		B Bobj; pubD pubDobj; pubpubD pubpubDobj;
		B* bp1 = &Bobj, * bp2 = &pubDobj, * bp3 = &pubpubDobj;
		pubD* dp1 = &pubDobj, * dp2 = &pubpubDobj;
		pubpubD* dp3 = &pubpubDobj;
		Bobj.intfun();			//√，调用B::intfun()
//		Bobj.intfun(1);			//×，找到B::intfun()，参数不匹配
//		Bobj.intfun2();			//×，找不到B::intfun2()
//		pubDobj.intfun();		//×，找到pubD::intfun()，参数不匹配
		pubDobj.intfun(1);		//√，调用pubD::intfun(int)
		pubDobj.intfun2();		//√，调用pubD::intfun2()
		pubpubDobj.intfun();	//√，调用pubpubD::intfun()
		pubpubDobj.intfun(1);	//√，调用pubpubD::intfun(int)
		pubpubDobj.intfun2();	//√，调用pubpubD::intfun2()

		bp1->intfun();			//√，运行时调用B::intfun()
		bp2->intfun();			//√，运行时调用B::intfun()
		bp3->intfun();			//√，运行时调用pubpubD::intfun()
//		dp1->intfun();			//×，找到pubD::intfun()，参数不匹配，不去找pubpubD::intfun()
//		dp2->intfun();			//×，找到pubD::intfun()，参数不匹配，不去找pubpubD::intfun()
		dp3->intfun();			//√，运行时调用pubpubD::intfun()

//		bp1->intfun(1);			//×，找到B::intfun()，参数不匹配，不去找pubD::intfun(int)
//		bp2->intfun(1);			//×，找到B::intfun()，参数不匹配，不去找pubD::intfun(int)
//		bp3->intfun(1);			//×，找到B::intfun()，参数不匹配，不去找pubD::intfun(int)
		dp1->intfun(1);			//√，调用pubD::intfun(int)
		dp2->intfun(1);			//√，调用pubD::intfun(int)
		dp3->intfun(1);			//√，调用pubpubD::intfun(int)

//		bp1->intfun2();			//×，找不到B::intfun2()
//		bp2->intfun2();			//×，找不到B::intfun2()
//		bp3->intfun2();			//×，找不到B::intfun2()
		dp1->intfun2();			//√，运行时调用pubD::intfun2()
		dp2->intfun2();			//√，运行时调用pubpubD::intfun2()
		dp3->intfun2();			//√，运行时调用pubpubD::intfun2()
	}
}

//////////////////////////////////////////////////////////////////////////
//模板类，其成员函数只有使用到时才被实例化
template <typename> class BlobPtr;										//为了在BlobPtr声名友元需要的前置声明
template <typename> class Blob;											//为了operator==中Blob类需要的前置声明
template <typename T> bool operator==(const Blob<T>&, const Blob<T>&);	//为了在Blob声名友元需要的前置声明
template <typename T>													//一个模板参数名（这里为T）在一个特定模板参数列表中只能出现一次，模板参数名会隐藏外层作用域中声明的相同名字
class Blob
{
public:
	typedef T value_type;												//使用value_type相当于使用T
	typedef typename std::vector<T>::size_type size_type;				//加上typename告诉编译器vector<T>::size_type是一个类型而不是一个成员

	friend class BlobPtr<T>;											//加上<T>将访问权限限制在相同的T类型
	friend bool operator==<T> (const Blob<T>&, const Blob<T>&);			//加上<T>将访问权限限制在相同的T类型

	Blob();
	Blob(std::initializer_list<T> il);
	template <typename It> Blob(It b, It e);							//成员模板，与类模板参数不同表示可接受不同类型迭代器

	size_type	size() const { return data->size(); }
	bool		empty() const { return data->empty(); }
	void		push_back(const T& t) { data->push_back(t); }
	void		puch_back(T&& t) { data->push_back(std::move(t)); }
	void		pop_back();

	T& back();
	T& operator[](size_type i);
private:
	std::shared_ptr<std::vector<T>> data;

	void check(size_type i, const std::string& msg) const;
};

template <typename T>													//类的类型参数，必须分开
template <typename It>													//构造函数的类型参数，必须分开
Blob<T>::Blob(It b, It e) :data(std::make_shared<std::vector<T>>(b, e)) {}//调用时通过实参自动推断成员模板实参

template <typename U>													//定义与声明的模板参数名可不同，但是要有相同数量和种类的参数
Blob<U>::Blob() :data(make_shared<std::vector<U>>()) {}

template <typename T>
Blob<T>::Blob(std::initializer_list<T> il) : data(make_shared<std::vector<T>>(il)) {}

template <typename T>													//定义在外部的函数也要加这一句
void Blob<T>::check(size_type i, const std::string& msg) const			//类名后面有<T>
{
	if (i >= data->size())
		throw std::out_of_range(msg);
}

template<typename T>
void Blob<T>::pop_back()
{
	check(0, "pop_back in empty blob");
	data->pop_back();
}

template <typename T>
T& Blob<T>::back()
{
	check(0, "back on empty blob");
	return data->back();
}

template<typename T>
T& Blob<T>::operator[](size_type i)
{
	check(i, "subscript out of range");
	return (*data)[i];
}

template <typename T>
class BlobPtr
{
public:
	BlobPtr() :curr(0) {}
	BlobPtr(Blob<T>& a, size_t sz = 0) :wptr(a.data), curr(sz) {}

	T& operator*() const;
	BlobPtr& operator++();				//在模板作用域类内模板名可以不指定模板实参T（使用BlobPtr而不是BlobPtr<T>）
	BlobPtr& operator--();
private:
	std::weak_ptr<std::vector<T>> wptr;
	size_t curr;

	std::shared_ptr<std::vector<T>> check(size_t, const std::string&) const;
};

template<typename T>
T& BlobPtr<T>::operator*() const
{
	auto p = check(curr, "dereference past end");
	return (*p)[curr];
}

template<typename T>
BlobPtr<T>& BlobPtr<T>::operator++()		//在模板作用域类外，模板名必须指定模板实参T（返回值使用BlobPtr<T>）
{
	BlobPtr ret = *this;					//在模板作用域类内，模板名可以不指定模板实参T
	++* this;
	return ret;
}
//template，模板显式实例化定义语句。实例化了的话，必须在使用模板之前已经定义或者extern声明，一个程序只会使用定义处实例化的版本，未实例化则编译器在每个使用模板的文件中生成一个实例化版本
//模板类实例化会实例化所有成员，所以所用类型必须要能用于模板所有成员
template class Blob<std::string>;

//////////////////////////////////////////////////////////////////////////
//模板类与友元
//  是否需要前置声明	非模板类是	模板类的一个实例是		模板类相应实例是	模板类所有实例是
//非模板类的友元		否			是					是				否
//模板类的友元			否			是					是				否
template <typename T> class TpltCls1;	//前置声明(1)，将模板的一个特定实例声明为友元时用到
class Cls1								//非模板类
{
	friend class TpltCls1<Cls1>;		//类TpltCls1<Cls1>是Cls1的一个友元，使用前置声明(1)
	template <typename T> friend class TpltCls2;//类TpltCls2所有实例都是Cls1的友元，无需前置声明
	template <typename T> void mem(const T& v) {}//成员模板，不能为虚函数，调用时通过实参自动推断成员模板实参
};

template <typename> class TpltCls1;		//前置声明(2)，将模板的对应实例声明为友元时用到
template <typename T>
class TpltCls3							//模板类
{
	friend class TpltCls1<T>;			//相同模板参数的为友元，需要前置声明(2)
	template <typename X> friend class TpltCls2;//类TpltCls2所有实例都是TpltCls3所有实例的友元，无需前置声明
	friend class Cls2;					//类Cls2是所有TpltCls3实例的友元
};

class Cls2
{};

template <typename T>
class TpltCls1							//模板类
{
	friend T;							//模板类型参数成为友元，即int为TpltCls1<int>的友元等（C++11标准）
public:
	static size_t count() { return ctr; }//通过类访问静态成员时需要特定实例，如TpltCls1<int>::count(); ，通过对象时不需要
	void mem();
private:
	static size_t ctr;
};

template <typename T>
static size_t TpltCls1<T>::ctr = 0;		//类模板的每个实例都独有一个static对象，在外部定义时使用<T>给每个实例独立初始化

template <typename T = int>				//默认模板参数
class TpltCls2							//模板类
{};

template <typename T> using twin = std::pair<T, unsigned>;	//template不能在函数内，可以在类内
template <typename T> using TTplt = TpltCls1<T>;			//使用using可以定义类模板的类型别名
void fun2()
{
	typedef TpltCls1<int> intTplt;			//使用typedef只能使用别名引用 实例化了的类
	twin<std::string> authors;				//authors是一个pair<string, unsigned>
	TpltCls2<> tp2;							//所有模板参数都有默认实参的类，可以只用尖括号表示使用默认类型（此处为int）
}

template <typename T>
class TpltCls1<T&> {};						//类模板部分特例化，在类名后尖括号中使用参数在模板参数列表中按位置对应。部分特例化版本的模板参数列表是原始模板参数列表的子集或特例化版本

template <>
void TpltCls1<int>::mem() {}				//特例化成员而不会整个类，在使用到的时候先使用特例化版本

//////////////////////////////////////////////////////////////////////////
//多重继承、虚继承、虚基类
struct MultiBase1
{
	MultiBase1() = default;
	MultiBase1(const std::string&);
	MultiBase1(std::shared_ptr<int>);
};

struct MultiBase2
{
	MultiBase2() = default;
	MultiBase2(const std::string&);
	MultiBase2(int);
};

struct MultiD : public MultiBase1, public MultiBase2	//多重继承，每个基类都指定访问说明符，若为class则默认前面加private，若为struct默认前面加public
{
	using MultiBase1::MultiBase1;
	using MultiBase2::MultiBase2;
	MultiD() = default;
	MultiD(const std::string&);							//继承了相同参数的构造函数时，需要为该类提供一个相应版本的自定义函数来覆盖
														//自定义拷贝和移动时需要在完整对象上执行，否则出错；合成的操作则会自动对基类部分执行拷贝和移动
};

class vBase {};											//vDeri1和vDeri2的虚基类
class vDeri1 :public virtual vBase {};					//虚继承，使派生类只含有一份虚基类部分
class vDeri2 :virtual public vBase {};					//虚继承，使派生类只含有一份虚基类部分
class Deri :public vDeri1, public vDeri2 {
	Deri() :vBase() {}									//√，虚继承中，构造函数可直接使用虚基类的构造函数来初始化。未显式调用虚基类的构造函数时，虚基类的默认构造函数被调用
														//构造函数调用顺序：首先是最高层的间接虚基类，然后是直接虚基类，然后是非虚基类的间接基类，然后直接非虚基类，最后派生类自己
};
class vDeri3 :public vBase {};
class vDeri4 :public vBase {};
class Deri2 :public vDeri3, public vDeri4 {
	//	Deri2() :vBase() {}									//×，非虚继承中，构造函数不可使用普通间接基类的构造函数
};

//////////////////////////////////////////////////////////////////////////
//嵌套类
//外层类对嵌套类、嵌套类对外层类都没有特殊访问权限（比如嵌套不能访问外层的protected、private，外层也不能访问嵌套的protected、private）
class OuterClass
{
public:
	using line_no = std::vector<int>::size_type;
	class NestedClass;				//访问权限由外部类决定，如定义在public下则可被随处访问
	NestedClass mem();
};
class OuterClass::NestedClass		//嵌套类可以定义在类的外部
{
public:
	line_no fun();					//嵌套类可以使用外层类的成员
	static int static_mem;
};
OuterClass::line_no OuterClass::NestedClass::fun() {}//类外函数定义
int OuterClass::NestedClass::static_mem = 42;//静态成员定义

//////////////////////////////////////////////////////////////////////////
//局部类
int globalint = 1;
void fun3()
{//外层函数对局部类的成员没有访问特权，所以要么将函数声明为局部类的友元，要么将局部类的成员设为public的
	static int si = 0;
	int i;
	enum Loc
	{
		a = 1024, b
	};
	struct Bar						//局部类，所有成员必须完整定义在类的内部（局部类的嵌套类除外）
	{
		Loc locVal;					//√，可以使用函数内的局部类型名
		int barVal;

		void BarFun(Loc l = a)
		{
			//barVal = i;			//×，不能使用函数内的局部变量
			barVal = ::globalint;	//√，可以使用全局对象
			barVal = si;			//√，可以使用函数内的静态对象
			locVal = b;				//√，可以使用函数内的枚举成员
		}
		class nestedClass;			//局部类可以定义嵌套类
	};
	class Bar::nestedClass {};		//局部类的嵌套类可以定义在局部类外，外部函数内
}

//////////////////////////////////////////////////////////////////////////
//位域
//类可以将其非静态数据成员定义成位域，以便向其他程序或硬件传递二进制数据
typedef unsigned int Bit;
class file_a
{
	Bit mode : 2;										//位域声明，类型 名字 冒号 常量表达式，类型必须为整型或枚举型，通常使用无符号类型（带符号型可能会产生奇怪行为）
	Bit modified : 1;									//常量表达式用于指定成员所占的二进制 位数，而不是其中存储的值，比如这句就是modified占1位
	Bit prot_owner : 3;
	Bit prot_group : 3;
	Bit prot_world : 3;
public:
	enum modes { READ = 01, WRITE = 02, EXECUTE = 03 };	//八进制表示
	file_a& open(modes);
	void write();
	void close();
	bool isRead() const { return mode & READ; }			//一般通过（隐式）内联函数检验或设置位域的值
	void setWrite() { mode |= WRITE; }					//一般通过（隐式）内联函数检验或设置位域的值
};
file_a& file_a::open(modes m)
{
	mode |= READ;										//在原来基础上设置READ
	if (m & WRITE)										//检查READ和WRITE打开
		/*...*/;
	return *this;
}
void file_a::write()
{
	modified = 1;										//将modified置位1
	/*...*/
}
void file_a::close()
{
	if (modified)
		/*...*/;
}
