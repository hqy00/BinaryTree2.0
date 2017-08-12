#pragma once

//一级空间配置器
#include<iostream>
using namespace std;
#include<memory.h>
#include<stdlib.h>
#include"trace.h"

typedef void(* NewHandleFunPtr) ();//函数指针类型
template<int inst>
class MallocAllocTemplate
{

public:
	static void* Allocate(size_t n)//Allocate
	{
		void* result = malloc(n);
		if(result == NULL)
		{
			result = OOM_Malloc(n);
		}
		return result;
	}

	static void* Reallocate(void* p,size_t /*oldsize*/,size_t newsize)//Reallocate
	{
		void* result = realloc(p,newsize);
		if(result == NULL)
		{
			result = OOM_Realloc(p,newsize);
		}
		return result;
		
	}


	static NewHandleFunPtr SetHandleFun(NewHandleFunPtr f)//设置空间申请不足时的处理函数
	{
		NewHandleFunPtr old = NewHandleFun;
		NewHandleFun = f;
		return old;
	}

	static void* OOM_Malloc(size_t n)//malloc申请空间失败时，用OOM_Malloc重新申请
	{
		NewHandleFunPtr temp;
		for(;;)//不断调用内存不足时的处理方法
		{
			temp  = NewHandleFun;
			if(NULL == temp)
				throw std::bad_alloc();

			(*temp)();
			
			void* result = malloc(n);
			if(result)
				return result;
		}
	}

	static void* OOM_Realloc(void* p,size_t newsize)//realloc申请空间失败时，用OOM_Realloc重新申请
	{
		NewHandleFunPtr temp;
		for(;;)//不断调用内存不足时的处理方法
		{
			temp  = NewHandleFun;
			if(NULL == temp)
				throw std::bad_alloc();

			(*temp)();
			
			void* result = realloc(p,newsize);
			if(result)
				return result;
		}
	}

	static void Deallocate(void* p,size_t/* n*/)//Deallocate
	{
		free(p);
	}
private:
	static NewHandleFunPtr NewHandleFun;//定义处理函数的函数指针
};

template<int inst>
NewHandleFunPtr MallocAllocTemplate<inst>::NewHandleFun = 0;







 //二级空间配置器

enum { ALIGN = 8}; //小区块的上调边界
enum { MAXTYPES = 128}; //小型区块的上限
enum { NFREELISTS = MAXTYPES/ALIGN};//freelists的个数16个

template<bool threads, int inst>
class DefaultAllocTemplate
{
private:
	//RoundUp将bytes上调至8的倍数
	static size_t RoundUp(size_t bytes)
	{ 
		return ( ( bytes + ALIGN -1 ) & (~(ALIGN - 1)) );//用 a & 0x1000,相当于将a的后三位清0，拿到的是8的倍数
	}

private:
	union obj//free_list的节点构造
	{
		union obj* free_list_link;//放的是下一个内存块的地址
		char client_data[1];//如果把空间交给用户，则放的是用户数据
	};

private:
	//16个free_lists
 	static obj* volatile free_list[NFREELISTS];//存放16个自由链表的数组

	//根据字节大小，决定使用第n号free_list，n从0开始算
	static size_t FreeIndex(size_t bytes)
	{
		return ((bytes + ALIGN - 1)/ALIGN-1);
	}

	//返回一个大小为n字节的对象，并可能加入大小为n的若干区块到free_list的对应链表
	static void* Refill(size_t n);

	//配置一大块空间，可容纳nobjs个大小为size字节的区块，nobjs默认为20个，
	//但万一内存池空间不足，获得的区块数可能小于20个
	static char* ChunkAlloc(size_t size,int &nobjs);

	/*chunkalloc函数内变量*/
	static char* startfree;	//内存池起始位置
	static char* endfree;	//内存池结束位置
	static size_t heapsize;	//记录一共申请了多少空间

public:
	static void* Allocate(size_t n);
	static void  Deallocate(void* p,size_t n);
	static void* Reallocate(void* p,size_t oldsize,size_t newsize);
};

//在类外对静态成员变量初始化
template<bool threads,int inst>
char* DefaultAllocTemplate<threads,inst>::startfree = 0;

template<bool threads,int inst>
char* DefaultAllocTemplate<threads,inst>::endfree = 0;

template<bool threads,int inst>
size_t DefaultAllocTemplate<threads,inst>::heapsize = 0;

template<bool threads,int inst>
typename DefaultAllocTemplate<threads,inst>::obj* volatile DefaultAllocTemplate<threads,inst>::free_list[NFREELISTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

 


/***********************************类外对函数进行定义**************************************/

//Allocate中间的过程类似于链表的头删，取到头结点供用户使用，再更新head的位置
template<bool threads,int inst>
void* DefaultAllocTemplate<threads,inst>::Allocate(size_t n)
{
	__TRACE_DEBUG("请求内存:%d\n", n); 
	obj* volatile * my_free_list;
	obj* result;

	//大于128就调用一级空间配置器
	if( n > MAXTYPES )
	{
		return (MallocAllocTemplate<inst>::Allocate(n));
	}

	//寻找16个free_lists中合适的一个
	my_free_list = free_list + FreeIndex(n);
	result = *my_free_list;
	if(result == NULL)
	{
		//没找到适合自己可用的free_lists，准备重新填充一条适合自己的可用的free_lists
		void* r = Refill(RoundUp(n));
		 __TRACE_DEBUG("从自由链表freeList[%d]取节点\n", FreeIndex(n)); 
		return r;
	}
	//调整free_lists
	*my_free_list = result->free_list_link;
	return result;
}



//Deallocate中间的过程类似于链表的头插，再更新head的位置
template<bool threads,int inst>
void DefaultAllocTemplate<threads,inst>::Deallocate(void *p, size_t n)
{
	__TRACE_DEBUG("释放内存(p:%p, n: %u)\n", p, n);
	obj* volatile * my_free_list;
	obj* q = (obj*)p;

	//大于128就调用一级空间配置器
	if( n > MAXTYPES )
	{
		MallocAllocTemplate<inst>::Deallocate(p,n);
		return;
	}

	//寻找16个free_lists中合适的一个
	my_free_list = free_list + FreeIndex(n);
	//调整free_lists,回收区块
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}


//返回一个大小为n字节的对象，并可能加入大小为n字节的若干区块到free_list的对应链表
//假设n已经适当调至8的倍数，缺省情况下默认重新填充20个新区块，但万一内存池空间不足，获得的区块数可能小于20个
template<bool threads,int inst>
void* DefaultAllocTemplate<threads,inst>::Refill(size_t n)
{
	int nobjs = 20;
	/*调用ChunkAlloc函数，尝试取得nobjs个区块作为free_lists的新结点，这里的ChunkAlloc
	参数中的nobjs传的是引用，因为ChunkAlloc函数内部几种情况的不同，会对nobjs进行修改，
	而Refill函数要通过nobjs的值做出对应的处理，所以传的引用*/
	char* chunk = ChunkAlloc(n,nobjs);

	obj* volatile* my_free_list;
	obj* result;
	obj* current_obj;
	obj* next_obj;
	int i;

	//如果只有一个区块，就将它分配给调用者用，free_lists无新结点
	if(1 == nobjs)
		return chunk;

	//否则准备调整free_list，纳入新结点
	my_free_list = free_list + FreeIndex(n);

	//以下是在chunk空间内建立free_list

	result = (obj* ) chunk;//将第一块分配给调用者
	*my_free_list = next_obj = (obj*)(chunk + n);
	
	//循环从1开始，因为第0块给调用者了
	for(i = 1; ; i++)
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);
		if(nobjs - 1 == i)
		{
			current_obj->free_list_link = 0;
			break;
		}
		else
			current_obj->free_list_link = next_obj;
	}
	return result;
}


//假设size已经适当调至8的倍数，缺省情况下默认nobjs重新填充20个新区块，
//但万一内存池空间不足，获得的区块数可能小于20个
template<bool threads,int inst>
char* DefaultAllocTemplate<threads,inst>::ChunkAlloc(size_t size, int &nobjs)
{
	char* result;
	size_t totalbytes = size * nobjs;
	size_t bytesleft = endfree - startfree;//内存池剩余空间

	if(bytesleft > totalbytes)
	{//内存池剩余空间完全满足需求量20个区块 

		__TRACE_DEBUG("内存池有足够的空间分配%d个对象\n",nobjs); 
		result = startfree;
		startfree += totalbytes;
		return result;
	}
	else if(bytesleft >= size)
	{//内存池空间不足，但能满足 1 <= 区块 <  20

		__TRACE_DEBUG("内存池只可以分配%d个对象\n", nobjs);
		nobjs = bytesleft/size;
		totalbytes = size * nobjs;
		result = startfree;
		startfree += totalbytes;
		return result;
	}
	else
	{//内存池剩余空间连一个区块大小都无法提供

		 __TRACE_DEBUG("内存池分配不了1个对象\n", nobjs);  
		size_t bytes_to_get = 2 * totalbytes + RoundUp(heapsize>>4);
		//以下试着让内存池中的零头发挥利用价值
		if(bytesleft > 0)
		{
			//内存池中还有剩余零头空间，首先寻找合适的free_list
			obj* volatile* my_free_list = free_list + FreeIndex(bytesleft);

			//调整free_list,将零头空间编入
			((obj* )startfree)->free_list_link = *my_free_list;
			*my_free_list = (obj* )startfree;

			__TRACE_DEBUG("内存池将剩余的内存放入freeList[%d]中\n", FreeIndex(bytesleft)); 
		}


		//向系统重新申请一块内存池
		startfree = (char*)malloc(bytes_to_get);
		__TRACE_DEBUG("从系统中获取%d内存\n", bytes_to_get);

		if(0 == startfree)
		{
			//系统空间不足，申请空间失败
			int i;
			obj* p;
			obj* volatile* my_free_list;
			//尝试从大于等于size字节的free_list上借一个区块过来
			for(i = size;i < MAXTYPES;i += ALIGN)
			{
				my_free_list = free_list + FreeIndex(i);
				p = *my_free_list;
				if(NULL != p)//说明有可以借用的区块存在
				{
					//调整free_list，拿出可用的区块
					*my_free_list = p->free_list_link;
					startfree = (char* )p;
					endfree = startfree + i;

					//递归调用自己，为了修正nobjs的值
					return (ChunkAlloc(size,nobjs));
					//注意，任何残余零头终将被编入适当的free_list中备用
				}
			}
			 //防止下一句的Malloc抛出异常  
            __TRACE_DEBUG("最后的办法，去一级空间配置器中进行查找\n"); 

			endfree = 0;

			//若系统内存不足，且别的free_list也没有可用的区块能借
			//此时，只能尝试调用一级空间配置器，看它能否尽点力
			startfree = (char* )MallocAllocTemplate<inst>::Allocate(bytes_to_get);
		}

		heapsize += bytes_to_get;
		endfree = startfree + bytes_to_get;
		//递归调用自己，为了修正nobjs的值
		return (ChunkAlloc(size,nobjs));
		
	}


}