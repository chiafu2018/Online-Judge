#include <iostream>
#include <algorithm>
#include <vector>


using namespace std;

void solve() {
	int n;
	cin >> n;
	vector<int> ind(n);
	vector<int> res(n);
	for (int i = 0; i < n; i++) {
		cin >> ind[i];
	}
	int tmp = ind[0];
	res[0] = ind[0];
	for (int i = 1; i < n; i++) {
		if (tmp - ind[i] >= 0 &&ind[i]!=0) {
			cout << "-1";
			return;
		}
		else {
			res[i] = res[i - 1] + ind[i];
			tmp += ind[i];
		}
	}
	for (auto x : res)
		cout << x << " ";
}

int main() {
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		solve();
		cout << endl;
	}
}
