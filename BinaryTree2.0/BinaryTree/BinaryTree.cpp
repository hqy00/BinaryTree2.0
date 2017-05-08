#include<iostream>
#include<queue>
#include<stack>
using namespace std;
#include<assert.h>

/**************************二叉树的基本操作及面试题**************************/

//定义二叉树结点信息
template<typename T>
struct Node
{
	Node(const T& x = T())//构造
		:_value(x)
		,_left(NULL)
		,_right(NULL)
	{}

	Node<T>* _left;		//左孩子
	Node<T>* _right;	//右孩子
	T _value;	//结点的值

};

//定义二叉树
template<typename T>
class BinaryTree
{
public:

	//无参构造
	BinaryTree()
		:_root(NULL)
	{}

	//带参构造
	BinaryTree(const T array[],size_t size,const T& invalued)
	{
		size_t index = 0;
		CreatBinaryTree(_proot,array,size,invalued,index);//创建二叉树

	}

	//拷贝构造
	BinaryTree(const BinaryTree<T>& bt)
	{
		_proot = CopyBinaryTree(bt._proot);
	}

	//赋值运算符重载
	BinaryTree<T>&  operator =(const BinaryTree<T>& bt)
	{
		if(this != &bt)
		{
			DestoryTree(_proot);//摧毁原二叉树
			_proot = CopyBinaryTree(bt._proot);//拷贝一个和bt一模一样的二叉树，将其根结点返回给_proot
		}

		return *this;
	}

	//析构
	~BinaryTree()
	{
		DestoryTree(_proot);//摧毁二叉树
	}

public:
	
	//前序遍历(根左右)
	void Pre()//递归版
	{
		PreOrder(_proot);
	}
	
	void Pre_Nor1()//非递归版1
	{
		//栈不空
		if(NULL == _proot)
		{
			return;
		}

		//让根先入栈
		stack<Node<T>*> s;
		s.push(_proot);

		while(!s.empty())
		{
			//取栈顶元素访问
			Node<T>* pcur = s.top();
			cout<<pcur->_value<<" ";
			s.pop();//删除栈顶元素
			if(pcur->_right)//先将其右孩子入栈
				s.push(pcur->_right);
			if(pcur->_left)//再将左孩子入栈
				s.push(pcur->_left);
		}

	}

	void Pre_Nor2()//非递归版2
	{
		//让根先入栈
		stack<Node<T>*> s;
		s.push(_proot);

		while(!s.empty())
		{
			Node<T>* pcur = s.top();//取栈顶元素
			s.pop();//删除栈顶元素
			while(pcur)
			{
				//访问该结点
				cout<<pcur->_value<<" ";	
				
				//若此结点有右孩子，将其右孩子入栈
				if(pcur->_right)	
					s.push(pcur->_right);

				//访问其左子树
				pcur = pcur->_left;
			}
		}

	}


	//中序遍历（左根右）
	void In()//递归版
	{
		InOrder(_proot);
	}

	void In_Nor()//非递归版
	{
		stack<Node<T>*> s;
		Node<T>* pcur = _proot;

		while(pcur || !s.empty())
		{
			while(pcur)
			{
				s.push(pcur);
				pcur = pcur->_left;
			}

			pcur = s.top();
			cout<<pcur->_value<<" ";
			s.pop();

			pcur = pcur->_right;
		}
	}

	//后序遍历（左右根）
	void Post()
	{
		PostOrder(_proot);
	}

	//层序遍历
	void Level()
	{
		LevelOrder(_proot);
	}
	
	//查找二叉树中值为data的结点
	Node<T>* FindBinaryTree(T data)
	{
		Node<T>* tmp = FindDateNode(_proot,data);
		return tmp;
	}

	//查找某一结点的双亲结点
	Node<T>* FindParent(Node<T>* Pcur)
	{
		Node<T>* tt = FindParentNode(_proot,Pcur);	
		return tt;
	}

	//获取某一结点的左孩子
	Node<T>* FindLeftChild(Node<T>* pcur)
	{
		assert(_proot);
		if(pcur)//检测pcur是否存在
		{
			Node<T>* tmp = NULL;
			if(tmp = pcur->_left)
			{
				return tmp;
			}
		}
		return NULL;
	}
	
	//获取某一结点的右孩子
	Node<T>* FindRightChild(Node<T>* pcur)
	{
		assert(_proot);
		if(pcur)//检测pcur是否存在
		{
			Node<T>* tmp = NULL;
			if(tmp = pcur->_right)
			{
				return tmp;
			}
		}
		return NULL;
	}

	//获得一个树的高度
	size_t Height()
	{
		size_t high = _height(_proot);
		return high;
	}

	//获取树中叶子结点的个数
	size_t LeeNode()
	{
		size_t count = _leenode(_proot);
		return count;
	}

	//获取树中某一层结点的个数
	size_t KLevelNode(size_t k)
	{
		size_t count = _klevelnode(_proot,k);
		return count;
	}

	//二叉树的镜像(反转左右子树)
	//递归版本
	void BinaryMirror()
	{
		 _BinaryMirror(_proot);
	}

	//非递归版本
	void BinaryMirror_Nor()
	{
		 //空树
		 if(NULL == _proot)
		 {
			 return;
		 }

		 queue<Node<T>*> q;
		 q.push(_proot);

		 while(!q.empty())
		 {
			 Node<T>* pcur = q.front();
			 if(pcur->_left)
				 q.push(pcur->_left);
			 if(pcur->_right)
				 q.push(pcur->_right);

			 std::swap(pcur->_left,pcur->_right);
			 q.pop();
		 }
	}

	
private:

	//用先序遍历访问（创建）
	void CreatBinaryTree(Node<T>*& root,const T arr[],size_t size,const size_t invalued,size_t& index)
	{
		if(index < size && arr[index] != invalued)//判断条件的顺序不能变，否则将导致下标访问不合法
		{
			root = new Node<T>(arr[index]);	//创建根结点
			CreatBinaryTree(root->_left,arr,size,invalued,++index);  //创建左子树
			CreatBinaryTree(root->_right,arr,size,invalued,++index); //创建右子树

		}
	}

	//二叉树的拷贝
	Node<T>* CopyBinaryTree(Node<T>* root)
	{
		Node<T>* NewNode = NULL;
		if(root)
		{
			NewNode = new Node<T>(root->_value);	//拷贝根结点

			NewNode->_left = CopyBinaryTree(root->_left);	//拷贝左子树
			NewNode->_right = CopyBinaryTree(root->_right);	//拷贝右子树
		}	
		return NewNode;

	}

	//摧毁二叉树
	void DestoryTree(Node<T>*& root)//用一级指针的引用来对二叉树进行操作
	{
		//利用后序访问的方式对二叉树中的结点进行销毁
		if(root)
		{
			DestoryTree(root->_left);
			DestoryTree(root->_right);
			delete root;
			root = NULL;
			
		}
	}

	//查找二叉树中值为data的结点
	Node<T>* FindDateNode(Node<T>* root,T& data)
	{
		if(root)
		{
			if(data == root->_value)
			{
				return root;
			}

			Node<T>* qq = NULL;
			if(qq = FindDateNode(root->_left,data))
			{
				return qq;
			}

			if(qq = FindDateNode(root->_right,data))
			{
				return qq;
			}
		}
		return NULL;
	}

	
	//查找某一结点的双亲结点
	Node<T>* FindParentNode(Node<T>* root,Node<T>* pcur)
	{
		assert(pcur);//pcur的合法性检测
		if(pcur == root)//传入的结点为根结点 
		{
			return NULL;
		}

		if(root)
		{
			if(pcur == root->_left || pcur == root->_right)
			{
				return root;
			}
				
			Node<T>* qq = NULL;
			if(qq = FindParentNode(root->_left,pcur))
			{
				return qq;
			}

			if(qq = FindParentNode(root->_right,pcur))
			{
				return qq;
			}
		}
		return NULL;
	}

	

	//前序遍历(根左右)
	void PreOrder(Node<T>* root)
	{
		if(root)
		{
			cout<<root->_value <<" ";
			PreOrder(root->_left);
			PreOrder(root->_right);
		}
	}

	//中序遍历（左根右）
	void InOrder(Node<T>* root)
	{
		if(root)
		{
			InOrder(root->_left);
			cout<<root->_value <<" ";
			InOrder(root->_right);
		}
	}

	//后序遍历（左右根）
	void PostOrder(Node<T>* root)
	{
		if(root)
		{
			PostOrder(root->_left);
			PostOrder(root->_right);
			cout<<root->_value <<" ";
		}
	}

	//层序遍历
	void LevelOrder(Node<T>* root)
	{
		if(NULL == root)//树为空返回
		{
			return;
		}
		queue<Node<T>*>	q;
		q.push(root);

		while(!q.empty())
		{
			//选取队头元素
			Node<T>* pcur = q.front();

			//将队头元素的左右子树压入队列
			if(pcur->_left)
				q.push(pcur->_left);
			if(pcur->_right)
				q.push(pcur->_right);
			
			//访问队头元素
			cout<<pcur->_value<<" ";
			
			//删除对头元素
			q.pop();
		}
		
	}

	//获得一个树的高度(深度)
	size_t _height(Node<T>* root)
	{
		//空树
		if(NULL == root)
		{
			return 0;
		}
		//只有一个结点
		if(NULL == root->_left && NULL == root->_right)
		{
			return 1;
		}

		//有多个结点。先求左右子树的高度，然后树的高度 = Max(左子树高度，右子树高度) + 1
		size_t left = _height(root->_left);
		size_t right = _height(root->_right);

		return ((left > right)?left:right) + 1;
	}

	//获取树中叶子结点的个数
	size_t _leenode(Node<T>* root)
	{
		//空树
		if(NULL == root)
		{
			return 0;
		}

		//只有一个结点
		if(NULL == root->_left && NULL == root->_right)
		{
			return 1;
		}

		//有多个结点。将左子树和右子树的叶子结点个数求和后返回
		size_t left = _leenode(root->_left);
		size_t right = _leenode(root->_right);

		return left + right;

	}


	//获取树中第k层结点的个数。将规模缩小，求树的第k层结点数，就是其左子树和右子树的第k-1层结点数之和
	 size_t _klevelnode(Node<T>* root,size_t k)
	 {
		 //断定是否为空树以及k的值是否合法
		 //这个k > _height(root)条件可以去掉，加与不加结果都一样（在纸上画一下），加上会多遍历一次。此处加上是为了看的更直观
		 if(NULL == root || k < 1 ) 
		 {
			 return 0;
		 }

		 //第一层只有一个根结点
		 if(1 == k)
		 {
			 return 1;
		 }

		 //其它层
		 size_t left = _klevelnode(root->_left,k-1);
		 size_t right = _klevelnode(root->_right,k-1);

		 return left + right;
	 }

	 //求二叉树的镜像(递归)
	 void _BinaryMirror(Node<T>* root)
	 {
		 //空树
		 if(NULL != root)
		 {
			 std::swap(root->_left,root->_right);
			 _BinaryMirror(root->_left);
			 _BinaryMirror(root->_right);
		 }

	 }



private:
	Node<T>* _proot;//根结点

};




int main()
{
	//int array1[] = {1,2,4,'#','#','#',3,5,'#','#',6,'#',7};
	int array1[] = {1,2,4,'#','#','#',3,5,'#','#',6};
	int array2[] = {1,2,4,'#',5,'#','#',6};
	BinaryTree<int> t1(array1,sizeof(array1)/sizeof(array1[0]),'#');
	BinaryTree<int> t2(t1);
	BinaryTree<int> t3(array2,sizeof(array2)/sizeof(array2[0]),'#');
	t3 = t1;

	//Node<int>* tmp1 = t1.FindBinaryTree(3);
	//if(tmp1)
	//	cout<<"该结点为"<<tmp1->_value<<endl;
	//else
	//	cout<<"二叉树中没有值为此的结点"<<endl;

	//t1.Pre();
	//cout<<endl;
	//t1.In();
	//cout<<endl;
	//t1.Post();
	//cout<<endl;
	//t1.Level();
	//cout<<endl;


	//Node<int>* tmp2 = t1.FindParent(tmp1);
	//if(tmp2)
	//	cout<<"该结点的双亲结点为"<<tmp2->_value<<endl;
	//else
	//	cout<<"参数结点为根结点,根结点没有双亲"<<endl;


	//Node<int>* tmp3 = t1.FindLeftChild(tmp1);
	//if(tmp3)
	//	cout<<"左孩子为："<<tmp3->_value<<endl;
	//else
	//	cout<<"此结点没有左孩子"<<endl;


	//Node<int>* tmp4 = t1.FindRightChild(tmp1);
	//if(tmp4)
	//	cout<<"右孩子为："<<tmp4->_value<<endl;
	//else
	//	cout<<"此结点没有右孩子"<<endl;


	//cout<<"这个树的高度为"<<t1.Height()<<endl;

	//cout<<"这个树的叶子结点个数为"<<t1.LeeNode()<<endl;

	//cout<<"这一层共有"<<t1.KLevelNode(3)<<"个结点"<<endl;

	//t1.BinaryMirror();
	//t1.BinaryMirror_Nor();
	//t1.Pre_Nor1();
	//t1.Pre_Nor2();
	t1.In_Nor();
	cout<<endl;
	return 0;
}