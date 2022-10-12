#include<string>
#include<memory>
#include<iostream>

using namespace std;


class A {
public:
	int id;

	A() : id(0)      { cout << "default ctor. this="  << this << " id=" << id << endl;  }
	A(int i) : id(i) { cout << "ctor. this="  << this << " id=" << id << endl;  }
  	~A()             { cout << "dtor. this="  << this << " id=" << id << endl;  }
};

int main()
{
	string* pstr = new string;
	cout << "str = " << *pstr << endl;
	/*
		pstr->string::string("hale");
		上面这句报错
			class std::__cxx11::basic_string<char>' has no member named 'string'
		说明无法直接通过指针调用构造函数
	*/
	A* pA = new A( 1 ); // 说明new时候可以使用构造函数赋予初值

	delete pstr;
	delete pA;
	return 0;
}