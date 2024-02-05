#include <stdio.h>
typedef long long lol;
#define max(a,b) (((a) > (b)) ? (a) : (b))
int count=0;

//using namespace std;

void solve(){
    int n,k;
    //cin>>k>>n;
	scanf("%d %d",&k,&n);
    int ans=1;
    int y=1;

    if(k==n){
        for(int i=1;i<=n;i++){
            //cout<<i<<" ";
			printf("%d ",i);
        }
        return;
    }
    for(int i=k;i>0;i--){
        if(ans<=n){
            //cout<<ans<<" ";
			printf("%d ",ans);
            if(ans+y<=n&&i-2<=max(0,n-(ans+y))){
                ans+=y;
                y++;
            }else{
                ans+=1;
            }
        }else{
            ans+=1;
        }
    }
}

int main()
{
    int x;
    //cin>>x;
	scanf("%d",&x);
    for(int i=0;i<x;i++){
        solve();
        //cout<<endl;
		printf("\n");
    }
}