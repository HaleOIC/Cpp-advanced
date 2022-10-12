#include<new>
#include<iostream>
#include<cassert>

using namespace std;

void noMoreMemory(){
	cerr << "out of memory"; 
	abort();
}

int main(){
	set_new_handler( noMoreMemory );
	int *p = new int [100000000000];
	assert( p );

	p = new int[1000000000000000];
	assert( p );
	return 0;
}