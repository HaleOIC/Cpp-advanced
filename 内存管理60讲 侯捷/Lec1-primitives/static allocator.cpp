class Foo{
private:
	long L;
	string str;
public:
	static allocator myAlloc;
	Foo( long l ) : L( l ) {}
	static void* operator new( size_t size ){
		return myAlloc.allocate(size);
	}
	static void operator delete( void* pdead ){
		myAlloc.deallocate( pdead );
	}
};

class allocator{
private:
	struct obj{
		struct obj* next;
	};
	obj* freeStore = nullptr;
	const int Chunk = 5;
public:
	void* allocate( size_t );
	void deallocate( void* );
};
void* allocator::allocate( size_t size ){
	obj* p = nullptr;
	if ( !freeStore ){
		size_t chunk = Chunk * size;
		freeStore = p = (obj*)malloc( chunk );
		for ( ; p != &freeStore[screenChunk - 1]; ++p )
			p->next = (obj*)((char*)p + size);
		p -> next = nullptr;
	}
	p = freeStore;
	freeStore = freeStore->next;
	return p;
}				

void Screen::operator delete( void* p ){
	(static_cast<obj*>(p)) -> next = freeStore;
	freeStore = (static_cast<obj*>(p));
}