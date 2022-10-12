/**
 * 当前任务就是对 new 和 delete 重写
 * 完成一个内存池的操作
 */
#include<string>
#include<memory>
#include<iostream>

using namespace std;

class Screen {
public:
	Screen(int x) : i(x) { };
	int get() { return i; }

	void* operator new( size_t );
    void  operator delete( void* );

private:
    Screen* next;
    static Screen* freeStore;
    static const int screenChunk;

private:
    int i;
};
Screen* Screen::freeStore = 0;
const int Screen::screenChunk = 24;

void* Screen::operator new( size_t size ){
	Screen* p = nullptr;
	if ( !freeStore ){
		size_t chunk = screenChunk * size;
		freeStore = p = 
			reinterpret_cast<Screen*>(new char[chunk]);
		for ( ; p != &freeStore[screenChunk - 1]; ++p )
			p->next = p + 1;
		p -> next = 0;
	}
	p = freeStore;
	freeStore = freeStore->next;
	return p;
}				
//本质上实现了内存池转换,并且获取一个内存

void Screen::operator delete( void* p ){
	(static_cast<Screen*>(p)) -> next = freeStore;
	freeStore = (static_cast<Screen*>(p));
	// 回收内存并且插入到 free list 前端
}

int main(){
	cout << "the sizeof Screen is = " << sizeof(Screen) << endl;	
	const size_t N = 100;
	Screen* p[N];
	for ( auto i = 0llu; i < N; ++i )
   	     p[i] = new Screen(i);

	for ( auto i = 0llu; i < 10; ++i ) cout << p[i] << endl;

	for ( auto i = 0llu; i < N; ++i ) delete p[i];    

	return 0;
}

