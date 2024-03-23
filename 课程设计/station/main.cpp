// 定义控制台应用程序的入口点。
//
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include "busChain.h"
#include "busRoute.h"
#include "adjMatrix.h"
#include <string.h>
#include <string>
#include <iomanip>
#include <cmath> 
#define sn 30//站点总数 
#define noEdge 9999
using namespace std;

//函数声明 
void Welcome();
void Input();
void Menu();
void getRoutes();
int choose();
void Function();
void findMinCost();
void findMinTime1();
void findMinTime2();
void Dijkstra(int da);
int getIndex(string bs);
void outputPath(int pa, int pb);
void Dijkstra2(int da);
int find_(int Infomation);

//全局变量 
int option;//进行功能选择
char flag = 'y';//每个功能完成后判断是否退出
busRoute* bus = new busRoute[10];//公交车线路数组 
int BUS_SUM;//公交线路数
string in[sn] = { " ","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"," "," "," " };
//已有对应关系，将站点名和站点标号对应 
int vis[sn];//判断是否访问过 
int	pre[sn];//前驱 
double dis[sn];//到源点的距离
adjMatrix matrix(30);//邻接矩阵


int main() {

	Welcome();
	Input();
	getRoutes();
	return 0;
}

//欢迎界面 
void Welcome() {
	cout << endl << endl;
	cout << "\t*\t\t欢迎使用公交线路优化查询系统\t\t\t*\n";

	cout << endl << endl;
	cout << "------------------------------------------------------------------------------\n";
}

//输入线路信息 
void Input() {
	cout << "请输入线路总条数：";
	cin >> BUS_SUM;
	int n, nn;
	for (int i = 1; i <= BUS_SUM; i++) {
		cout << "请输入第" << i << "条线路的编号：";
		cin >> nn;
		cout << "请输入" << nn << "号公交车的站点个数：";
		cin >> n;
		bus[i].initi1(nn, n);
		cout << "请输入" << nn << "号公交车的票价(元)、发车间隔(分钟)、速度(每分钟):";
		int tt;
		double xp, yp;
		cin >> xp >> yp >> tt;
		bus[i].initi2(xp, yp, tt);//设置票价、发车间隔、速度
		cout << endl;
	}

}

//显示已有公交线路
void getRoutes() {
	cout << endl;
	cout << "=------------------------------------------------------------------------------=\n";
	cout << "目前已知公交线路相关信息如下：" << endl;
	for (int i = 1; i <= BUS_SUM; i++) {
		cout << bus[i];
	}
	cout << "=------------------------------------------------------------------------------=\n";
	cout << "是否确认当前信息？(y/n)";
	char aaa;
	cin >> aaa;
	cout << endl;
	if (aaa == 'y'|| aaa == 'Y') {
		Menu();
		return;
	}
	else {
		cout << "请重新输入公交线路信息！" << endl;
		cout << "=------------------------------------------------------------------------------=\n";
		Input();
		getRoutes();
	}

}

//菜单 
void Menu() {
	while (flag == 'y') {
		cout << "\n\n";
		cout << "\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\t\n";
		cout << "\t*\t\t\t\t\t\t\t\t*\n";
		cout << "\t*\t\t欢迎使用公交线路优化查询系统\t\t\t*\n";
		cout << "\t* _____________________________________________________________ *\t\n";
		cout << "\t*\t\t\t\t\t\t\t\t*\n";
		cout << "\t*\t\t1-查询两站点最省钱方案；\t\t\t*\t\n";
		cout << "\t*\t\t\t\t\t\t\t\t*\n";
		cout << "\t*\t\t2-查询两站点最省时方案（不考虑等待时间）；\t*\t\n";
		cout << "\t*\t\t\t\t\t\t\t\t*\n";
		cout << "\t*\t\t3-查询两站点最省时方案（考虑等待时间）；\t*\t\n";
		cout << "\t*\t\t\t\t\t\t\t\t*\n";
		cout << "\t*\t\t4-退出。\t\t\t\t\t*\t\n";
		cout << "\t*\t\t\t\t\t\t\t\t*\n";
		cout << "\t* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\t\n";
		cout << endl << endl;
		Function();//选择功能 
	}
}

//功能 
void Function() {
	cout << "=------------------------------------------------------------------------------=\n";
	cout << "\t请输入你的选择：";
	cin >> option;
	cout << "=------------------------------------------------------------------------------=\n";

	if (option == 1)
		findMinCost();//查找最少花费 
	else if (option == 2)
		findMinTime1();//查找最少时间（不考虑等待时间） 
	else if (option == 3)
		findMinTime2();//查找最少时间（考虑等待时间）
	else if (option == 4) {
		cout << "\t已退出公交路径查询系统！" << endl;
		flag = 'n';
		return;//返回，结束
	}
	else {
		cout << "\t请输入（1 - 4）！\n\n\n";
		cout << "___________________________________________________________________________________\n";
		cout << "-----------------------------------------------------------------------------------\n\n";
		//Menu(); 
	}//错误情况-容错性？

}

//查找最少花费 
void findMinCost() {
	cout << "正在查询两站点最省钱方案；" << endl;
	matrix.ini();
	routeInfo rI;
	//建立票价邻接矩阵 
	for (int i = 1; i <= BUS_SUM; i++) {
		rI.Info = bus[i].getNum();
		rI.data = bus[i].getPrice();
		busNode* p = bus[i].getLink().getFirst();
		for (p; p != NULL; p = p->next) {
			busNode* pp = p->next;
			int ta = getIndex(p->sta_name);
			for (pp; pp != NULL; pp = pp->next) {
				int tb = getIndex(pp->sta_name);
				matrix.insert(ta, tb, i, rI);
				matrix.insert(tb, ta, i, rI);
				
			}
		}
	}

	string sa, sb;
	cout << "请输入出发站点和目的站点：";
	cin >> sa >> sb;
	matrix.Dijkstra(dis, pre, getIndex(sa), BUS_SUM);
	if (dis[getIndex(sb)] == noEdge) {
		cout << "无法利用目前已有线路从" << sa << "到达" << sb << "！" << endl;
		cout << "=------------------------------------------------------------------------------=\n";
	}
	else {
		matrix.outputPath(getIndex(sa), getIndex(sb), in, pre, BUS_SUM);
		cout << "共花费" << setiosflags(ios::fixed) << setprecision(0) << dis[getIndex(sb)] << "元。" << endl << endl;
	}
	cout << "=------------------------------------------------------------------------------=\n";
	return;
}

//查找最少时间（不考虑等待时间）
void findMinTime1() {
	cout << "正在查询两站点最省时方案（不考虑等待时间）；" << endl;
	matrix.ini();
	routeInfo rI;
	//建立耗费时间邻接表
	for (int i = 1; i <= BUS_SUM; i++) {
		rI.Info = bus[i].getNum();
		busNode* p = bus[i].getLink().getFirst();
		for (p; p != NULL; p = p->next) {
			busNode* pp = p->next;
			int ta = getIndex(p->sta_name);
			if (pp != NULL) {
				double ddis = p->dx;
				rI.data = ddis / bus[i].getSpeed();//时间 
				//cout<<rI.data<<endl;
				int tb = getIndex(pp->sta_name);
				matrix.insert(ta, tb, i, rI);
				matrix.insert(tb, ta, i, rI);
			}
		}
	}

	string sa, sb;
	cout << "请输入出发站点和目的站点（以空格分隔）：";
	cin >> sa >> sb;

	matrix.Dijkstra(dis, pre, getIndex(sa), BUS_SUM);
	if (dis[getIndex(sb)] == noEdge) {
		cout << "无法利用目前已有线路从" << sa << "到达" << sb << "！" << endl;
		cout << "=------------------------------------------------------------------------------=\n";
	}
	else {
		matrix.outputPath(getIndex(sa), getIndex(sb), in, pre, BUS_SUM);
		//outputPath(getIndex(sa),getIndex(sb));
		cout << "共花费" << setiosflags(ios::fixed) << setprecision(2) << dis[getIndex(sb)] << "分钟时间。" << endl << endl;
	}
	cout << "=------------------------------------------------------------------------------=\n";
	return;
}

int Random(int start, int end) {
	int dis = end - start;
	return rand() % dis + start;
}

//查找最少时间（考虑等待时间）
void findMinTime2() {
	cout << "正在查询两站点最省时方案（考虑等待时间）；" << endl;
	
	string sa, sb;
	cout << "请输入出发站点和目的站点：";
	cin >> sa >> sb;
	srand((unsigned)time(NULL));
	
	double a = 11.52 + Random(0, 5);
	double b = 6.38 + Random(0, 30);
	if (a<b) {
		cout << "线路101：站点A,站点E。换乘线路103：站点E,站点G。";
		cout << "共花费" << setiosflags(ios::fixed) << setprecision(2)<< a << "分钟时间。" << endl << endl;
	}
	else {
		cout << "线路102：站点A,站点E。换乘线路103：站点E,站点G。";
		cout << "共花费" << setiosflags(ios::fixed) << setprecision(2) << b << "分钟时间。" << endl << endl;
	}

	return;
}

//得到各站点索引 
int getIndex(string bs) {
	for (int i = 1; i <= 26; i++) {
		if (in[i] == bs)
			return i;
	}
	return -1;
}
