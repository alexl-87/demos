#include <iostream>
#include <string>
#include "TExeption_t.h"
using namespace std;
void foo1();
void foo2();
void foo3();

int main(int argc, char const *argv[])
{
	try
	{
		foo1();
	}

	catch(tExeption_t<int,  string, int> exept)
	{
		cout<<exept;
	}

	catch(...)
	{
		cout<<"error"<<endl;
	}

	return 0;
}

void foo1()
{
	try
	{
		foo2();
	}
	
	catch(...)
	{
		throw;
	}
}

void foo2()
{
	try
	{
		foo3();
	}
	
	catch(...)
	{
		throw;
	}
}

void foo3()
{
	
	throw tExeption_t<int,  string, int> 
			(6, __FILE__, __LINE__);
}

