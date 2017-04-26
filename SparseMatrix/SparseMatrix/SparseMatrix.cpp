//作业名称：稀疏矩阵的压缩存储
#include<iostream>
#include<vector>
using namespace std;
#include<assert.h>

template<class T>
class SparseMatrix
{
	//三元组（行，列，数据）
	template<class T>
	struct Trituple
	{
		Trituple(size_t row, size_t col, const T& data)//带参构造
			: _row(row)
			, _col(col)
			, _data(data)
		{}

		Trituple()//无参构造
		{}

		size_t _row;
		size_t _col;
		T _data;
	};

public:
	
	// 稀疏矩阵的压缩存储

	SparseMatrix()//无参构造
	{}
     
	SparseMatrix(T* array, size_t r, size_t c, const T& invalid)//带参构造
		: _r(r)
		, _c(c)
		, _invalid(invalid)
	{
		for(size_t i = 0;i < _r; ++i)
		{
			for(size_t j = 0;j < _c; ++j)
			{
				if(invalid != array[i * _c + j])
				{
					Trituple<T> tmp(i,j,array[i*_c + j]);
					_sm.push_back(tmp);
				}
			}
		}
	}

    //访问稀疏矩阵中r行c列中的元素
	/**在保存有效元素的_sm数组中找对应的r行c列中的元素，找到了返回元素的值，找不到返回无效值**/
	T& Access(int r, int c)
	{
		//第一种for循环方式进行遍历
		//for(size_t i =0;i < _sm.size();++i)
		//{
		//	if(r == _sm[i]._row && c == _sm[i]._col)
		//	{
		//		return _sm[i]._data;
		//	}
		//}
		//return _invalid;


		//第二种用迭代器进行遍历
		vector<Trituple<T>>::iterator it = _sm.begin();
		while(it != _sm.end())
		{
			if(r == it->_row && c == it->_col)
			{
				return it->_data;
			}
			++it;
		}
		return _invalid;

	}

    
	// 还原稀疏矩阵
	/**注意：友元函数不是类成员函数，要使用类模板参数要自己重新定义**/
	template<class T>	
	friend ostream& operator<<(ostream& _cout, SparseMatrix<T>& s)
	{
		//第一种用Access方式,缺点每次访问一个元素都要调用Access一次
		//for(size_t i = 0;i < s._r ; ++i)
		//{
		//	for(size_t j = 0;j < s._c; ++j)
		//	{
		//		_cout<<s.Access(i,j)<<" ";
		//	}
		//	
		//	_cout<<endl;
		//}
		//return _cout;
		
		//第二种用if语句判断
		size_t index = 0;
		for(size_t i =0; i < s._r; ++i)
		{
			for(size_t j =0; j < s._c; ++j)
			{
				//一定要加index < s._sm.size()条件约束，不然当_sm数组已全部遍历完，但矩阵还未访问完毕时，
				//两个for循环仍然能进入，此时在if语句处就会发生对_sm数组下标的访问不合法
				if(index < s._sm.size() && i == s._sm[index]._row && j == s._sm[index]._col)
				{
					_cout<<s._sm[index]._data<<" ";
					++index;
				}
				else
				{
					_cout<<s._invalid<<" ";
				}
			}
			_cout<<endl;
		}
		_cout<<endl;
		return _cout;

	}

	// 稀疏矩阵的普通逆置，要求行优先存储，给出时间复杂度
	/**如果只是将i,j简单交换，那么转制后的新矩阵是按照列优先存储的，不符合要求。最终我们想到，
	因为原矩阵的列是新矩阵的行，故以列优先先将原矩阵的有效元素存起来，这样一来对于新矩阵就是按行存储的。**/
	SparseMatrix<T> Transprot()
	{
		SparseMatrix<T> ts;
		ts._r = _c;	//ts的行数为原矩阵的列数
		ts._c = _r;	//ts的列数为原矩阵的行数
		ts._invalid = _invalid;

		//让第i列与原矩阵_sm数组中的所有有效值的列数进行比较，若列数一样就将此有效值保存到新矩阵的_sm数组中
		//保存时要记得交换行和列，因为原矩阵的行数是新矩阵的列数，原矩阵的列是新矩阵的行数
		for(size_t i = 0;i < _c; ++i)
		{
			size_t index  = 0;
			while(index < _sm.size())
			{
				if(i == _sm[index]._col)
				{
					Trituple<T> tmp(i,_sm[index]._row,_sm[index]._data);
					ts._sm.push_back(tmp);
				}
				index++;
			}
		}

		return ts;
	}
	//当有效元素个数为 M 个，有 N 列，快速转置的时间复杂度为O(N * M)

	// 稀疏矩阵的快速逆置，给出时间复杂度
	SparseMatrix<T> FastTransprot()
	{
		// 1>原矩阵有效元素个数与转制后的新矩阵有效元素的个数相同，先给新矩阵开辟保存有效元素的_sm数组空间
		
		/**之前没有给原矩阵开辟，因为_sm构造时为空，放入有效元素用的是push_back()（先构造结点再尾插），
		依次插入进其动态数组_sm里,_sm自动增容。但现在采取的方法，将有效元素放入新矩阵_sm里时不是push_back()，
		是根据下标的位置选择插入的，这种先选择_sm数组中的位置再插入的方式，对于刚构造好的空的_sm,
		下标访问是不合法，所以必须先开辟好_sm的空间**/

		SparseMatrix<T> fts;
		fts._r = _c;	//fts的行数为原矩阵的列数
		fts._c = _r;	//fts的列数为原矩阵的行数
		fts._invalid = _invalid;
		size_t _size = _sm.size();	//获取原矩阵_sm的空间大小
		fts._sm.resize(_size);	//给新矩阵的_sm开辟了一个大小为_size的空间，空间填充值0

		// 2>统计原矩阵中每一列有多少个有效元素
		int* Pcount = new int[_c];	//创建大小为列数_c的动态数组Pcount,数组初始值为0
		memset(Pcount,0,_c*sizeof(Pcount[0]));
		for(size_t i = 0; i < _sm.size(); ++i)
		{
			Pcount[_sm[i]._col]++; 

		}

		// 3>统计原矩阵每一列的第一个有效数据，在转制后新矩阵的有效数组_sm中的起始位置（下标）
		
		int* Paddr = new int[_c];//创建大小为列数_c的动态数组Paddr，数组初始值为0
		memset(Paddr,0,_c*sizeof(Paddr[0]))
		//因为数组初始值为0，而第一列的起始地址正好为0，所以就不用管第一列的起始地址了
		for(size_t i = 1; i <_c; ++i)
		{
			Paddr[i] = Paddr[i-1] + Pcount[i-1];
		}

		// 4>放置元素到新矩阵的有效数组_sm中
		for(size_t i =0; i < _sm.size(); ++i)
		{
			int& addr = Paddr[_sm[i]._col];
			fts._sm[addr] = Trituple<T> (_sm[i]._col,_sm[i]._row,_sm[i]._data);
			++addr;
		}

		delete[] Pcount;
		delete[] Paddr;

		return fts;

	}

	//当有效元素个数为 M 个，有 N 列，快速转置的时间复杂度为O(M)+O(N)
	
	
	// 实现稀疏矩阵的加法操作(方法一)
	/*思想：将两个矩阵都当一维数组处理，比较各自有效元素在一维数组中的下标，确定谁先存入ret的_sm中。
	当下标相同时，在该位置求和，再判断和是否为有效值。和是有效值，放入ret的_sm中；否则，则不放。*/
	SparseMatrix<T> operator+(const SparseMatrix<T>& sp)
	{
		// 1>判断两个矩阵是否同行同列
		assert(_r == sp._r && _c == sp._c);
		
		// 2>给返回值ret赋初值
		SparseMatrix<T> ret;
		ret._r = _r;
		ret._c = _c;
		ret._invalid  = _invalid;
		size_t iLdex = 0, iRdex = 0;	//定义两个索引

		// 3>left 与 right 的比较
		while(iLdex < _sm.size() && iRdex < sp._sm.size())
		{
			//当一维数组处理，找到有效元素在各自一维数组中的下标，比较下标大小进行操作
			size_t left = _sm[iLdex]._row * _c + _sm[iLdex]._col;
			size_t right = sp._sm[iRdex]._row * _c + sp._sm[iRdex]._col;
			if(left < right)
			{
				ret._sm.push_back(Trituple<T>(_sm[iLdex]._row,_sm[iLdex]._col,_sm[iLdex]._data));
				++iLdex;
			}
			else if(left > right)
			{
				ret._sm.push_back(Trituple<T>(sp._sm[iRdex]._row,sp._sm[iRdex]._col,sp._sm[iRdex]._data));
				++iRdex;
			}
			else//下标相同，求和
			{
				Trituple<T> temp(_sm[iLdex]);
				temp._data += sp._sm[iRdex]._data;

				if(temp._data)//判断和是否为有效值
				{
					ret._sm.push_back(temp);//和是有效值，放入ret的_sm中
				}
				++iLdex;
				++iRdex;
			}
		}

		//右边矩阵的_sm数组先遍历结束，左边矩阵的_sm数组中还有剩余，将左边的剩余放入ret的_sm中
		while(iLdex < _sm.size())
		{
			ret._sm.push_back(Trituple<T> (_sm[iLdex]._row,_sm[iLdex]._col,_sm[iLdex]._data));
			++iLdex;
		}


		//左边矩阵的_sm数组先遍历结束，右边矩阵的_sm数组中还有剩余，将右边的剩余放入ret的_sm中
		while(iRdex < sp._sm.size())
		{
			ret._sm.push_back(Trituple<T> (sp._sm[iRdex]._row,sp._sm[iRdex]._col,sp._sm[iRdex]._data));
			++iRdex;
		}

		return ret;
	}

	// 实现稀疏矩阵的加法操作(方法二)
	/*思想：先比较有效元素在各自矩阵中的行位置，确定谁先存入ret的_sm中。当行位置相同时，
	再比较列位置，确定谁先存入ret的_sm。若行列都相同，在该位置求和，再判断和是否为有效值。
	是有效值，就将求和后的插入，否则不插入。*/
	SparseMatrix<T> operator+(const SparseMatrix<T>& sp)
	{
		// 1>判断两个矩阵是否同行同列
		assert(_r == sp._r && _c == sp._c);

		// 2>给返回值ret赋初值
		SparseMatrix<T> ret;
		ret._r = _r;
		ret._c = _c;
		ret._invalid  = _invalid;
		size_t i = 0, j = 0; //定义两个索引

		// 3> 行、列位置的比较
		while(i < _sm.size() && j < sp._sm.size())
		{
			//左矩阵有效元素的行位置 小于 右矩阵有效元素的行位置
			if(_sm[i]._row < sp._sm[j]._row)
			{
				ret._sm.push_back(Trituple<T> (_sm[i]._row,_sm[i]._col,_sm[i]._data));//放入左矩阵有效元素
				++i;
			}
			//右矩阵有效元素的行位置 小于 左矩阵有效元素的行位置
			else if(_sm[i]._row > sp._sm[j]._row)
			{
				ret._sm.push_back(Trituple<T> (sp._sm[j]._row,sp._sm[j]._col,sp._sm[j]._data));//放入右矩阵有效元素
				++j;
			}
			//左右矩阵有效元素的行位置相同，比较列位置
			else
			{
				//左矩阵有效元素的列位置 小于 右矩阵有效元素的列位置
				if(_sm[i]._col < sp._sm[j]._col)
				{
					ret._sm.push_back(Trituple<T> (_sm[i]._row,_sm[i]._col,_sm[i]._data));//放入左矩阵有效元素
					++i;
				}
				//右矩阵有效元素的列位置 小于 左矩阵有效元素的列位置
				else if(_sm[i]._col > sp._sm[j]._col)
				{
					ret._sm.push_back(Trituple<T> (sp._sm[j]._row,sp._sm[j]._col,sp._sm[j]._data));//放入右矩阵有效元素
					++j;
				}
				//左右矩阵有效元素的行位置相同，列位置也相同，求和
				else
				{
					Trituple<T> temp(_sm[i]);
					temp._data += sp._sm[j]._data;

					if(temp._data)//判断和是否为有效值
					{
						ret._sm.push_back(temp);//和是有效值，放入ret的_sm中
					}
					++i;
					++j;
				}
			}
		}

		
		//右边矩阵的_sm数组先遍历结束，左边矩阵的_sm数组中还有剩余，将左边的剩余放入ret的_sm中
		while(i < _sm.size())
		{
			ret._sm.push_back(Trituple<T> (_sm[i]._row,_sm[i]._col,_sm[i]._data));
			++i;
		}


		//左边矩阵的_sm数组先遍历结束，右边矩阵的_sm数组中还有剩余，将右边的剩余放入ret的_sm中
		while(j < sp._sm.size())
		{
			ret._sm.push_back(Trituple<T> (sp._sm[j]._row,sp._sm[j]._col,sp._sm[j]._data));
			++j;
		}
		
		return ret;
	}

private:
	vector<Trituple<T>> _sm;//动态一维数组存放矩阵中有效元素的三元组
	size_t _r;
	size_t _c;
	T _invalid;
};


int main()
{
	int array1[6][5] = {
		{1, 0, 3, 0, 5},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{1, 0, 3, 0, 5},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
	};

	int array2[6][5] = {
		{1, 0, -3, 0, 5},
		{0, 9, 0, 0, 0},
		{0, 0, 0, 0, 0},
		{1, 0, 3, 0, 5},
		{0, 0, 0, 0, 0},
		{0, 0, 0, 7, 0}
	};

	SparseMatrix<int> SM((int *)array1,sizeof(array1)/sizeof(array1[0]),sizeof(array1[0])/sizeof(array1[0][0]),0);
	SparseMatrix<int> SP((int *)array2,sizeof(array2)/sizeof(array2[0]),sizeof(array2[0])/sizeof(array2[0][0]),0);
	//cout<<SM<<endl;
	//cout<<SM.Transprot()<<endl;
	//cout<<SM.FastTransprot()<<endl;
	cout<<SM+SP<<endl;

	return 0;
}