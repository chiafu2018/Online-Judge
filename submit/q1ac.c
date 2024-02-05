#include<stdio.h>

int main(){
	int times;
	scanf("%d", &times);
	while(times--){
		int a, b;
		scanf("%d %d", &a, &b);
		printf("sum: %d\n", a+b);
	}

	return 0;
}

