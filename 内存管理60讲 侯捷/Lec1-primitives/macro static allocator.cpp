#define DECLARE_POOL_ALLOC() \
public: \
    void* operator new(size_t size) { return myAlloc.allocate(size); } \
    void operator delete(void* p) { myAlloc.deallocate(p, 0); } \
protected: \
    static allocator myAlloc; 

#define IMPLEMENT_POOL_ALLOC(class_name) \
allocator class_name::myAlloc; 

class Foo{
	DECLARE_POOL_ALLOC()
private:
	long L;
	string str;
public:
	Foo( long l ) : L( l ) {}
};
IMPLEMENT_POOL_ALLOC(Foo) 

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