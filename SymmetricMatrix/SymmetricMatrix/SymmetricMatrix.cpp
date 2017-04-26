//实现对称矩阵以及压缩存储
/*
下三角有i>=j,上三角i<j   (i,j下标都从0开始)
对于对称矩阵有a(i,j)=a(j,i)，即行列互换，代入上式即可得
那么很显然，我们可以将这样一个对称矩阵的下三角元素（上三角同样可行）压缩存储到一个一元数组中，
从而节省较多的空间，接下来我们分析一下如何存储。 
分析：第一行有一个元素，第二行有两个元素……第i行有i个元素…… 
若对称矩阵是n*n的，则我们需要一个长度为1+2+…+n的数组来依次存储每行元素，根据等差数列前n项和公式得：
S(n) = n*(n+1)/2
上式表示，计算前n行（包括第n行）的元素个数

但代入下标时，由于i,j下标是从0开始的，
所以，S(i) = i * (i+1)/2
表示，计算前i行（不包括第i行）的元素个数

再看本行(第i行)，本元素前有j个元素

所以代入下标以后，对称矩阵里的元素在一维数组中的位置存在一一对应的关系：
下三角存储 i >= j,  SymmetricMatrix[i][j] == arry[i*(i+1)/2+j]
上三角存储 i < j,   SymmetricMatrix[i][j] == arry[j*(j+1)/2+i]

下面的程序我们只保存下三角。
*/

#include<iostream>
using namespace std;

template<class T>
class SymmetricMatrix
{
public:
	SymmetricMatrix(int* a, size_t n)//矩阵的构造
		:_pData(new T[n*(n+1)/2])//为压缩存储的一维数组申请空间
		,_n(n)
	{
		size_t count = 0;
		for(size_t i=0; i<_n; i++)
		{
			for(size_t j=0; j < _n; j++)
			{
				if(i >= j)//在下三角
				{
					_pData[count] = a[i*n +j];
					count++;
				}
				else//优化，只要i<j(到上三角区域)就把整行跳过
				{
					break;
				}
			} 
		}
	}

	T& Access(int rows, int cols)
	{
		if(rows < cols)//到上三角区域
		{
			std::swap(rows,cols);//转成下三角
		}
		
		return  _pData[rows*(rows+1)/2 +cols];

	}
        
	const T& Access(int rows, int cols)const
	{
		if(rows < cols)
		{
			std::swap(rows,cols);
		}
		
		return  _pData[rows*(rows+1)/2 +cols];
	}


	~SymmetricMatrix()//析构
	{
		if(_pData)
		{
			delete[] _pData;
			_pData = NULL;
			_n = 0;
		}
	}

private:
	template<class TT>//因为是友元，不是类的成员函数，不能用类的模板参数，要用只能自己重新定义
	friend ostream& operator<<(ostream& out,SymmetricMatrix<TT>& sm)//‘<<’运算符重载
	{
		for(size_t i=0; i< sm._n; i++)
		{
			for(size_t j=0; j < sm._n; j++)
			{
				out<<sm.Access(i,j)<<" ";

			}
			out<<endl;
		}

		return out;
	}

private:
	T* _pData;	//压缩存储的一维数组
	size_t _n;	//n*n的矩阵 
};


int main()
{
	int a[5][5] = {
		{ 0, 1, 2, 3, 4 },  
        { 1, 0, 1, 2, 3 },  
        { 2, 1, 0, 1, 2 },  
        { 3, 2, 1, 0, 1 },  
        { 4, 3, 2, 1, 0 }
	};


/*二维数组按行顺序存储在内存中的，第一行存完接着存第二行，第二行存完接着存第三行。。。。
根据二维数组在内存中的存储方式，可以将二维数组看作一维数组处理
可以将二维数组强转成一级指针，根据一级指针遍历二维数组的所有元素。*/	
	SymmetricMatrix<int>sm((int*)a,5);
	cout<<sm<<endl;


	return 0;
}