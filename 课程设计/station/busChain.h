#ifndef busChain_
#define busChain_

#include<iostream>
#include<string>
#include<cmath>
#define noEdge 9999
using namespace std;

struct busNode {
	string sta_name;//站点名称
	double dx;//站点横坐标
	double dy;//站点纵坐标
	busNode* next;//指向下一个站点的指针
};

//公交车链表 
class busChain {
	friend class busRoute;//友元
public:
	busChain() {//构造函数
		firstNode = NULL;
		listSize = 0;
	}
	~busChain();//析构函数

	bool isEmpty() { return listSize == 0; }//判断是否为空
	int size() { return listSize; }//站点数目
	busNode* getFirst() { return firstNode; }//得到首节点 
	string getFirstStation() { return firstNode->sta_name; }//得到始发站 
	void insert(const string& s, const double& x, const double& y);//在链表最后插入一个节点 
	void get_distance();//计算每两个站点之间的距离，存入dx 
	void output(ostream& out) const;//输出

private:
	busNode* firstNode;//指向首节点的指针
	int listSize;//节点个数
};

//析构函数
busChain::~busChain() {
	cout << "111" << endl;
	busNode* p = firstNode, * pp;
	while (p != NULL) {
		pp = p->next;
		delete p;
		p = pp;
	}
}

//在最后插入一个站点 
void busChain::insert(const string& s, const double& x, const double& y) {
	if (firstNode == NULL) {
		firstNode = new busNode;
		firstNode->sta_name = s;
		firstNode->dx = x;
		firstNode->dy = y;
		firstNode->next = NULL;
	}
	else {
		busNode* p = firstNode;
		while (p->next != NULL) {
			p = p->next;
		}

		busNode* pp = new busNode;
		pp->sta_name = s;
		pp->dx = x;
		pp->dy = y;
		p->next = pp;
		pp->next = NULL;
	}
	listSize++;
	//return *this;
}

void busChain::get_distance() {
	busNode* p = firstNode, * pp;
	for (pp = p->next; pp != NULL; pp = pp->next) {
		p->dx = sqrt(abs(p->dx - pp->dx) * abs(p->dx - pp->dx) + abs(p->dy - pp->dy) * abs(p->dy - pp->dy));
		p = pp;
	}
	p->dx = noEdge;
}

void busChain::output(ostream& out) const {
	busNode* p;
	for (p = firstNode; p->next != NULL; p = p->next) {
		out << p->sta_name << "(" << p->dx << "," << p->dy << ")" << "->";
	}
	out << p->sta_name << "(" << p->dx << "," << p->dy << ")" << "。";
}

ostream& operator<<(ostream& out, const busChain& theChain) {
	theChain.output(out);
	return out;
}

#endif
#pragma once

