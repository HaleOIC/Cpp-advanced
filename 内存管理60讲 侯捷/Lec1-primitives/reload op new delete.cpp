/**
 * 为什么重载 operator new 和 operator delete 
 * 因为当使用delete 和 new 的时候 势必调用 operator new 和 operator delete 来实现
 * 相对应的功能
 * 但是 new 和 delete 本身无法重载 从而只能在此动手来实现内存管理接管权的获取
 */
#include<string>
#include<memory>
#include<iostream>

using namespace std;

class Foo{
public: 
	int _id;
	long _data;
	string _str;

	Foo() : _id( 0 ) { cout << "default ctor.this = " << this << " id = " << _id << endl; }
	Foo( int i ) : _id( i ) { cout << "ctor.this = " << this << " id = " << _id << endl; }
	~Foo() { cout << "dtor.this = " << this << " id = " << _id << endl; }

	static void* operator new ( size_t );
	static void operator delete( void* );
	static void* operator new[] ( size_t );
	static void operator delete[] ( void* );

};
void* Foo::operator new( size_t size ){
	Foo* p = (Foo*) malloc( size );
	cout << "Foo::operator new, size = "<< sizeof( p ) << "		" <<
	 	"return: " << p << endl;
	return p;
}
void Foo::operator delete( void* p ){	
	cout << "Foo::operator delete , size = " << sizeof( p ) << "  p =  " << p << endl; 
	free( p );
}
void* Foo::operator new[] ( size_t size ){
	Foo* p = (Foo*) malloc( size );
	cout << "Foo::operator new[], size = "<< sizeof( p ) << "		" <<
	 	"return: " << p << endl;
	return p;
}
void Foo::operator delete[] ( void* p ){
	cout << "Foo::operator delete[], size = " << sizeof( p ) << "  p =  " << p << endl; 
	free( p );
}
int main(){
	cout << "sizeof ( Foo ) = " << sizeof( Foo ) << endl;
	Foo* p = new Foo( 7 );
	delete p;

	Foo* pArr = new Foo[5];
	delete[] pArr;

	return 0;
}