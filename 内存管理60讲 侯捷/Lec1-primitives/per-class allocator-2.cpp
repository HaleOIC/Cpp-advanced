#include<string>
#include<memory>
#include<iostream>

using namespace std;

class Airplane {   
private:
  	struct AirplaneRep {
    	unsigned long miles;
    	char type;
  	};
private:
  	union {
    	AirplaneRep rep;  
    	Airplane* next;  
  	};
public:
  	unsigned long getMiles() { return rep.miles; }
  	char getType() { return rep.type; }
  	void set(unsigned long m, char t)
  	{
     	rep.miles = m;
     	rep.type = t;
  	}
public:
  	static void* operator new(size_t size);
  	static void  operator delete(void* deadObject, size_t size);
private:
  	static const int BLOCK_SIZE;
  	static Airplane* headOfFreeList;
};
Airplane* Airplane::headOfFreeList;  
const int Airplane::BLOCK_SIZE = 512;  


void* Airplane::operator new(size_t size){

  	Airplane* p = headOfFreeList;  
  	if (p)
    	headOfFreeList = p->next;
  	else {
    	Airplane* newBlock = static_cast<Airplane*>
       		(::operator new(BLOCK_SIZE * sizeof(Airplane)));
    	for (int i = 1; i < BLOCK_SIZE-1; ++i)
      		newBlock[i].next = &newBlock[i+1];
    	newBlock[BLOCK_SIZE-1].next = 0; 
    	p = newBlock;
    	headOfFreeList = &newBlock[1];
  	}
  	return p;
}


void Airplane::operator delete(void* deadObject,size_t size){
         
  	if (size != sizeof(Airplane)) {   
    	::operator delete(deadObject);
    	return;
  	}
  	Airplane *carcass =
    	static_cast<Airplane*>(deadObject);
  	carcass->next = headOfFreeList;
  	headOfFreeList = carcass;
}
// 

int main(){
	
  	cout << "sizeof( Airplane ) = " << sizeof(Airplane) << endl;    //8

	size_t const N = 100;
	Airplane* p[N];	

   	for (auto i = 0llu; i< N; ++i) p[i] = new Airplane;     
			
  	p[1]->set(1000,'A'); 	
  	p[5]->set(2000,'B');
  	p[9]->set(500000,'C');
  	cout << p[1] << ' ' << p[1]->getType() << ' ' << p[1]->getMiles() << endl;
  	cout << p[5] << ' ' << p[5]->getType() << ' ' << p[5]->getMiles() << endl;
  	cout << p[9] << ' ' << p[9]->getType() << ' ' << p[9]->getMiles() << endl; 
  	
   	for (auto i = 0llu; i< 10; ++i) cout << p[i] << endl; 		 
	 
   	for (auto i = 0llu; i< N; ++i) delete p[i]; 	

   	return 0;
}
// 具体解释见 slides P39 - 40
// 在 effective c++ 中将重新诠释