#ifndef _DATE_H_
#define _DATE_H_

#include<iostream>
#include<assert.h>
using namespace std;

class Date
{
public:

	Date()	//构造函数
	{
		_year = 0;
		_month = 0;
		_day = 0;
	}

	Date(int year, int month, int day)	//构造函数
	{
		if((year > 0)&&(month >0 && month <13)&&(day >0 && day<=GetDays(year,month)))	// 日期合法性检查
		{
			_year = year;
			_month = month;
			_day = day;

		}
		else
		{
			cout<<"输入的日期不合法，请重新输入！"<<endl;
		}
	}

	Date(const Date& d1)	//拷贝构造函数
	{
		_year = d1._year;
		_month = d1._month;
		_day = d1._day;
	}

	~Date()	//析构函数
	{}
	friend ostream& operator<<(ostream& _cout, const Date& d);//友元<<运算符重载函数的声明
	Date& operator=(const Date& d);
	Date operator+(int days);
	Date operator-(int days);
	int operator-(const Date& d);
	Date& operator++();
	Date operator++(int);
	Date& operator--();
	Date operator--(int);
	bool  Date::operator>(const Date& d);
	bool Date::operator<(const Date& d);
	bool Date::operator==(const Date& d);
	bool Date::operator!=(const Date& d);
	bool IsLeapyear(int year);	//判断是否是闰年
	int Date::GetDays(int year,int month);	//获得某年某月的天数


private:
	int _year;
	int _month;
	int _day;
};



#endif