#include<string>
#include<memory>
#include<iostream>

using namespace std;

int main(){
	cout << "sizeof ( string ) = " << sizeof( string ) << endl;
	int* buf = new int[ sizeof( int ) * 3];
	cout << "buf = " << buf << endl;
	string* pc = new( buf ) string( "this is test the placement new!" );
	cout << "pc = " << pc << " *pc = " << *pc << endl;
	for ( auto i = 0llu; i < 1; ++i ) cout << *( buf++ );
	return 0;
}
/**
 * 对上面程序的理解
 * 倘若你通过new获得一块内存，那么你确实可以通过返回的内存指针访问
 * 但是此时内存中的数值是不确定的，包括着原来使用过没有清零的值
 * 为什么string还是能写入但是连续地址完全不够，肯定是因为
 * string对象保存一个指针，这个指针指向了一串连续的地址，在那个地址里面实现写入
 * 在这个地址写入的不过是string对象指针指向的地址的值罢了
 * 用上述程序输出结构可以验证猜想
 */

/**
 * 虽然有placement new 但是对应的 placement delete 却是不存在的
 */
