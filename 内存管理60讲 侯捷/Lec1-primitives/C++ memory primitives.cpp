#include<new>
#include<memory>				// 标准库的allocator 包含在 memory 库里
#include<iostream>
#include<complex>

using namespace std;

int main()
{
	void* p1 = malloc( 512 );
	free( p1 );							//最常见的类似C的内存分配

	complex<int>* p2 = new complex<int>;
	delete p2;

	void* p3 = ::operator new( 512 );
	::operator delete( p3 );

	// 上述是常见的可以参与内存管理的

	int* p4 = allocator<int>().allocate( 5 );
	allocator<int>().deallocate( p4, 5 );
	// 通过对象进行调用，是 non-static 的, allocator<int>() 创建了一个临时对象

	// 上下的情况分配的空间是难以管理，因为前后很难实现完全照应
	// 采取更高效的内存实现方式

	return 0;
}
