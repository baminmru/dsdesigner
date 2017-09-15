#include "bamilib.h"
//#include <alloc.h>

/*
bool First = true;
long MyNewCnt=0;

void * operator new(size_t size){
	if(First){
		First=false;
		MyNewCnt=0;
	}

	if(!size){
		Debug("\nError: ZERO block !!!\n");
		return 0;
	}else{

		void * ptr;
		ptr=0;
		ptr=malloc(size);
		size_t tmp;
		tmp=((size_t *)ptr)[-1];
		MyNewCnt+=tmp;
		//Debug("%ld (%ld) ",size,MyNewCnt);
		if(!ptr){
			Debug("\nError: Can't allocate %ld bytes of memory !!!\n" ,size);
			exit(0);
		}

		return ptr;
	}
};

*/
/*
void operator delete(void * ptr){
	size_t tmp;
	if(ptr){

		if(!First){
			tmp=((size_t *)ptr)[-1];
			MyNewCnt-=tmp;
			if(MyNewCnt<0){
				Debug("\nError: disbalansed delete !!!\n");
				_exit(0);
			}
			//Debug("-- %ld (%ld)\n",tmp,MyNewCnt);
		}

		free(ptr);
		ptr=0;
	} else
		Debug("Error: Delete for zero pointer !!!\n");


	if(heapcheck()==_HEAPCORRUPT){
		Debug("Heap CORRAPTED !!!\n");
		exit(0);
	}

};
*/
/*
long MemoryCheck(){
	if( MyNewCnt==0){
		Debug("\nAll new calls balansed!\n");
	}
	if( MyNewCnt>0){
		Debug("\n%ld bytes of memory lost !\n",MyNewCnt);
	}

	return MyNewCnt;
};

*/

long MemoryCheck(){
	/*if(heapcheck()==_HEAPCORRUPT){
		Debug("Heap CORRAPTED !!!\n");
		exit(0);
	}
	*/
	return 0;
};


