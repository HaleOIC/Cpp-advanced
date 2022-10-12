#include<memory>
#include<iostream>


using namespace std;

class Foo{
public:
	Foo() : _id(0) { cout << " Foo::Foo()" << endl;}
	Foo( int i ) : _id(i) {
		cout << " Foo::Foo(int)" << endl; 
		cout << " throw bad " << endl;
	}

	void* operator new( size_t size ){
		cout << "operator new(size_t size), size= " << size << endl;
		return malloc( size );
	}
	void* operator new( size_t size, void* start ){
		cout << "operator new(size_t size, void* start), size= " << size << "  start= " << start << endl;
		return start;								// 此处对应的是用户已知想要写入的地址
	}
	void* operator new( size_t size, long extra ){
	  	cout << "operator new(size_t size, long extra)  " << size << ' ' << extra << endl;
		return malloc( size + extra );
	}
	void* operator new( size_t size, long extra, char init ){
	  	cout << "operator new(size_t size, long extra, char init)  " << size << ' ' << extra << ' ' << init << endl;
		return malloc( size + extra );
	}

	//不管如何实现 placement new 都必须将 size_t 作为第一参数不然将编译器报错

	void operator delete(void*,size_t)
  	{ cout << "operator delete(void*,size_t)  " << endl;  }

  	void operator delete(void*,void*)
  	{ cout << "operator delete(void*,void*)  " << endl;  }

  	void operator delete(void*,long)
  	{ cout << "operator delete(void*,long)  " << endl;  }

  	void operator delete(void*,long,char)
  	{ cout << "operator delete(void*,long,char)  " << endl; }

  	/**
  	 * 可以不对应
  	 * 但是不能没有相对应的函数
  	 */

private:
	int _id;
};

int main(){
	Foo start;
	Foo* p1 = new Foo;
	Foo* p2 = new( &start ) Foo;
	Foo* p3 = new( 100 ) Foo;
	Foo* p4 = new( 100, 'a' ) Foo;
	Foo* p5 = new( &start ) Foo(1);
	delete p1;
	delete p2;
	delete p3;
	delete p4;
	delete p5;
	return 0;
}