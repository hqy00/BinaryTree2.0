#include "date.h"

ostream& operator<<(ostream& _cout, const Date& d)	//友元<<运算符重载函数的定义
{
	_cout<<d._year<<"-"<<d._month<<"-"<<d._day;
	return _cout;
}



Date& Date::operator =(const Date &d)	//赋值运算符重载
{
	if(this != &d)	//判断是否自己给自己复制
	{
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}

	return *this;

}



bool  Date::IsLeapyear(int year)	//判断是否是闰年
{
	if((year % 4 ==0)&&(year % 100 != 0)||(year % 400 ==0))
	{
		return true;
	}

	return false;
}

	
int Date::GetDays(int year,int month)	//获得某年某月的天数
{
	assert(month > 0 && month <13);
	int day[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if((month == 2)&&(IsLeapyear(year)==1))
	{
		day[2] += 1;
		return day[2];
	}

	return day[month];

}

Date Date::operator +(int days)  //实现增加若干天
{
	Date temp = *this;	
	if(days < 0)	
	{
		return *this - (0 - days);	//调用“-”运算符重载
	}

    temp._day = _day + days;	
    while (temp._day>GetDays(temp._year,temp._month))	//检测是否加后的日期大于当年当月最大日期
    {
        temp._day -= GetDays(temp._year, temp._month);	//如果大，用现有日期减去当年当月最大日期
        if (temp._month < 12)	//月份加1
            temp._month++;
        else
        {
			temp._month = 1;
            temp._year++;
        }
    }
    return temp;
}


Date Date::operator-(int days)	//实现倒退若干天
{
	Date temp = *this;	
	if(days < 0)
	{
		return *this + (0 - days);	//调用“+”运算符重载
	}
    temp._day = _day - days;	//还需往前倒退temp._day天
	while(temp._day <= 0)	
	{
		if(temp._month > 1 && temp._month <13)	//如果是负数，往前倒一个月
		{
			temp._month--;
		}
		else
		{
			temp._month = 12;
			temp._year--;
		}
		
		temp._day += GetDays(temp._year,temp._month);//如果退回天数加上这个月的最大日期还是负数，
													//重复此循环,直至为正数
	}

	return temp;
}


int Date:: operator-(const Date& d)	//计算两个日期对象之间相差的天数
{   
    Date max(*this);  
    Date min(d);
	int day = 0; 
    if (max < min)	//“<”的左右两个操作数都是类类型，调用“<”运算符重载
	{  
        max = d;  
        min = *this;  
    }  
 
    while (1) 
	{  
       if ((min + day)== max)  //调用“+”运算符重载，也调用了“==”运算符重载
	   {
		   break; 
	   } 
        day++;  
    }

    return day;  
}  



Date& Date:: operator++()	//前置++
{
	*this = *this + 1;		//因为“+”左操作符是类类型对象，要调用“+”重载运算符
	return *this;



	//int month_days = GetDays(_year,_month);		//某年某月有多少天
	//if(_day <　month_days)	//没有超出当月最大天数
	//{
	//	_day++;
	//}
	//else if(_day ==　month_days))	//等于当月最大天数
	//{
	//	_day = 1;
	//	if(_month <12)
	//	{
	//		_month++;
	//	}
	//	else
	//	{
	//		_month = 1;
	//		_year++;
	//	}
	//}
	//else
	//{
	//	assert(false);
	//}

	//return *this;

}


Date Date::operator++(int)	//后置++
{
	Date temp = *this;
	*this = *this + 1;
	return temp;
	//int month_days = GetDays(_year,_month);		//某年某月有多少天
	//int temp = *this;
	//if(_day <　month_days)	//没有超出当月最大天数
	//{
	//	_day++;
	//}
	//else if(_day ==　month_days))	//等于当月最大天数
	//{
	//	_day = 1;
	//	if(_month <12)
	//	{
	//		_month++;
	//	}
	//	else
	//	{
	//		_month = 1;
	//		_year++;
	//	}
	//}
	//else
	//{
	//	assert(false);
	//}

	//return temp;

}



Date& Date::operator--()	//前置--
{
	*this = *this - 1;		//因为“-”左操作符是类类型对象，要调用“-”重载运算符
	return *this;

}

Date Date::operator--(int)	//后置--
{
	Date temp = *this;
	*this = *this - 1;
	return temp;

}



bool  Date::operator>(const Date& d)
{
    if ((_year > d._year) || ((_year == d._year) &&( _month > d._month) )
        || ((_year == d._year) && (_month == d._month) && (_day > d._day)))
        return true;
    else
        return false;
}

bool Date::operator<(const Date& d)
{
    if ((_year < d._year) || ((_year == d._year) && (_month < d._month))
        || ((_year == d._year) && (_month == d._month) && (_day < d._day)))
        return true;
    else
        return false;
}

bool Date::operator==(const Date& d)
{
    return ((_year == d._year) && (_month == d._month) && (_day == d._day));
}

bool Date::operator!=(const Date& d)
{
    return !(*this == d);	//因为“==”两边的操作数都是类类型的，所以调用“==”运算符重载
}



//int main()
//{
//	Date d1(2017,2,16);
//	Date d2(2016,11,11);
//	//Date d3;
//	//d3 = d2;
//	//d3 = d1++;
//	cout<<d2-d1<<endl;
//	//cout<<d1-30<<endl;
//	//cout<<d1<<"fsfsgdfg"<<d2<<endl;
//	//system("pause");
//	return 0;
//}