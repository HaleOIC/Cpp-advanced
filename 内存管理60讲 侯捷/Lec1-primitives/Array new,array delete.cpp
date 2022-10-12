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

int main(){
	A* pA = new A[5];			// array new 指针之后， 对于新内存进行array delete 
								// 如果不用delete[] 将有明显的报错 程序直接崩溃
	delete[] pA;

	string* psa = new string[3];
	/* 
		delete psa;
		 上述语句一定造成内存泄露，因为string 本身由一个指针指向一块内存完成
		 此时delete psa 的确会得到一个 地址 并且回收掉地址指向的对应的区域，但是
		 地址内的string指针指向的地方并没有被收回，这就造成了内存的泄露
	*/
	delete[] psa;


	const size_t size = 3;

	A* buf = new A[ size ];
	A* tmp = buf;

	cout << " buf = " << buf << " tmp = " << tmp << endl;


	for ( auto i = 0LLu; i < size; ++i ) new( tmp++ )A( i ); 
	// 使用了placement new 方式完成了间接对构造函数的调用

	delete[] buf; 	
	// 观察输出，显然逆序

	return 0;
}