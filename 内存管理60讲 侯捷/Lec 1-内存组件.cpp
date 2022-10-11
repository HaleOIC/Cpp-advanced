#include<iostream>
#include<memory>

using namespace std;

class A{
public:
	int id = 0;

	A() : id( 0 ) { cout << "default ctor.this= " << this << " id = " << id << endl;  }
	A( int i ) : id( i ) { cout << "ctor.this=" << this << " id = " << id << endl;  }
	~A() { cout << "dtor.this = " << this << " id = " << id << endl; }
};
int main(){
	constexpr int size = 3;
	A* buf = new A[size];
	A* tmp = buf;

	cout << "buf = " << buf << " tmp = " << tmp << endl;

	for ( auto i = 0; i < size; ++i )
		new(tmp++) A(i);							//无法直接调用构造函数但是可以曲线救国实现在指针指向处调用构造函数

	cout  << " buf = " << buf << " tmp = " << tmp << endl; 	// 次序逆反

	delete[] buf;

	return 0; 
}