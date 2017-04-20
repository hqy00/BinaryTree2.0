//#include<iostream>
//using namespace std;

//用递归方式完成以下程序

//求前N个自然数之和
//int N_Sum(int n)
//{
//	if(n == 0)
//	{
//		return 0;
//	}
//	else
//	{
//		return N_Sum(n-1) + n;
//	}
//
//}
//int main()
//{
//	cout<<N_Sum(5)<<endl;
//	return 0;
//}


//求N的阶乘
//int fac(int n)
//{
//	if(n == 1)
//	{
//		return 1;
//	}
//	else
//	{
//		return fac(n-1) * n;
//	}
//
//}
//int main()
//{
//	cout<<fac(5)<<endl;
//	return 0;
//}


//struct Node{
//
//	Node(int value)
//		:data(value)
//	{}
//
//	Node* next;
//	int data;
//};
//逆序打印单链表
//void  PrintReverse(Node *&phead)
//{
//	if(phead)
//	{
//		PrintReverse(phead->next);
//		cout<<phead->data<<endl;
//	}
//}
//
//int main()
//{
//	Node(1),Node(2),Node(3),Node(4);
//	Node *a = &Node(1);
//	Node *b = &Node(2);
//	Node *c = &Node(3);
//	Node *d = &Node(4);
//
//	a->next  = b;
//	b->next  = c;
//	c->next  = d;
//	d->next  = NULL;
//
//	PrintReverse(&a);
//
//}

//逆序销毁单链表
//void DestroyReserve(Node *&phead)
//{
//	if(phead)
//	{
//		DestroyReserve(phead->next);
//		delete phead;//注意这里用delete删除，就必须用new来给结点开辟空间
//		phead  = NULL;
//	}
//
//}
//
//int main()
//{
//	Node *a = new Node(1);
//	Node *b = new Node(2);
//	Node *c = new Node(3);
//	Node *d = new Node(4);
//
//	a->next  = b;
//	b->next  = c;
//	c->next  = d;
//	d->next  = NULL;
//
//	DestroyReserve(&a);
//}


//在单链表中逆序查找某个值为data的结点
//Node* Find(Node* phead,int data)
//{
//	if(phead)
//	{
//		Node* res = Find(phead->next,data);//res的作用是把找到的结点返回去
//		if(res)
//		{
//			return res;
//		}
//		else
//		{
//			if(data == phead->data)
//			{
//				return phead;
//			}
//		}
//	}
//
//	return NULL;
//}
//
//int main()
//{
//	Node *a = new Node(1);
//	Node *b = new Node(2);
//	Node *c = new Node(3);
//	Node *d = new Node(4);
//
//	a->next  = b;
//	b->next  = c;
//	c->next  = d;
//	d->next  = NULL;
//
//	if(Find(a,2))
//	{
//		cout<<"找到了\n"<<endl;
//	}
//}

//二分查找递归形式
//int BinarySearch(int* array,int left,int right,int data)
//{
//	if(left > right)//合法性检查
//	{
//		return;
//	}
//	int mid = left + (right - left)>>1;
//	if(data == array[mid])
//	{
//		return array[mid];
//	}
//	else if(data < array[mid])
//	{
//		return BinarySearch(array,left,mid,data);
//	}
//	else
//	{
//		return BinarySearch(array,mid+1,right,data);
//	}
//
//	return -1;
//}

//逆序打印数组中的元素
//void PrintfReverseArray(int* arr,int size)
//{
//	if(size)
//	{
//		cout<<arr[size-1]<<" ";//数组的逆向打印是先打印再递归，是因为数组一开始就能找到最后一个元素，但链表不行
//		PrintfReverseArray(arr,size-1);
//	}
//
//}
//
//int main()
//{
//	int arr[] = {1,2,3,4,5};
//	PrintfReverseArray(arr,5);
//	return 0;
//}


//判断一个字符串是否为回文
//普通版
//bool Ispalindereme(char* str,size_t size)
//{
//	int begin = 0;
//	int end = size -1;
//	while(begin < end)//对于奇数个回文字符，跳出循环的条件是bagin = end;对于偶数个回文字符，跳出循环的条件是bagin > end;
//	{
//		if(str[begin] != str[end])
//		{
//			return false;
//		}
//		begin++;
//		end--;
//	}
//
//	return true;
//}

//递归版
//bool Ispalindereme(char* str,size_t size)
//{
//	if(size <= 1)
//		return true;
//	if(str[0] != str[size-1])//优化了，先判断首尾是否一致
//	{
//		return false;
//	}
//	return Ispalindereme(str+1,size-2);
//	return true;
//
//}

//全排列Perm，并给出其时间复杂度
//void Perm(int* arr,int size,int N)
//{
//	if(size == N)
//	{
//		for(size_t idx;idx < size;idx++)
//		{
//			cout<< arr[idx]<<"";
//		}
//		
//		cout<<endl;
//	}
//	else
//	{
//		for(size_t idx = N;idx < size;idx++)
//		{
//			std::swap(arr[i],arr[N]);
//			Perm(arr,size,N+1);
//			std::swap(arr[i],arr[N]);
//		}
//	}
//}
//
//其时间复杂度为N！
//




