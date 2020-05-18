#include "String_t.h"
#include <iostream>
#include <stdio.h>
using namespace std;
void printMenu();
void testGetLen();
void testGetString();
void testSetString();
void testCompare();
void testOpEq();
void printTest();
void testUppercase();
void testLowercase();
void testAppendChar();
void testAppendStr_t();
void testPrependStr_t();
void testPrependChar();
void testLessThan();
void testMoreThan();
void testLessEqThan();
void testMoreEqThan();
void testEqual();
void testNotEqual();
void testSearchSubstring();
void testFirstOccurance();
void testLastOccurance();
void testGetSubstring();


void initStr_t1();
void initStr_t1Char();
void initStr_t1Str_t2();

char tempstr[1024];
string_t str_t1;
string_t str_t2;
int main()
{
	int switchCase = 0;
	cout << "\033[2J\033[1;1H";
	while(1)
	{

		printMenu();
		cin>>switchCase;
		cout << "\033[2J\033[1;1H";
		unsigned int capacity = 0;
		bool sencase;
		switch(switchCase)
		{
			case 1:
				testGetLen();
				break;
			case 2:
				testGetString();
				break;
			case 3:
				testSetString();
				break;
			case 4:
				testCompare();
				break;
			case 5:
				testOpEq();
				break;
			case 6:
				printTest();
				break;
			case 7:
				testUppercase();
				break;
			case 8:
				testLowercase();
				break;
			case 9:
				testAppendChar();
				break;
			case 10:
				testAppendStr_t();
				break;
			case 11:
				testPrependChar();
				break;
			case 12:
				testPrependStr_t();
				break;
			case 13:
				testLessThan();
				break;
			case 14:
				testMoreThan();
				break;
			case 15:
				testMoreEqThan();
				break;
			case 16:
				testLessEqThan();
				break;
			case 17:
				testEqual();
				break;
			case 18:
				testNotEqual();
				break;
			case 19:
				testSearchSubstring();
				break;
			case 20:
				testFirstOccurance();
				break;
			case 21:
				testLastOccurance();
				break;
			case 22:
				testGetSubstring();
				break;
			case 23:
				cout << string_t::getDefCapacity() << endl;
				break;
			case 24:
				cin>>capacity;
				cout<<string_t::setDefCapacity(capacity)<<endl;
				break;
			case 25:
				cout<<string_t::getCase()<<endl;
				break;
			case 26:
				cin>>sencase;
				cout<<string_t::setCase(sencase)<<endl;
				break;
			case 27:
				cout<<string_t::getNumOfInst()<<endl;
				break;

			default:
				goto END;
				break;
		}
	}
END:
	return 0;
}

void printMenu()
{
	cout<<"\n1 - Get length\t\t12 - Prepend string\n"
		<<"2 - Get string\t\t13 - Operator <\n"
		<<"3 - Set string\t\t14 - Operator >\n"
		<<"4 - Compare\t\t15 - Operator >=\n"
		<<"5 - Operator =\t\t16 - Operator <=\n"
		<<"6 - Print string\t17 - Operator ==\n"
		<<"7 - Upper case\t\t18 - Operator !=\n"
		<<"8 - Lower case\t\t19 - Search substring\n"
		<<"9 - += char*\t\t20 - First occurance\n"
		<<"10 - += string\t\t21 - Last occurance\n"
		<<"11 - Prepend char*\t22 - Operator ()\n"
		<<"---------------------------------------\n\n" 
		<<"\t23 - get default capacity\n"
		<<"\t24 - set default capacity\n"
		<<"\t25 - get sensetive case\n"
		<<"\t26 - set sensetive case\n"
		<<"\t27 - get num of instances\n"<<endl;
}
//	1
void testGetLen()
{
	initStr_t1();
	cout<<"Length: "<<str_t1.getLength()<<endl;
}
//	2
void testGetString()
{
	initStr_t1();
	cout<<str_t1.getString()<<endl;
}
//	3
void testSetString()
{
	initStr_t1();
	cout<<"New string is: " << str_t1 << endl;
}
//	4
void testCompare()
{
	initStr_t1Str_t2();
	cout<<"\nResult: "<<str_t1.compare(str_t2)<<endl;
}
//	5
void testOpEq()
{
	initStr_t1Str_t2();
	str_t1 = str_t2; 
	cout<<"First string = "<<str_t1<<endl;
}
//	6
void printTest()
{
	initStr_t1();
	str_t1.print();
}
//	7
void testUppercase()
{
	initStr_t1();
	str_t1.upperCase();
	cout<<str_t1<<endl;
}
//	8
void testLowercase()
{
	initStr_t1();
	str_t1.lowerCase();
	cout<<str_t1<<endl;
}
//	9
void testAppendChar()
{
	initStr_t1Char();
	str_t1+=tempstr;
	cout<<str_t1<<endl;
}
//	10
void testAppendStr_t()
{
	initStr_t1Str_t2();
	str_t1+=str_t2;
	cout<<str_t1<<endl;
}
//	11
void testPrependChar()
{
	initStr_t1Char();
	str_t1.prepend(tempstr);
	cout<<str_t1<<endl;
}
//	12
void testPrependStr_t()
{
	initStr_t1Str_t2();
	str_t1.prepend(str_t2);
	cout<<str_t1<<endl;
}
//	13
void testLessThan()
{
	initStr_t1Str_t2();
	cout<<(str_t1<str_t2)<<endl;
}
//	14
void testMoreThan()
{
	initStr_t1Str_t2();
	cout<<(str_t1>str_t2)<<endl;
}
//	15
void testMoreEqThan()
{
	initStr_t1Str_t2();
	cout<<(str_t1>=str_t2)<<endl;
}
//	16
void testLessEqThan()
{
	initStr_t1Str_t2();
	cout<<(str_t1<=str_t2)<<endl;
}
//	17
void testEqual()
{
	initStr_t1Str_t2();
	cout<<(str_t1==str_t2)<<endl;
}
//	18
void testNotEqual()
{
	initStr_t1Str_t2();
	cout<<(str_t1!=str_t2)<<endl;
}
//	19
void testSearchSubstring()
{
	initStr_t1Str_t2();
	cout << str_t1.isContains(str_t2) << endl;
}
//	20
void testFirstOccurance()
{
	initStr_t1();
	cout<<"Insert char:"<<endl;
	char c;
	cin>>c;
	cout<<str_t1.firstOccurance(c)<<endl;
}
//	21
void testLastOccurance()
{
	initStr_t1();
	cout<<"Insert char:"<<endl;
	char c;
	cin>>c;
	cout<<str_t1.lastOccurance(c)<<endl;
}
//	22
void testGetSubstring()
{
	initStr_t1();
	cout<<"Insert start index:"<<endl;
	int start;
	cin>>start;
	cout<<"Insert length:"<<endl;
	int len;
	cin>>len;
	cout<<str_t1(start, len)<<endl;

}
//**********************//
void initStr_t1()
{
	cout<<"Insert string"<<endl;
	cin>>tempstr;
	str_t1.setString(tempstr);
}
void initStr_t1Char()
{
	cout<<"Insert string_t"<<endl;
	cin>>tempstr;
	str_t1.setString(tempstr);
	cout<<"Insert char*"<<endl;
	cin>>tempstr;
}

void initStr_t1Str_t2()
{
	cout<<"Insert string_t"<<endl;
	cin>>tempstr;
	str_t1.setString(tempstr);
	cout<<"Insert second string_t"<<endl;
	cin>>tempstr;
	str_t2.setString(tempstr);
}