
/**
 * describe:³Ø¹«¹²Àà
 * create date:2016/2/22 15:46:06
 * author:wangzhia
 */
#ifndef _POOR_H_
#define _POOR_H_

#include "avltree.h"
#include "Define.h"

using namespace std;

//³Ø
template<class D> 
class Poor
{
public:
	Poor();
	
	~Poor();
	
	void PoorInit() {m_DataList.setDelayTime(1);m_DataList.setMaxNum (MAX_DEQUE_NUM);}
	
	void PutRecord (D *d) {m_DataList.push_first (d);}
		
	D *GetRecord () {return m_DataList.pop();}
	
	void ClearRecord () {m_DataList.clear();};
	
	int GetRecordNum () {return m_DataList.sum();};
	
private:
	deque2<D> m_DataList;
	
};

template<class D> 
Poor<D>::Poor()
{
}

template<class D> 
Poor<D>::~Poor()
{
}

#endif
