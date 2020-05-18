#ifndef TExeption_t
#define TExeption_t
#include <iostream>
using namespace std;
template <class PARAM_1, class PARAM_2, class PARAM_3>
class tExeption_t
{
	
public:
	tExeption_t();
	tExeption_t(const PARAM_1& p1, const PARAM_2& p2, const PARAM_3& p3);
	tExeption_t(const tExeption_t& exept);
	tExeption_t& operator=(const tExeption_t& exept);
	~tExeption_t();

	const PARAM_1& getParam_1() const;
	const PARAM_2& getParam_2() const;
	const PARAM_3& getParam_3() const;

private:
	PARAM_1 m_p1;
	PARAM_2 m_p2;
	PARAM_3 m_p3;
};

template <class PARAM_1, class PARAM_2, class PARAM_3>
ostream& operator<<(ostream& sout, tExeption_t<PARAM_1, PARAM_2, PARAM_3>& exept)
{
	sout<<exept.getParam_1()<<endl
		<<exept.getParam_2()<<endl
		<<exept.getParam_3()<<endl;
	return sout;
}

template <class PARAM_1, class PARAM_2, class PARAM_3>
tExeption_t<PARAM_1, PARAM_2, PARAM_3>::tExeption_t(){}

template <class PARAM_1, class PARAM_2, class PARAM_3>
tExeption_t<PARAM_1, PARAM_2, PARAM_3>::tExeption_t(const PARAM_1& p1, const PARAM_2& p2, const PARAM_3& p3):m_p1(p1)
{
	m_p2 = p2;
	m_p3 = p3;
}

template <class PARAM_1, class PARAM_2, class PARAM_3>
tExeption_t<PARAM_1, PARAM_2, PARAM_3>::tExeption_t(const tExeption_t& exept)
{
	m_p1 = exept.m_p1;
	m_p2 = exept.m_p2;
	m_p3 = exept.m_p3;
}

template <class PARAM_1, class PARAM_2, class PARAM_3>
tExeption_t<PARAM_1, PARAM_2, PARAM_3>& tExeption_t<PARAM_1, PARAM_2, PARAM_3>::operator=(const tExeption_t& exept)
{
	if(this != exept)
	{
		m_p1 = exept.m_p1;
		m_p2 = exept.m_p2;
		m_p3 = exept.m_p3;
	}
	return this;
}

template <class PARAM_1, class PARAM_2, class PARAM_3>
tExeption_t<PARAM_1, PARAM_2, PARAM_3>::~tExeption_t(){}

template <class PARAM_1, class PARAM_2, class PARAM_3>
const PARAM_1& tExeption_t<PARAM_1, PARAM_2, PARAM_3>::getParam_1()const
{
	return m_p1;
}

template <class PARAM_1, class PARAM_2, class PARAM_3>
const PARAM_2& tExeption_t<PARAM_1, PARAM_2, PARAM_3>::getParam_2()const
{
	return m_p2;
}

template <class PARAM_1, class PARAM_2, class PARAM_3>
const PARAM_3& tExeption_t<PARAM_1, PARAM_2, PARAM_3>::getParam_3()const
{
	return m_p3;
}

#endif