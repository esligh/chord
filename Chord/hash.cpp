#include "stdafx.h"
#include "hash.h"
#include <assert.h>
int Hash::hash_bits = 0;
hash_value Hash::max ;
hash_value Hash::min;
void Hash::set_hashbits(int m)
{
	hash_bits = m;
}
int Hash::get_hashbits()
{
	return hash_bits;
}
//hash���� ����  ��chord��ʹ��SHA-1
int Hash::_hash(const std::string& r,hash_value* va)
{
	if(r.empty())
		return 0;
	int sum =0 ;
	int size = 1<<hash_bits;
	size_t len =r.length();
	for(size_t i =0;i<len;i++){
		sum+=(r[i]+i)*7;
	}
	va->value_ = abs(sum%size);
	return 1;
}
//�ж�hashֵ�Ƿ����
bool Hash::isequal(const hash_value& v1,const hash_value& v2)
{
	return (v1.value_ == v2.value_);
}
//�ж�v1�Ƿ����v2 ��hashֵ
bool Hash::islarge(const hash_value& v1,const hash_value& v2)
{
	return (v1.value_>v2.value_);
}
bool Hash::issmall(const hash_value& v1,const hash_value& v2)
{
	return (v1.value_<v2.value_);
}
//between�����ж�һ���ڵ���chord�����Ƿ��������ڵ�֮��
/*
����˵��: v ���жϽڵ�
(v1��v2]�ڵ�ָ���жϵ����伴(pre_node,cur_node] 
ע:��chord�ϰ�˳ʱ������ж�
*/
bool Hash::between(const hash_value& v,const hash_value& v1,const hash_value& v2)
{
	if(isequal(v,v2)){  //v1��v2��ͬ������
		return true; 
	}else if(islarge(v2,v1)){
		if(islarge(v,v1) && issmall(v,v2)){
			return true;
		}else{
			return false;
		}
	}else{
		if((islarge(v,v1)&& islarge(v,v2)) || issmall(v,v1)&& issmall(v,v2)){
			return true;
		}else{
			return false;
		}
	}
}

const hash_value& Hash::get_max()
{
	max.value_ = (1<<get_hashbits());
	return max;
}
const hash_value& Hash::get_min()
{
	min.value_ =0;
	return min;
}
//������chord���������ڵ�ľ���(����˳ʱ�뷽�򣬼�v2Ϊv1�ĺ��) 
void Hash::distance(const hash_value& v1,const hash_value& v2,hash_value* pr)
{
	int mod = 1<<(get_hashbits());
	if(issmall(v1,v2)){
		pr->value_=v2.value_-v1.value_;
	}else if(islarge(v1,v2)){
		pr->value_ = (v2.value_+mod)-v1.value_;
	}else{
		pr->value_ = 0;
	}
}