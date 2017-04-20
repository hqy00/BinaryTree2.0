//*********************实现Stack的基本操作***************************
//  要求：在进行扩容时，先创建新空间，再把原空间的数据拷贝到新空间，再释放旧空间，最后指向新空间。
//	在把原空间的数据拷贝到新空间时，我们有两种种方法：1>使用memcpy拷贝元素；2>使用for循环赋值
//  memcpy拷贝的优点是效率高，缺点是它是浅拷贝。memcpy函数的功能是从源src所指的内存地址的起始位置开始拷贝n个字节到目标dest所指的内存地址的起始位置中
//	for循环拷贝的优点是不管深浅拷贝都不会出错，缺点是效率低
//	为了拷贝时，既能效率高，又能顾忌深浅拷贝，所以我们引入类型萃取。
//	类型萃取是如果栈中保存的是POD（内置数据类型)，内置数据类型不用考虑深浅拷贝，那么直接使用memcpy拷贝元素
//  若不是POD（内置数据类型),则要考虑深浅拷贝，此时使用for循环进行拷贝
//#include<iostream>
//using namespace std;
//#include<assert.h>
//#include "date.h"
//
//
//struct _TrueType
//{
//	bool Get()
//	{
//		return true;
//	}
//};
//
//struct _FalseType
//{
//	bool Get()
//	{
//		return false;
//	}
//};
//
//template<class TP>
//struct TypeTraits	//非内置类型调用模板类
//{
//	typedef _FalseType _IsPODType;
//};
//
////*************内置类型调用下面对应的模板类的特化*****************
//template<>
//struct TypeTraits<int>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<unsigned int>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<char>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<unsigned char>
//{
//	typedef _TrueType _IsPODType;
//};
//
//
//template<>
//struct TypeTraits<short>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<unsigned short>
//{
//	typedef _TrueType _IsPODType;
//};
//
//
//template<>
//struct TypeTraits<long>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<unsigned long>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<double>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<long double>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<float>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<long long>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<unsigned long long>
//{
//	typedef _TrueType _IsPODType;
//};
//
//template<>
//struct TypeTraits<bool>
//{
//	typedef _TrueType _IsPODType;
//};
//
////*****************************************************************
//
////栈的实现
//template<class T>
//class Stack
//{
//public:
//	Stack(size_t capacity = 10)//构造
//		:_size(0)
//		,_capacity(capacity)
//		,_pData(new T[capacity])
//	{}
//
//    Stack(const Stack<T>& s)//拷贝构造-->先创建一个跟s一模一样的空间--->再将s的数据拷贝过来（用萃取）
//		:_size(s._size)
//		,_capacity(s._capacity)
//		,_pData(new T[s._capacity])
//	{
//		cout<<"Type :"<<typeid(T).name<<endl;//打印栈内存放数据的类型
//		if(TypeTraits<T>::_IsPODType().Get())
//		{
//			memcpy(_pData,s._pData,s._size * sizeof(T));
//		}
//		else
//		{
//			for(size_t idx = 0; idx < s._size;idx++)
//			{
//				_pData[idx] = s._pData[idx];
//			}
//		}
//	}
//
//    Stack<T>& operator=(const Stack<T>& s)//赋值运算符重载
//	{
//		cout<<"Type :"<<typeid(T).name<<endl;
//
//		if(this != &s)
//		{
//			if(NULL != _pData)
//			{
//				delete[] _pData;//销毁原空间
//				_pData = new T[s._capacity];//给它重新开辟和s一样大小的新空间
//				_capacity = s._capacity;
//			}
//		
//			if(TypeTraits<T>::_IsPODType().Get())//利用类型萃取，把s里的数据拷贝到新空间
//			{
//				memcpy(_pData,s._pData,s._size * sizeof(T));
//			}
//			else
//			{
//				for(size_t idx = 0; idx < s._size;idx++)
//				{
//					_pData[idx] = s._pData[idx];
//				}
//			}
//
//			_size = s._size;
//
//		}
//
//		return *this;
//	}
//
//	void Push(const T& num)//压栈
//	{
//		_CheckCapacity();//先检测栈的容量
//		_pData[_size] = num;
//		_size++;
//
//	}
//
//	void Pop()//出栈
//	{
//		assert(0 != _size);
//		_size--;
//
//	}
//	size_t Size()const//栈内有效元素个数
//	{
//		return _size;
//	}
//
//	size_t Capacity()const//栈内容量大小
//	{
//		return _capacity;
//	}
//
//	T& Top()//栈顶元素
//	{
//		return _pData[_size-1];
//	}
//
//	const T& Top()const
//	{
//		return _pData[size-1];
//
//	}
//
//	bool Empty()const//判断栈是否为空
//	{
//		return 0 == _size;
//	}
//	
//	~Stack()//析构
//	{
//		if(_pData)
//		{
//			delete[] _pData;
//			_pData = NULL;
//			_size = 0;
//			_capacity = 0;
//
//		}
//
//	}
//
//	void Printf()
//	{
//		for(size_t idx = 0; idx < _size;idx++)
//		{
//			cout<<_pData[idx]<<" ";
//		}
//		cout<<endl;
//	}
//
//
//private:
//	T* _pData;		//指向栈的指针
//	size_t _capacity;//栈的容量
//	size_t _size;	//栈内现有元素个数
//
//
//private:
//	void _CheckCapacity()//检查容量，容量不够时增容
//	{
//		if(_size == _capacity)//因为栈的结构决定了，它只能对栈顶这一个元素进行操作，所以增容出现的情况只能是_size == _capacity
//		{
//			// 申请空间
//			T* temp = new T[_capacity*2+3];//加了一个数，是为了防止构造时传进的_capacity是0
//			
//			// 拷贝元素
//			if(TypeTraits<T>::_IsPODType().Get())//利用类型萃取，把s里的数据拷贝到新空间
//			{
//				memcpy(temp,_pData,_size * sizeof(T));
//			}
//			else
//			{
//				for(size_t idx = 0; idx < _size;idx++)
//				{
//					temp[idx] = _pData[idx];
//				}
//			}
//
//			// 释放旧空间
//			delete[] _pData;
//
//			// 指向新空间
//			_pData = temp;
//			_size = _size;
//			_capacity = _capacity * 2 + 3;
//
//		}
//	}
//
//
//};


//int main()
//{
//	Date d1(2017,3,3);
//	Date d2(2017,5,1);
//	Date d3(2017,4,25);
//	Stack<Date> s(1);//非内置类型
//
//	s.Push(d1);
//	s.Push(d2);
//	s.Push(d3);
//
//	Stack<int> s1;//内置类型
//	Stack<int> s2(0);
//	Stack<int> s3(3);

	//s1.Push(1);
	//s1.Push(2);
	//s1.Push(3);
	//s1.Push(4);
	//s1.Printf();
	//s1.Pop();
	//s1.Printf();
	//cout<<s1.Top()<<endl;
	//cout<<s1.Size()<<endl;
	//cout<<s1.Capacity()<<endl;

	//s2.Push(1);
	//s2.Push(2);
	//s2.Push(3);
	//s2.Push(4);
	//s2.Printf();
	//s2.Pop();
	//s2.Printf();
	//cout<<s2.Top()<<endl;
	//cout<<s2.Size()<<endl;
	//cout<<s2.Capacity()<<endl;

//	return 0;
//}


//***********************求逆波兰表达式（后缀表达式）的运算结果************************
//将中缀表达式转化为逆波兰表达式（后缀表达式）后，现在计算逆波兰表达式的值。
//#include<iostream>
//#include<stack>
//using namespace std;
//#include<assert.h>
//
//enum OPERATOR //定义枚举类型
//{
//	DATA,
//	ADD,
//	SUB,
//	MUL,
//	DIV
//};
//
//struct Cell //定义元素
//{
//	OPERATOR _operator;//定义枚举类型的变量
//	int _data;//数值
//};
//
//
//int CalcRpn(Cell cell[],size_t size)
//{
//	stack<int> s;//定义栈s
//	for(size_t idx = 0;idx < size; idx++)
//	{
//		if(cell[idx]._operator == DATA)//如果是数字就入栈
//		{
//			s.push(cell[idx]._data);
//		}
//		else
//		{
//			int right = s.top();//让先出栈的数据，作为双目操作符的右操作数
//			s.pop();
//			int left = s.top();//让后出栈的数据，作为双目操作符的左操作数
//			s.pop();
//
//			switch(cell[idx]._operator)
//			{
//			case ADD:
//				s.push(left + right);
//				break;
//
//			case SUB:
//				s.push(left - right);
//				break;
//
//			case MUL:
//				s.push(left * right);
//				break;
//
//			case DIV:
//				if(right == 0)//除法的除数不能为0
//				{
//					return -1;
//				}
//				s.push(left / right);
//				break;
//
//			default:
//				cout<<"计算出错，表达式中有无法识别的操作符"<<endl;
//				exit(1);
//
//
//			}
//		}
//	}
//	return s.top();
//
//}
//int main()
//{
//	// 12 3 4 + * 6 - 8 2 / +  把逆波兰表达式先存到一个数组中
//	Cell cell[] = {{DATA,12},{DATA,3},{DATA,4},{ADD,0},{MUL,0},
//	                 {DATA,6},{SUB,0},{DATA,8},{DATA,2},{DIV,0},{ADD,0}};
//
//	int num = CalcRpn(cell,sizeof(cell)/sizeof(cell[0]));
//	if(-1 == num)
//	{
//		cout<<"表达式不合法，除法运算中的除数为0"<<endl;
//	}
//	else
//	{
//		cout<<num<<endl;
//	}
//
//
//	return 0;
//
//}


//***********************括号匹配问题********************************
//左右括号的匹配问题，可以用栈来很好的解决。 
//1. 遇到左括号，就入栈。 
//2. 遇到右括号，判断此时栈是否为空。 
//* 栈为空，说明右括号多。 
//* 不为空，判断栈顶元素与该右括号是否对应，不对应，说明不匹配。如{(};若匹配则出栈。 
//3. 最后，遍历整个Str后,若栈为空，说明完全匹配，若栈不空，说明左括号多。

//#include<iostream>
//#include<stack>
//using namespace std;
//#include<assert.h>
//
//void CheckMatch(char* p)
//{
//	stack<char> s;//定义一个栈
//
//	while(*p)
//	{
//		if(*p =='('|| *p =='['|| *p == '{')//遇到左括号，就入栈。
//		{
//			s.push(*p);
//			p++;
//		}
//		else if(*p ==')'|| *p ==']'|| *p == '}')//遇到右括号，判断此时栈是否为空
//		{
//			if(s.empty())//栈为空，说明右括号多
//			{
//				cout<<"右括号比左括号多"<<endl;
//				exit(1);
//			}
//			else//不为空，判断栈顶元素与该右括号是否对应
//			{
//				if(((s.top()=='(')&&(*p==')')) || ((s.top()=='[')&&(*p==']')) || ((s.top()=='{')&&(*p=='}')))
//				{
//					s.pop();//匹配则出栈
//					p++;
//				}
//				else//不对应，说明不匹配
//				{
//					cout<<"括号不匹配"<<endl;
//					exit(1);
//				}
//
//			}
//		}
//		else//遇到其他字符
//		{
//			p++;
//		}
//	}
//
//	//能出循环证明字符串遍历完毕。此时栈内只能有两种情况：1>不为空，且只剩下左括号了，这种情况匹配失败;2>为空，匹配成功
//	if(s.empty())
//	{
//		cout<<"匹配成功"<<endl;
//	}
//	else
//	{
//		cout<<"左括号比右括号多"<<endl;
//	}
//
//}
//int main()
//{
//	//char str[] = "(())abc{[(])}";//括号不匹配
//	//char str[] = "(()))abc{[]}";//右括号多于左括号
//	//char str[] = "(()()abc{[]}";//左括号多于右括号
//	char str[] = "(())abc{[]()}";//匹配成功
//	CheckMatch(str);
//	return 0;
//}



