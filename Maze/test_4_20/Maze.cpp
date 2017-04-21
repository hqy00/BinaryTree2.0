//回溯法思想实现迷宫（递归实现）,要求如下：
//保存迷宫的地图给成动态的
//迷宫的数据保存在文件中
//可自行添加辅助函数

#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;
#include<assert.h>

#define MAX_ROW  10
#define MAX_COL  10

struct Seat//坐标点
{
	Seat(int x, int y)
		: _x(x)
		, _y(y)
	{}
	int _x;
	int _y;
};

// 数据保存到文件
class Maze
{
public:
	Maze(int Row, int Col)//将迷宫地图储存完毕
	{
		FILE* mz = fopen("Map.txt","r");//从.txt文件中读取地图
		assert(mz != NULL);

		/***************动态创建数组***********************/

		//创建保存有每一行地址的空间
		_map = new int*[Row];		

		//创建保存有每一行数据的空间
		for(int idx= 0; idx < Row; idx++)
		{
			_map[idx] = new int[Col];
		}
		/**************************************************/

		for (int i = 0; i < Row; i++)
		{
			for (int j = 0; j < Col;)
			{
				//对文件中的每个字符进行获取，如果获取到的字符是我们要的，进行处理保存到数组中，如果不是，那么结束这次循环，读取下一个字符。
				char ch = fgetc(mz);
				if (ch == '0' || ch == '1')//注意这里是字符，不是数字
				{
					_map[i][j] = ch-'0';
					j++;//一定要注意，因为我是按照矩阵来写的，数与数之间有空格，行与行之间有回车，所以当取到不为‘0’或‘1’的数时，j不会自增
				
				}				
			}
		}

		fclose(mz);
	}

	bool IsPass(const Seat& s)//判断是否为通路，1是通路，0是墙（死路）
	{
		if(s._x < 0 || s._x >= MAX_ROW || 
			s._y < 0 || s._y >= MAX_COL)//为了不使访问的坐标非法，在这里检测到是出口时就返回
			return true;


		if(1 == _map[s._x][s._y])//是1返回true,是0返回false
			return true;

		return false;
	}

	bool PassMaze(Seat s)//将入口点传入
	{
		if(s._x < 0 || s._x >= MAX_ROW || s._y < 0 || s._y >= MAX_COL)//判断是否为出口
			return true;

		 //判断是否为通路
		if(IsPass(s))
		{
			_map[s._x][s._y] = 2;

			Seat front(s._x-1, s._y);//向上
			Seat left(s._x, s._y-1); //向左
			Seat right(s._x, s._y+1);//向右
			Seat down(s._x+1, s._y); //向下

			if(PassMaze(front))//递归
			{
				return true;
			}
			else if(PassMaze(left))//递归
			{
				return true;
			}
			else if(PassMaze(right))//递归
			{
				return true;
			}
			else if(PassMaze(down))//递归
			{
				return true;
			}
			else
			{
				_map[s._x][s._y] = 3;
			}
		}

		return false; //开始回溯
	}

	void PrintMaze()
	{
		for (int i = 0; i < MAX_ROW; ++i)
		{
			for (int j = 0; j < MAX_COL;++j)
			{
				cout<<_map[i][j]<<" ";
				
			}
			cout<<endl;
		}
		cout<<endl;
	}


	~Maze()//析构(销毁动态开辟的数组)
	{ 
		//释放保存有每一行数据的空间
		for(int idx=0; idx< MAX_ROW; idx++)
		{
			delete[] _map[idx];
		}

		 //释放保存有每一行地址的空间
		delete[] _map;
	}

private:
	int** _map;

};


int main()
{
		Maze maze(MAX_ROW, MAX_COL);
		maze.PrintMaze();
		maze.PassMaze(Seat(9, 6));
		maze.PrintMaze();
		return 0;
}


//最终结果：
//1.成功从入口点走出去了。那么在入口和出口间，有一条由2组成的路。
//2.失败了。那么在入口和出口间，不会出现一条由2组成的路。但会在迷宫里出现一条或多条由三组成的路，这些
//都是找迷宫出口失败后回溯回去的路线，且这些由3组成的路最终都从最初的入口点出去了。



//循环实现迷宫（用栈来实现）

#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
#include<stack>
using namespace std;
#include<assert.h>


#define MAX_ROW  10
#define MAX_COL  10

struct Seat//坐标点
{
	Seat(int x = 0, int y = 0)
		: _x(x)
		, _y(y)
	{}
	int _x;
	int _y;
};

// 数据保存到文件
class Maze
{
public:
	Maze(int Row, int Col)//将迷宫地图储存完毕
	{
		FILE* mz = fopen("Map.txt","r");//从.txt文件中读取地图
		assert(mz != NULL);

		/***************动态创建数组***********************/

		//创建保存有每一行地址的空间
		_map = new int*[Row];		

		//创建保存有每一行数据的空间
		for(int idx= 0; idx < Row; idx++)
		{
			_map[idx] = new int[Col];
		}
		/**************************************************/

		for (int i = 0; i < Row; i++)
		{
			for (int j = 0; j < Col;)
			{
				//对文件中的每个字符进行获取，如果获取到的字符是我们要的，进行处理保存到数组中，如果不是，那么结束这次循环，读取下一个字符。
				char ch = fgetc(mz);
				if (ch == '0' || ch == '1')//注意这里是字符，不是数字
				{
					_map[i][j] = ch-'0';
					j++;//一定要注意，因为我是按照矩阵来写的，数与数之间有空格，行与行之间有回车，所以当取到不为‘0’或‘1’的数时，j不会自增
				
				}				
			}
		}

		fclose(mz);
	}

	bool IsPass(const Seat& pos)//判断是否为通路，1是通路，0是墙（死路）
	{
		if(pos._x < 0 || pos._x >= MAX_ROW || 
			pos._y < 0 || pos._y >= MAX_COL)//为了不使访问的坐标非法，在这里检测到是出口时就返回
			return true;


		if(1 == _map[pos._x][pos._y])//是1返回true,是0返回false
			return true;

		return false;
	}

	bool PassMaze(Seat pos)//将入口点传入
	{
		stack<Seat> s;
		s.push(pos);

		while(!s.empty())
		{
			Seat &cur = s.top();
			Seat next;
			
			if(cur._x < 0 || cur._x >= MAX_ROW || cur._y < 0 || cur._y >= MAX_COL)//判断是否为出口
				return true;

			_map[cur._x][cur._y] = 2;//走过的点标记为2

			next = cur;//向上
			--next._x ;
			if(IsPass(next))
			{
				s.push(next);
				continue;
			}

			
			next = cur;//向左
			--next._y ;
			if(IsPass(next))
			{
				s.push(next);
				continue;
			}

			next = cur;//向右
			++next._y ;
			if(IsPass(next))
			{
				s.push(next);
				continue;
			}

			next = cur;//向下
			++next._x ;
			if(IsPass(next))
			{
				s.push(next);
				continue;
			}

			//该点附近没有通路 
			Seat prev = s.top();  
			_map[prev._x][prev._y] = 3;//退回的标记为3  
		    s.pop(); 

		}

		return false;//此迷宫没有通路，此时栈为空
	}

	void PrintMaze()
	{
		for (int i = 0; i < MAX_ROW; ++i)
		{
			for (int j = 0; j < MAX_COL;++j)
			{
				cout<<_map[i][j]<<" ";
				
			}
			cout<<endl;
		}
		cout<<endl;
	}


	~Maze()//析构(销毁动态开辟的数组)
	{ 
		//释放保存有每一行数据的空间
		for(int idx=0; idx< MAX_ROW; idx++)
		{
			delete[] _map[idx];
		}

		 //释放保存有每一行地址的空间
		delete[] _map;
	}

private:
	int** _map;

};


int main()
{
		Maze maze(MAX_ROW, MAX_COL);
		maze.PrintMaze();
		maze.PassMaze(Seat(9, 6));
		maze.PrintMaze();
		return 0;
}

