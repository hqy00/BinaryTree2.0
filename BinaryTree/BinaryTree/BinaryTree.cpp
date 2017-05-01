#include<iostream>
#include<queue>
using namespace std;
#include<assert.h>

/**************************递归实现二叉树**************************/

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
	void Pre()
	{
		PreOrder(_proot);
	}

	//中序遍历（左根右）
	void In()
	{
		InOrder(_proot);
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
		Node<T>* tmp;
		tmp = FindDateNode(_proot,data);
		if(NULL == tmp)
		{
			cout<<"二叉树中没有值为此的结点"<<endl;
			exit(1);
		}
		return tmp;
	}

	//查找某一结点的双亲结点
	Node<T>* FindParent(Node<T>* Pcur)
	{
		Node<T>* tt;
		tt = FindParentNode(_proot,Pcur);	
		if(NULL == tt)
		{
			cout<<"参数结点为根结点,根结点没有双亲"<<endl;
			exit(1);
		}
		return tt;
	}

	//获取某一结点的左孩子
	Node<T>* FindLeftChild(Node<T>* Pcur)
	{
		Node<T>* tt;
		tt = FindLeftNode(_proot,Pcur);	
		if(NULL == tt)
		{
			cout<<"参数结点的左孩子不存在"<<endl;
			exit(1);
		}
		return tt;
	}
	
	//获取某一结点的右孩子
	Node<T>* FindRightChild(Node<T>* Pcur)
	{
		Node<T>* tt;
		tt = FindRightNode(_proot,Pcur);	
		if(NULL == tt)
		{
			cout<<"参数结点的右孩子不存在"<<endl;
			exit(1);
		}
		return tt;

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

			Node<T>* pp = NULL;
			if(pp = FindDateNode(root->_right,data))
			{
				return pp;
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

			Node<T>* pp = NULL;
			if(pp = FindParentNode(root->_right,pcur))
			{
				return pp;
			}
		}
		return NULL;
	}

	
	//获取某一结点的左孩子
	Node<T>* FindLeftNode(Node<T>* root,Node<T>* pcur)
	{
		assert(pcur);//pcur的合法性检测
		if(root)
		{
			if(pcur == root && NULL != root->_left)
			{	
				return root->_left;
			}
				
			Node<T>* qq = NULL;
			if(qq = FindLeftNode(root->_left,pcur))
			{
				return qq;
			}

			Node<T>* pp = NULL;
			if(pp = FindLeftNode(root->_right,pcur))
			{
				return pp;
			}
		}
		return NULL;

	}


	//获取某一结点的右孩子
	Node<T>* FindRightNode(Node<T>* root,Node<T>* pcur)
	{
		assert(pcur);//pcur的合法性检测
		if(root)
		{
			if(pcur == root && NULL != root->_right)
			{
				return root->_right;
			}
				
			Node<T>* qq = NULL;
			if(qq = FindRightNode(root->_left,pcur))
			{
				return qq;
			}

			Node<T>* pp = NULL;
			if(pp = FindRightNode(root->_right,pcur))
			{
				return pp;
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

private:
	Node<T>* _proot;//根结点

};




int main()
{
	int array1[] = {1,2,4,'#','#','#',3,5,'#','#',6};
	int array2[] = {1,2,4,'#',5,'#','#',6};
	BinaryTree<int> t1(array1,sizeof(array1)/sizeof(array1[0]),'#');
	BinaryTree<int> t2(t1);
	BinaryTree<int> t3(array2,sizeof(array2)/sizeof(array2[0]),'#');
	t3 = t1;

	Node<int>* tmp1;
	tmp1 = t1.FindBinaryTree(2);
	cout<<tmp1->_value<<endl;
	t1.Pre();
	cout<<endl;
	t1.In();
	cout<<endl;
	t1.Post();
	cout<<endl;
	t1.Level();
	cout<<endl;

	Node<int>* tmp2;
	tmp2 = t1.FindParent(tmp1);
	cout<<tmp2->_value<<endl;

	Node<int>* tmp3;
	tmp3 = t1.FindLeftChild(tmp1);
	cout<<tmp3->_value<<endl;

	Node<int>* tmp4;
	tmp4 = t1.FindRightChild(tmp1);
	cout<<tmp4->_value<<endl;

	return 0;
}