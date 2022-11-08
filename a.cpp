#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

// https://atcoder.jp/contests/ahc012/tasks/ahc012_a
// https://img.atcoder.jp/ahc012/f756367b32.html?lang=ja

#define nowTime 1.0 * (clock() - start) / CLOCKS_PER_SEC
#define TEMP 30.0 - 28.0 * nowTime / TIME_LIMIT
#define TIME_LIMIT 2.98
#define D_MAX 10
#define N_MAX 10101
#define K_MAX 100
#define G_MAX 8500

#define K_MIN 0.4
#define K_CHANGE 4

string BIT;

int N, K, _K = 11;
int A[D_MAX];
long X[N_MAX], Y[N_MAX];

int k, _k, __k;
long best_score = -1;
long PX[K_MAX], PY[K_MAX], QX[K_MAX], QY[K_MAX];
long _PX[K_MAX], _PY[K_MAX], _QX[K_MAX], _QY[K_MAX];
long __PX[K_MAX], __PY[K_MAX], __QX[K_MAX], __QY[K_MAX];

// 記録用
long Score[1010101];
long Count = 0;

unsigned long xor128()
{
	static unsigned long __x = 123456789;
	static unsigned long __y = 362436069;
	static unsigned long __z = 521288629;
	static unsigned long __w = 88675123;
	unsigned long __t;
	__t = (__x^(__x<<11));
	__x = __y;
	__y = __z;
	__z = __w;
	return (__w=(__w^(__w>>19))^(__t^(__t>>8)));
}

void record()
{
	__k = k;
	for (int i = 0; i < __k; i++)
	{
		__PX[i] = PX[i];
		__PY[i] = PY[i];
		__QX[i] = QX[i];
		__QY[i] = QY[i];
	}
}

void _input()
{
	cin >> N >> K;
	for (int i = 0; i < D_MAX; i++)
		cin >> A[i];
	for (int i = 0; i < N; i++)
		cin >> X[i] >> Y[i];
}

void initPQ()
{
	for (int i = 0; i < _K; i++)
	{
		PX[i] = 5000;
		PY[i] = G_MAX - 1700*i;
		QX[i] = -5000;
		QY[i] = G_MAX - 1700*i;
	}
}

void makePQ(int i)
{
	long px, py, qx, qy;
	int t = xor128() % 2;
	int s = xor128() % 2;

	if (t == 0)
	{
		px = xor128() % G_MAX;
		if (s % 2 != 0)
			px *= -1;
		py = 5000;
		qx = px;
		qy = -5000;
	}

	PX[i] = xor128() % G_MAX;
	if (s % 2 != 0)
		PX[i] *= -1;
	PY[i] = 5000;
	QX[i] = PX[i];
	QY[i] = -5000;
}

void makek()
{
	k = _K + xor128() % (K - _K);
	k = max(_K + 1, k);
}

void init()
{
	_input();
	K -= 11;

	for (int i = 0; i < K; i++)
		BIT.push_back('0');

	initPQ();
	for (int i = _K; i < K; i++)
		makePQ(i);

	k = K;
}

void escapePQ(int l, int r, bool DO)
{
	if (DO)
	{
		_k = k;
		for (int i = l; i < r; i++)
		{
			_PX[i] = PX[i];
			_PY[i] = PY[i];
			_QX[i] = QX[i];
			_QY[i] = QY[i];
		}
	}

	else
	{
		k = _k;
		for (int i = l; i < r; i++)
		{
			PX[i] = _PX[i];
			PY[i] = _PY[i];
			QX[i] = _QX[i];
			QY[i] = _QY[i];
		}
	}
}

long getScore()
{
	unordered_map<string, int> mp;

	for (int i = 0; i < N; i++)
	{
		string line = BIT; // 左上true, 右下false
		bool ok = true;
		for (int j = 0; j < k; j++)
		{
			long px, py, qx, qy;
			px = PX[j];
			py = PX[j];
			qx = QX[j];
			qy = QY[j];

			if (px == qx)
			{
				if (X[i] < px)
					line[j] = '1';
				if (X[i] == px)
					ok = false;
			}

			else
			{
				if (Y[i] < py)
					line[j] = '1';
				if (Y[i] == py)
					ok = false;
			}
		}

		if (ok)
			mp[line]++;
	}

	long res = 0;
	double up = 0, down = 0;
	vector<int> count(N_MAX, 0);
	for (auto [str, x] : mp)
		count[x]++;
	for (int i = 0; i < D_MAX; i++)
	{
		up += min(A[i], count[i]);
		down += A[i];
	}

	res = 1e6 * up / down;
	return res;
}

long _getScore()
{
	long res = 0;
	return 0;
}

void printLastScore()
{
	k = __k;
	for (int i = 0; i < __k; i++)
	{
		PX[i] = __PX[i];
		PY[i] = __PY[i];
		QX[i] = __QX[i];
		QY[i] = __QY[i];
	}

	cerr << getScore() << '\n';
}

void solve()
{
	clock_t start = clock();
	long now = getScore();

	while (nowTime <= TIME_LIMIT)
	{
		makek();

		int l = _K + xor128() % (k - _K);
		int r = min(l + k/K_CHANGE, k);

		escapePQ(l, r, true);

		for (int i = l; i < r; i++)
			makePQ(i);

		long nex = getScore();

		double temp = TEMP;
		double prob = exp(min(0.0, (nex - now) / temp));

		if (1.0*(xor128()%10000/10000) < prob)
		{
			now = nex;
			Score[Count++] = nex;
			if (nex > best_score)
				record();
		}
		else
			escapePQ(l, r, false);
	}
}

void output()
{
	cout << __k << '\n';
	for (int i = 0; i < __k; i++)
		cout << __PX[i] << ' ' << __PY[i] << ' ' << __QX[i] << ' ' << __QY[i] << '\n';
}

void printGraph()
{
	// to out.txt and graph_show.py
	cout << Count << '\n';
	for (int i = 0; i < Count; i++)
		cout << Score[i] << " \n"[i == Count-1];
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(0);
	init();
	solve();
	output();
	// printLastScore();
	// printGraph();
	return 0;
}