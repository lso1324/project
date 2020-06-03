
#include <iostream>
#include <stack>
#include <queue>
#include <fstream>
#include <ctime>
#define MAXSIZE 100
using namespace std;

char Lmap[MAXSIZE][MAXSIZE], Smap[MAXSIZE][MAXSIZE];
bool LX[MAXSIZE * MAXSIZE], SX[MAXSIZE * MAXSIZE];
int Lpath[MAXSIZE * MAXSIZE], Spath[MAXSIZE * MAXSIZE];
int dx[4] = { -1,1,0,0 }, dy[4] = { 0,0,-1,1 };
int Size, id_;


int dir(int next, bool visit[], int target) {
	int dis = INT_MAX;
	int tx = target / Size, ty = target % Size, nx = next / Size, ny = next % Size;

	for (int i = 0; i < 4; i++) {
		int x = nx + dx[i], y = ny + dy[i];
		int id = x * Size + y;
		if ((x >= 0 && x < Size && y >= 0 && y < Size) && visit[id] == false && SX[id] == false) {
			if (dis > abs(tx - x) + abs(ty - y)) {
				dis = abs(tx - x) + abs(ty - y);
				id_ = id;
			}
		}
	}
	return dis;
}

int condir(int curr, bool visit[], int des) {
	int x = curr / Size, y = curr % Size;
	bool flag = true;
	int id;

	if (dir(curr, visit, des) <= abs(des / Size - curr / Size) + abs(des % Size - curr % Size)) {
		flag = false;
		id = id_;
	}
	if (flag)
		return -1;
	else
		return id;
}

bool ImprovedSoukup(int x1, int y1, int x2, int y2) {
	bool visit[MAXSIZE * MAXSIZE];
	memset(visit, false, (Size * Size) + 1);

	int src = x1 * Size + y1;
	int des = x2 * Size + y2;

	stack<int> s;
	queue<int> q;

	s.push(src);
	Spath[src] = -1;
	visit[src] = true;

	int cnt = 0;

	while (!s.empty()) {
		int pid = s.top();

		if (pid == des) {
			return true;
		}

		if (dir(pid, visit, des) <= abs(des / Size - pid / Size) + abs(des % Size - pid % Size)) {
			int id = id_;
	
			s.push(id);
			visit[id] = true;
			Spath[id] = pid;
			if (id == des) {
				return true;
			}

			while (condir(id, visit, des) >= 0) {
				int new_id = condir(id, visit, des);
				s.push(new_id);
				visit[new_id] = true;
				Spath[new_id] = id;
				if (new_id == des) {
					return true;
				}
				id = new_id;
			}
		}
		while (!s.empty()) {
			pid = s.top();
			int tx = pid / Size, ty = pid % Size;
			for (int i = 0; i < 4; i++) {
				int x = tx + dx[i], y = ty + dy[i];
				int id = x * Size + y;
				if ((x >= 0 && x < Size && y >= 0 && y < Size) && !visit[id] && !SX[id]) {
					
					q.push(id);
					visit[id] = true;
					Spath[id] = pid;
				}
			}

			s.pop();
		}
		while (!q.empty()) {
			s.push(q.front());
			q.pop();
		}
	}
	return false;
}

bool Lee_Algorithm(int x1, int y1, int x2, int y2) {
	bool visit[MAXSIZE * MAXSIZE];
	memset(visit, false, (Size * Size) + 1);

	int src = x1 * Size + y1;
	int des = x2 * Size + y2;
	queue<int> q;
	q.push(src);
	Lpath[src] = -1;

	while (!q.empty()) {
		int p = q.front();
		visit[p] = true;
		int x = p / Size, y = p % Size;

		for (int i = 0; i < 4; i++) {
			int tx = x + dx[i], ty = y + dy[i];
			int id = tx * Size + ty;

			if ((tx >= 0 && tx < Size && ty >= 0 && ty < Size) && !visit[id] && !LX[id]) {
				if (id == des) {
					Lpath[id] = p;
					return true;
				}
				q.push(id);
				Lpath[id] = p;
			}
		}
		q.pop();
	}
	return false;
}

int main()
{
	int numOfX;
	pair<int, int> start, dest, X; //Coordination

	ifstream fin("input.txt");
	ofstream Lout("lee.txt");
	ofstream Sout("imporvedsoukup.txt");

	fin >> Size;
	fin >> start.first >> start.second;
	fin >> dest.first >> dest.second;
	fin >> numOfX;


	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) {
			Lmap[i][j] = '.';
			Smap[i][j] = '.';
		}
	}
	memset(LX, false, (Size * Size) + 1);
	memset(SX, false, (Size * Size) + 1);

	for (int i = 0; i < numOfX; i++) {
		fin >> X.first >> X.second;
		Lmap[X.first][X.second] = Smap[X.first][X.second] = 'X';
		LX[X.first * Size + X.second] = SX[X.first * Size + X.second] = true;
	}

	Lmap[start.first][start.second] = 'S';
	Lmap[dest.first][dest.second] = 'T';
	Smap[start.first][start.second] = 'S';
	Smap[dest.first][dest.second] = 'T';

	time_t s1, s2, s3, s4;



	Lout << "Lee Algorithm" << endl;	
	Lout << endl;

	s1 = clock();

	if (Lee_Algorithm(start.first, start.second, dest.first, dest.second)) {    //find destination and return Lpath;
		int id = dest.first * Size + dest.second;
		LX[id] = true;
		LX[start.first * Size + start.second] = true;

		while (Lpath[id] != -1) {
			Lmap[Lpath[id] / Size][Lpath[id] % Size] = '*';
			LX[Lpath[id]] = true;
			id = Lpath[id];
		}
		Lmap[start.first][start.second] = 'S';
	
	}
	else      
		Lout << "No route" << endl;
	s2 = clock();

	double Lee_exectime = (double)(s2 - s1);


	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) Lout << Lmap[i][j] << " ";
		Lout << endl;
	}
	Lout << endl;
	Lout << "Execution Time: " << Lee_exectime << "ms" << endl;
	
	Sout << "Improved Soukup Algorithm " << endl;
	Sout << endl;

	s3 = clock();

	if (ImprovedSoukup(start.first, start.second, dest.first, dest.second)) {    //find destination and return Lpath;

		int id = dest.first * Size + dest.second;
		SX[id] = true;
		SX[start.first * Size + start.second] = true;
		while (Spath[id] != -1) {
			Smap[Spath[id] / Size][Spath[id] % Size] = '*';
			SX[Spath[id]] = true;
			id = Spath[id];
		}
		Smap[start.first][start.second] = 'S';
	
	}
	else
		Sout << "No route" << endl;
	s4 = clock();

	double MS_exectime = (double)(s4 - s3);


	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < Size; j++) Sout << Smap[i][j] << " ";
		Sout << endl;
	}
	Sout << endl;
	Sout << "Execution Time: " << MS_exectime << "ms" << endl;

	return 0;

}