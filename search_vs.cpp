#include <time.h>　
#include <windows.h>　　　
#include<queue>
#include<iostream>
#include "math.h" 
#include "stdlib.h"  
#include<stack>
#define num 9
 
using namespace std;

int v;
int nodenum=1;

//定义节点结构体
struct Node {
	int state[num];   //方阵数组
	struct Node* parent; //父节点
	int value; //欧式距离
	int diff; //不在位的将牌数
	int depth; //走几步
	friend bool operator < (Node A, Node B) //按照value值+depth值小的方案构造优先级队列
	{
		return A.value + A.depth > B.value + B.depth;//最小优先队列 
	}
};

priority_queue<Node> openTable;     //优先级队列，open表
queue<Node> closeTable;     //普通队列，close表
stack<Node> Path;     //最终路径，用栈存储 



void getdata(Node& start, Node& end)
{
	cout << "请选择计算value值的方式" << endl;
	cout << "1为曼哈顿距离，2为将牌数，3为欧式距离：" << endl;
	cin >> v;
	cout << "请输入初始方阵：" << endl;
	for (int i = 0; i < num; i++)
	{
		cin >> start.state[i];
	}
	cout << "请输入目标方阵：" << endl;
	for (int i = 0; i < num; i++)
	{
		cin >> end.state[i];
	}
	start.parent = NULL;
	start.depth = 0;
	start.value = 0;
	end.parent = NULL;
	end.depth = 0;
	end.value = 0;
}

bool exist(Node start, Node end)//用逆序数判断是否存在可行解
{
	int n1 = 0;
	int n2 = 0;
	int r1, r2;
	for (int i = 0; i < num - 1; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (start.state[i] != 0 && start.state[j] != 0 && start.state[i] > start.state[j])
				n1++;
		}
	}
	for (int i = 0; i < num - 1; i++)
	{
		for (int j = i + 1; j < num; j++)
		{
			if (end.state[i] != 0 && end.state[j] != 0 && end.state[i] > end.state[j])
				n2++;
		}
	}
	r1 = n1 % 2;
	r2 = n2 % 2;
	if (r1 == r2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int** CreaMatrix(int m, int n)  
{
	int** a = new int* [m];
	for (int i = 0; i < m; i++)
	{
		a[i] = new int[n];
	}
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i][j] = 0;
		}
	}
	return a;
}

int getvalue1(Node node, Node end)// 用曼哈顿距离计算value值  
{
	int rownum = sqrt(num);
	int count = 0;
	int** node1;
	int** end1;
	node1 = CreaMatrix(rownum, rownum);
	end1 = CreaMatrix(rownum, rownum);
	for (int i = 0; i < num; i++) {
		node1[i / rownum][i % rownum] = node.state[i];
		end1[i / rownum][i % rownum] = end.state[i];
	}
	for (int i = 0; i < rownum; i++)
		for (int j = 0; j < rownum; j++)
		{
			if (node1[i][j] != end1[i][j])
			{
				for (int k = 0; k < rownum; k++)
					for (int w = 0; w < rownum; w++)
						if (node1[i][j] == end1[k][w])
							count = count + abs(i - k) + abs(j - w);
			}
			else {
				continue;
			}
		}
	return count + node.depth;   //返回估计值 
}

int getvalue2(Node node, Node end)// 用不在位的将牌数计算value值  
{
 
	int count = 0;
	for (int i = 0; i < num; i++)
	{
		if (node.state[i] != end.state[i])
		{
			count++;
		}
	}
	return count + node.depth;   //返回估计值 
}

int getvalue3(Node node, Node end)// 用欧式距离计算value值  
{
	int rownum = sqrt(num);
	int count = 0;
	int** node1;
	int** end1;
	node1 = CreaMatrix(rownum, rownum);
	end1 = CreaMatrix(rownum, rownum);
	for (int i = 0; i < num; i++) {
		node1[i / rownum][i % rownum] = node.state[i];
		end1[i / rownum][i % rownum] = end.state[i];
	}
	for (int i = 0; i < rownum; i++)
		for (int j = 0; j < rownum; j++)
		{
			if (node1[i][j] != end1[i][j])
			{
				for (int k = 0; k < rownum; k++)
					for (int w = 0; w < rownum; w++)
						if (node1[i][j] == end1[k][w])
							count = count + sqrt(abs(i - k)* abs(i - k) + abs(j - w)* abs(j - w));
			}
			else {
				continue;
			}
		}
	return count + node.depth;   //返回估计值 
}

bool ifsame(Node node1, Node node2)  //判断两个方阵是否相等。 
{
	for (int i = 0; i <= num-1; i++)
	{
		if (node1.state[i] != node2.state[i])
		{
			return false;
		}
	}
	return true;
}
 
 
void creanode(Node& origin, Node born)//产生新节点
{
	int rownum = sqrt(num);
	int zeroindex=0; 
	for (int i = 0; i < num ; i++)
	{
		if (origin.state[i] == 0)
		{
			zeroindex = i;
			break;
		}
	}

	int x = zeroindex / rownum, y = zeroindex % rownum;
	for (int d = 0; d < 4; d++) //找到S扩展的子节点，加入open表中
	{
		int x1 = x, y1 = y;//新0坐标
		Node tempnode;
		if (d == 0)  x1 = x - 1;  //向左 
		if (d == 1)	 y1 = y - 1;  //向下 
		if (d == 2) x1 = x + 1;   //向右 
		if (d == 3)	 y1 = y + 1;   //向上 
		int newzreo = x1 * rownum + y1; //0新的位置
		if (x1 >= 0 && x1 < rownum && y1 >= 0 && y1 < rownum) //如果移动合法
		{
			/* 交换格子的内容*/
			tempnode = origin;
			tempnode.state[zeroindex] = origin.state[newzreo];
			tempnode.state[newzreo] = 0;
			if (origin.parent != NULL && (*origin.parent).state[newzreo] == 0) //如果新节点和爷爷节点一样，舍弃该节点
			{
				continue;
			}
			/* 把子节点都加入open表中 */
			tempnode.parent = &origin;
			if (v == 1)
			{
				tempnode.value = getvalue1(tempnode, born);
			}
			else if (v == 2)
			{
				tempnode.value = getvalue2(tempnode, born);
			}
			else
			{
				tempnode.value = getvalue3(tempnode, born);
			}
			tempnode.depth = origin.depth + 1;
			openTable.push(tempnode);
			nodenum++;
		}
	}
}

int main()
{
	//clock_t starttime, endtime;
	int rownum = sqrt(num);
	Node start, end;   
	getdata(start, end);
	if (!exist(start, end))
	{
		cout << "没有可行解";
		return 0;
	}
	openTable.push(start);   //入队  将初始节点压入open表 
	//starttime = clock();  //开始时间
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	while (true)
	{
		closeTable.push(openTable.top()); //将open表中优先级最高(估计值最小)的元素压入close表中
		openTable.pop(); //剔除open表中优先级最高的元素
		if (!ifsame(closeTable.back(), end)) //如果当前棋局与目标棋局不相同，则拓展当前节点，
		{
			creanode(closeTable.back(), end);
		}
		else
		{
			break;
		}
	}

	QueryPerformanceCounter(&t2);
	double time = (double)(t2.QuadPart - t1.QuadPart) / (double)tc.QuadPart;
	//endtime = clock();  
	Node tempNode;   //临时变量暂存队前数据
	tempNode = closeTable.back();  //back  返回队列的最后一个元素即最后入队的元素 
	while (tempNode.parent != NULL)
	{
		Path.push(tempNode);//入栈 
		tempNode = *(tempNode.parent);//指向父节点
	}
	Path.push(tempNode);
	cout << "至少要移动" << Path.size() - 1 << "步" << endl;
	/* 输出方案 */
	while (Path.size() != 0)
	{
		for (int i = 0; i <= num-1; i++)
		{
			cout << Path.top().state[i] << " ";
			if ((i + 1) % rownum == 0)
				cout << endl;
		}
		Path.pop();
		cout << "-------" << endl;
		cout << "\n";
	}
	cout << "搜索节点个数："<<nodenum<<endl;
	//cout << "time = " << double(endtime - starttime) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
	cout << "搜索使用的时间： " << time <<" s"<< endl;  //输出时间（单位：ｓ）
	return 0;
}

