Complex* pc = new Complex( 1, 2 );
// 在编译器中眼里将转换成
Complex* pc;
try {
	void* mem = operator new ( sizeof ( Complex ) );
	pc = static_cast< Complex* >( mem );
	pc->Complex::Complex( 1, 2 );			//这条语句无法被执行，只能被编译器实现
} catch ( std::bad_alloc ) {
	// 如果失败，那么就不执行构造函数
}



delete pc;
// 在编译器眼里转换成
pc->~Complex();			//调用析构函数
operator delete( pc ); 		//释放内存

// 但是 operator new 和 operator delete 
// 底层都是执行了 malloc 和 free 
