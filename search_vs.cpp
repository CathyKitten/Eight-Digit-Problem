#include <time.h>��
#include <windows.h>������
#include<queue>
#include<iostream>
#include "math.h" 
#include "stdlib.h"  
#include<stack>
#define num 9
 
using namespace std;

int v;
int nodenum=1;

//����ڵ�ṹ��
struct Node {
	int state[num];   //��������
	struct Node* parent; //���ڵ�
	int value; //ŷʽ����
	int diff; //����λ�Ľ�����
	int depth; //�߼���
	friend bool operator < (Node A, Node B) //����valueֵ+depthֵС�ķ����������ȼ�����
	{
		return A.value + A.depth > B.value + B.depth;//��С���ȶ��� 
	}
};

priority_queue<Node> openTable;     //���ȼ����У�open��
queue<Node> closeTable;     //��ͨ���У�close��
stack<Node> Path;     //����·������ջ�洢 



void getdata(Node& start, Node& end)
{
	cout << "��ѡ�����valueֵ�ķ�ʽ" << endl;
	cout << "1Ϊ�����پ��룬2Ϊ��������3Ϊŷʽ���룺" << endl;
	cin >> v;
	cout << "�������ʼ����" << endl;
	for (int i = 0; i < num; i++)
	{
		cin >> start.state[i];
	}
	cout << "������Ŀ�귽��" << endl;
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

bool exist(Node start, Node end)//���������ж��Ƿ���ڿ��н�
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

int getvalue1(Node node, Node end)// �������پ������valueֵ  
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
	return count + node.depth;   //���ع���ֵ 
}

int getvalue2(Node node, Node end)// �ò���λ�Ľ���������valueֵ  
{
 
	int count = 0;
	for (int i = 0; i < num; i++)
	{
		if (node.state[i] != end.state[i])
		{
			count++;
		}
	}
	return count + node.depth;   //���ع���ֵ 
}

int getvalue3(Node node, Node end)// ��ŷʽ�������valueֵ  
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
	return count + node.depth;   //���ع���ֵ 
}

bool ifsame(Node node1, Node node2)  //�ж����������Ƿ���ȡ� 
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
 
 
void creanode(Node& origin, Node born)//�����½ڵ�
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
	for (int d = 0; d < 4; d++) //�ҵ�S��չ���ӽڵ㣬����open����
	{
		int x1 = x, y1 = y;//��0����
		Node tempnode;
		if (d == 0)  x1 = x - 1;  //���� 
		if (d == 1)	 y1 = y - 1;  //���� 
		if (d == 2) x1 = x + 1;   //���� 
		if (d == 3)	 y1 = y + 1;   //���� 
		int newzreo = x1 * rownum + y1; //0�µ�λ��
		if (x1 >= 0 && x1 < rownum && y1 >= 0 && y1 < rownum) //����ƶ��Ϸ�
		{
			/* �������ӵ�����*/
			tempnode = origin;
			tempnode.state[zeroindex] = origin.state[newzreo];
			tempnode.state[newzreo] = 0;
			if (origin.parent != NULL && (*origin.parent).state[newzreo] == 0) //����½ڵ��үү�ڵ�һ���������ýڵ�
			{
				continue;
			}
			/* ���ӽڵ㶼����open���� */
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
		cout << "û�п��н�";
		return 0;
	}
	openTable.push(start);   //���  ����ʼ�ڵ�ѹ��open�� 
	//starttime = clock();  //��ʼʱ��
	LARGE_INTEGER t1, t2, tc;
	QueryPerformanceFrequency(&tc);
	QueryPerformanceCounter(&t1);
	while (true)
	{
		closeTable.push(openTable.top()); //��open�������ȼ����(����ֵ��С)��Ԫ��ѹ��close����
		openTable.pop(); //�޳�open�������ȼ���ߵ�Ԫ��
		if (!ifsame(closeTable.back(), end)) //�����ǰ�����Ŀ����ֲ���ͬ������չ��ǰ�ڵ㣬
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
	Node tempNode;   //��ʱ�����ݴ��ǰ����
	tempNode = closeTable.back();  //back  ���ض��е����һ��Ԫ�ؼ������ӵ�Ԫ�� 
	while (tempNode.parent != NULL)
	{
		Path.push(tempNode);//��ջ 
		tempNode = *(tempNode.parent);//ָ�򸸽ڵ�
	}
	Path.push(tempNode);
	cout << "����Ҫ�ƶ�" << Path.size() - 1 << "��" << endl;
	/* ������� */
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
	cout << "�����ڵ������"<<nodenum<<endl;
	//cout << "time = " << double(endtime - starttime) / CLOCKS_PER_SEC << "s" << endl;  //���ʱ�䣨��λ����
	cout << "����ʹ�õ�ʱ�䣺 " << time <<" s"<< endl;  //���ʱ�䣨��λ����
	return 0;
}

