#include <stdio.h>
 
 
 
void solve() {
	int n;
	//cin >> n;
    scanf("%d",&n);
	//vector<int> ind(n);
	//vector<int> res(n);
    int ind[120];
    int res[120];
	for (int i = 0; i < n; i++) {
		//cin >> ind[i];
        scanf("%d",&ind[i]);
	}
	int tmp = ind[0];
	res[0] = ind[0];
	for (int i = 1; i < n; i++) {
		if (tmp - ind[i] >= 0 &&ind[i]!=0) {
			//cout << "-1";
            printf("-1");
			return;
		}
		else {
			res[i] = res[i - 1] + ind[i];
			tmp += ind[i];
		}
	}
	for (int i=0;i<n;i++){
		//cout << x << "  ";
        printf("%d ",res[i]);
    }
}
 
int main() {
	int t;
	//cin >> t;
    scanf("%d",&t);
	for (int i = 0; i < t; i++) {
		solve();
		//cout << endl;
        printf("\n");
	}
}