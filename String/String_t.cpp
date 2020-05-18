#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include "String_t.h"
#include <cmath>

using namespace std;
unsigned int string_t::defCapacity = 2;
bool string_t::caseSens = true;
size_t string_t::numOfInstances = 0;

string_t::string_t()
{
	buildStr(0);
	numOfInstances++;
}

string_t::string_t(const char* newStr)
{
	buildStr(newStr);
	numOfInstances++;
}

string_t::string_t(const string_t& str)
{
	buildStr(str.m_str);
	numOfInstances++;
}

string_t::~string_t()
{
	delete[] m_str;
	numOfInstances--;
}

int string_t::getLength() const
{
	return strlen(m_str);
}

const char* string_t::getString() const
{
	return m_str;
}

void string_t::setString(const char* str)
{
	delete[] m_str;
	buildStr(str);
}

int string_t::compare(const string_t& str) const
{
	int result = 0;
	if(caseSens)
	{
		result = strcmp(m_str, str.m_str);
	}

	else
	{
		result = strcasecmp(m_str, str.m_str);
	}
	
	if(result != 0)
	{
		result = (result<0)?1:2;
	}
	return result;
}

int string_t::firstOccurance(const char c) const
{
	int i = 0;

	if(caseSens)
	{
		while(m_str[i] != '\0')
		{
			if(m_str[i]==c)
			{
				return i;
			}
			i++;
		}
	}

	else
	{
		while(m_str[i] != '\0')
		{
			if(toupper(m_str[i])==toupper(c))
			{
				return i;
			}
			i++;
		}	
	}
	return -1;
}

int string_t::lastOccurance(const char c) const
{
	int len = strlen(m_str);
	int i;
	if(caseSens)
	{
		for (i = len-1; i >-1; --i)
		{
			if(m_str[i]==c)
			{
				return i;
			}
		}
	}

	else
	{
		for (i = len-1; i >-1; --i)
		{
			if(toupper(m_str[i])==toupper(c))
			{
				return i;
			}
		}
	}
	return i;
}

string_t& string_t::operator=(const string_t& str)
{
	if(this != &str)
	{
		delete[] m_str;
		buildStr(str.m_str);
	}

	return *this;
}

void string_t::print() const
{
	printf("%s", m_str);
}

void string_t::buildStr(const char* str)
{
	if(str != 0)
	{
		unsigned int len = strlen(str);
		if(len >= defCapacity)
		{
			m_capacity = (unsigned int) pow(2,(unsigned int)log2(len)+1);
			defCapacity = m_capacity;
		}

		else
		{
			m_capacity = defCapacity;	
		}
		m_str = new char[m_capacity];
		strcpy(m_str, str);
	}
		
	else
	{
		m_str = new char[defCapacity];
		m_str[0] = '\0';
		m_capacity = defCapacity;
	}
}

void string_t::upperCase()
{
	int i = 0;

	while(m_str[i] != '\0')
	{
		m_str[i] = toupper(m_str[i]);
		i++;
	}
}

void string_t::lowerCase()
{
	int i = 0;

	while(m_str[i] != '\0')
	{
		m_str[i] = tolower(m_str[i]);
		i++;
	}
}

string_t& string_t::operator+=(const char* str)
{
	char* tempStr = charsCat(m_str, str);
	if(tempStr != 0)
	{
		delete[] m_str;
		buildStr(tempStr);
	}
	return *this;
}

string_t& string_t::operator+=(const string_t& str_t)
{
	*this+=str_t.m_str;
	return *this;
}

string_t& string_t::prepend(const char* str)
{
	char* tempStr = charsCat(str, m_str);
	if(tempStr != 0)
	{
		delete[] m_str;
		buildStr(tempStr);
	}
	return *this;
}

string_t& string_t::prepend(const string_t& str_t)
{
	this->prepend(str_t.m_str);
	return *this;
}

bool string_t::operator<(const string_t& str_t) const
{
	return(compare(str_t) == 2);
}

bool string_t::operator>(const string_t& str_t) const
{
	return(compare(str_t) == 1);
}

bool string_t::operator>=(const string_t& str_t) const
{
	int result = this->compare(str_t);
	return(result == 1 || result == 0);
}

bool string_t::operator<=(const string_t& str_t) const
{
	int result = this->compare(str_t);
	return(result == 2 || result == 0);
}

bool string_t::operator==(const string_t& str_t) const
{
	return(compare(str_t) == 0);
}

bool string_t::operator!=(const string_t& str_t) const
{
	return(compare(str_t) != 0);
}

string_t string_t::operator()(unsigned int start, unsigned int length)
{
	string_t retVal; 
	
	if(length<(unsigned int)strlen(m_str)-start)
	{
		char* tempStr = new char[m_capacity-1];
		strncpy(tempStr, (const char*) &m_str[start], length);
		retVal.setString(tempStr); 
		delete[] tempStr;
	}
	return retVal;
}

char& string_t::operator[](unsigned int index)
{
	return m_str[index];
}
char string_t::operator[](unsigned int index) const
{
	return m_str[index];
}

bool string_t::isContains(const string_t& str_t) const
{
	return(strstr(m_str, str_t.m_str) != 0);
}

ostream& operator<<(ostream& sout, const string_t& str_t)
{
	sout << str_t.getString();
	return sout;
}

istream& operator>>(istream& sin, string_t& str_t)
{
	char tempStr[1024];
	sin>>tempStr;
	str_t.setString(tempStr);
	return sin;
}

char* string_t::charsCat(const char* str_left, const char* str_right)
{
	if(str_left != 0 && str_right != 0)
	{
		char* tempStr = new char[strlen(str_left)+strlen(str_right)+1];
		strcpy(tempStr, str_left);
		strcat(tempStr, str_right);
		return tempStr;
	}
	return 0;
}

bool string_t::setCase(bool set)
{
	bool retVal = caseSens;
	caseSens = set;
	return retVal;
}

bool string_t::getCase()
{
	return caseSens;
}

unsigned int string_t::setDefCapacity(unsigned int capacity)
{
	unsigned int retVal = defCapacity;
	capacity = (unsigned int) pow(2,(unsigned int)log2(capacity)+1);
	defCapacity = capacity;
	return retVal;
}

unsigned int string_t::getDefCapacity()
{
	return defCapacity;
}
size_t string_t::getNumOfInst()
{
	return numOfInstances;
}