#ifndef _CHORD_EVR_H_
#define _CHORD_EVR_H_
#include "chordring.h"
/*���Ի�����*/
class Environment
{
public:
	Environment();
	Environment(Node& node);
	~Environment();
	bool join(const Node& node);
	bool leave(const Node& node);
	bool share_file(const Node&node,const std::string&filename);
	bool find_file(const Node& node,const hash_value& hv,Addr_Lst& list);
	//add other test function
	/*......*/
//////////////////////////////////////////////////////////////////////////
	//����
	void PrintFingerTable()const;
	void PrintKeyTable()const;
private:
	ChordRing _ring;
};
#endif/*Environment.h*/