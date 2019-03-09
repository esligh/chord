#include "stdafx.h"
#include <assert.h>
#include "node.h"
#include "hash.h"
#define  STR_LEN		30
Node::Node()
{
}
Node::~Node()
{
}
Node::Node(address& addr)
{
	hash_node(addr);	//���㱾�ؽڵ��hashֵ
	local_node  = addr;
	finger_entry fe;	
	fe.start_.value_ = fe.end_.value_ = INVALID_MAP_VALUE;
	fe.start_.value_= local_node.map.value_;
	finger_tbl.set_finger_entry(0,fe);
	int M = 1<<(get_m());	//��ȡmֵ
	int size = finger_tbl.get_finger_size();//��ȡ���С
	for(int i = 1;i<size;i++){
		finger_entry loop;
		int n = 1<<(i-1);
		int m = 1<<i;
		loop.start_.value_ = (local_node.map.value_+n)%M;
		loop.end_.value_  = (local_node.map.value_+m)%M;
		loop.successor =addr;
		//successor����Ĭ�� ��address�ṹ ���췽��
		finger_tbl.set_finger_entry(i,loop);
	}
	predecessor = local_node; //ǰ�̽ڵ㼴Ϊ���ڵ�
	bActive = false;//�ǻ�ڵ�
}
//������ǰ�̽ڵ�
void Node::set_predecessor(const address& ad)
{
	predecessor = ad;
}
const address& Node::get_predecessor()const
{
	return predecessor;
}
//�Ƿ�
bool Node::isActive()const
{
	return bActive;
}
//���û���
void Node::setActive(bool b)
{
	bActive =b;
}
//�洢�ؼ��ֱ���
bool Node::store_key(const key_entry& ke)
{
	return (key_tbl.add_keyEntry(ke) == OK);
}
const FingerTable& Node::get_finger_table()const
{
	return finger_tbl;
}
hash_value Node::get_hash_value()const
{
	return local_node.map;	
}
void Node::set_successor(const address& ad)
{
	finger_tbl.set_successor(ad);
}
//���½ڵ��finger��Ŀ
//��chord����ad�ڵ�ļ�����뿪��Ҫ������finger��
int Node::update_finger_table(const Node& node,NODE_OPER_TYPE type)
{
	if(CommonEnv::node_equal(local_node,node.get_localnode()))
		return 0;
	int size = finger_tbl.get_finger_size();
	hash_value ad_hv = node.get_hash_value();	//������뿪�ڵ��hashֵ;
	hash_value local_hv = local_node.map;
	const finger_entry* base = finger_tbl.get_finger();
	assert(base != NULL);
	if(type == ADD_NODE){//�ڵ����
		for(int i = 1;i<size;i++){
			if(get_finger_table().isneed_change_successor(i,ad_hv)){
				finger_tbl.set_entry_successor(i,node.get_localnode());
			}
		}
	}else if(type == DEL_NODE){//�ڵ��뿪
		for(int i=1;i<size;i++){
			if(Hash::isequal(ad_hv,base[i].successor.map)){ //���finger�����successor���뿪�ڵ��hash_value ����Ҫ����
				address succ = node.get_successor();	//��ȡ���̵�address
				finger_tbl.set_entry_successor(i,succ);	
			}
		}
	}
	return 1;
}
//��ȡ�ڵ�ĺ��
const address& Node::get_successor()const
{
	return finger_tbl.get_successor()->successor;
}
//��ȡ���ؽڵ�
const address& Node::get_localnode()const
{
	return local_node;
}
const  KeyTable& Node::get_key_table()const
{
	return key_tbl;
}
//��������ΪIndex��finger�����successor
void Node::set_fingerEntrySuccessor(int index,const address& addr)
{
	finger_tbl.set_entry_successor(index,addr);
}
/*
�������ܣ����һ���ļ���Ϣ�����ؽڵ��keytable��
����˵��:
name: �ļ�����
addr: �ļ��Ĵ洢�ڵ��ַ
ע��ͬһ���ڵ㲻�ܹ���hash_valueֵ��ͬ���ļ�
*/
bool Node::add_file(const std::string& name,const address& addr)
{
	key_entry en;
	en.name = name;
	hash_key(name,&en.key);//�����ļ�hashֵ
	int ret = key_tbl.add_holder(en.key,addr); //����Ѿ����ڸ��ļ��ļ�¼���ֻ����Ӵ洢�ڵ��ַ��keytable��
	if(ret == -1){//û���ļ���¼
		en.holder_lst.push_back(addr); //add key 
		int  result=key_tbl.add_keyEntry(en); //��ӱ���
		return (result == 1);
	}
	return true;
}
//��chord����ɾ��s��ָ�Ľڵ�
bool Node::DelKeyEntry(KeyTable::NodePointer s)
{
	if(s==NULL)
		return false;
	return (key_tbl.DeleteList(s) == OK);	
}
/*�ڽڵ��keyTable���л�ȡKeyֵΪhv�Ĵ洢��ַ�б�*/
/*
hv: keyֵ
lst:�洢��ַ�б�
*/
bool Node::find_file(const hash_value& hv,Addr_Lst& lst)
{
	key_entry* pke = key_tbl.find_entry(hv);
	if(pke == NULL)
		return false;
	lst = pke->holder_lst;	//���ش洢�б�
	return true;
}
/*�������ܣ����Ҵ洢<K,V>�Խڵ�*/
/*����˵��:
key_hv: keyֵ
addr: ���ҽ���� �洢�ڵ�
��������:
1.����Keyֵ�Ƿ��ڱ��ؽڵ�����̽ڵ�֮��
2.����keyֵ�Ƿ���finger����,������ڣ���ѯ����key_hv�����ǰ��ڵ�
3.������finger��Ŀ�в��Ҳ�����K�����ڵ���
ע��3�в�����K�Ľڵ���ָ��Ŀ��startֵ����key��ǰ�̶�end����key�ĺ�̣�
��start��chord���ϴ���(start,end)֮��
*/
bool Node::get_closest_finger(const hash_value& key_hv,address& addr)
{	
	int size = finger_tbl.get_finger_size();
	const finger_entry* pbase= finger_tbl.get_finger();
	assert(pbase != NULL&&size>1);
	hash_value cur_hv = local_node.map; //���ؽڵ�hash
	hash_value successor_hv = get_successor().map; //��̽ڵ�hash
	if(Hash::between(key_hv,cur_hv,successor_hv)){ //��ѯkey�Ƿ��ڱ��ؽڵ������֮�� 
		addr=get_successor();
		return true; //�ҵ��洢<K,V>�ڵ�
	}else if(!Hash::between(key_hv,pbase[1].start_,pbase[size-1].start_)){ //key_hv����finger���У���ѯ��key_hv�����ǰ��ڵ�
		hash_value min_hv = Hash::get_max();
		hash_value distance;
		int index=0;
		for(int i =1;i<size;i++){
			Hash::distance(pbase[i].successor.map,key_hv,&distance); //����ڵ�hash��key_hv�ľ���
			if(Hash::issmall(distance,min_hv)){
				min_hv = distance;
				index = i;
			}
		}
		addr = (pbase[index].successor); //�ҵ�keyֵ�����ǰ��ڵ�
		return false; 
	}else{//��ѯfinger��Ŀ ����key��(start,end)֮���finger[i].successor
		for(int i=2;i<size-1;i++){
			if(Hash::isequal(key_hv,pbase[i].start_)){ //�ҵ�keyֵ��ֱ�ӷ��� successor �����سɹ�
				addr =pbase[i].successor;
				return true; 
			}
			if(Hash::between(key_hv,pbase[i].start_,pbase[i].end_)){
				addr= pbase[i].successor;
				return false;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
int Node::hash_node(address& addr)
{
	char temp[STR_LEN];
	sprintf_s(temp,STR_LEN,"%s:%d",addr.ip.c_str(),addr.port);
	Hash::_hash(std::string(temp),&addr.map);
	return 1;
}
int Node::hash_key(const std::string& name,hash_value* phv)
{
	return Hash::_hash(name,phv);
}
//copy construct
Node::Node(const Node& node)
{
	if(this != &node){
		local_node  = node.local_node;
		predecessor = node.predecessor;
		finger_tbl	   = node.get_finger_table();
		key_tbl		   = node.get_key_table();
		bActive		 =node.isActive();
	}
}
Node& Node::operator=(const Node& rhs)
{
	local_node = rhs.local_node;
	predecessor = rhs.local_node;
	copy_finger(rhs.get_finger_table());
	key_tbl = rhs.get_key_table();
	bActive = rhs.isActive();
	return *this;
}
bool Node::copy_finger(const FingerTable& other)
{
	int size =other.get_finger_size();
	for(int i=0;i<size;i++){
		const finger_entry* pfe = other.get_finger_entry(i);
		if(pfe == NULL) 
			return false;
		finger_entry fe;
		fe.start_ = pfe->start_;
		fe.end_ =pfe->end_;
		fe.successor = pfe->successor;
		finger_tbl.set_finger_entry(i,fe);
	}
	return true;
}
//debug 
void Node::PrintFingerTable()
{
	finger_tbl.PrintFingerTable();
}
void Node::PrintKeyTable()
{
	char temp[20];
	sprintf_s(temp,20,"node hash_value=%d\n",get_hash_value().value_);
	Debuger::PrintDescript(temp);
	key_tbl.PrintKeyTable();
}