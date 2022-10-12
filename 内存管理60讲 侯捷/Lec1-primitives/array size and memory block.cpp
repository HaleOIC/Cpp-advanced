#include<string>
#include<memory>
#include<iostream>

using namespace std;

class Demo{
public:
	int value;
	int val1;
	int val2;
	char c;

	Demo() : value( 0 ), val1( 0 ), val2( 0 ), c( ' ' ) {};
	Demo( int val ) : value( val ) {};
	~Demo() { cout << "dtor.this = " << this << endl; }
};
// 本来长度应该是 13 byte 但是 自动对齐给的是 16


int main(){

	int* pi = new int[10]; 	// sizeof( pi ) : 4
	cout << "sizeof( pi ) = " << sizeof( pi ) << endl;
	delete pi;

	int ia[10];
	cout << sizeof( ia ) << endl;

	cout << "sizeof ( Demo ) = " << sizeof( Demo ) << endl;

	auto* pDemo = new Demo[3];
	cout << "sizeof ( pDemo ) = " << sizeof( pDemo ) << endl; 
	// 指针大小测试就是 8 byte 跟指向对象类型无关

	delete[] pDemo;
	/*
		delete pDemo;
		则会崩溃
	*/
	return 0;
}