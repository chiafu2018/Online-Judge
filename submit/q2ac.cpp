#include <iostream>
#include <algorithm>
#include <vector>
typedef long long lol;
int count=0;

using namespace std;

void solve(){
    int n,k;
    cin>>k>>n;
    int ans=1;
    int y=1;

    if(k==n){
        for(int i=1;i<=n;i++){
            cout<<i<<" ";
        }
        return;
    }
    for(int i=k;i>0;i--){
        if(ans<=n){
            cout<<ans<<" ";
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
    cin>>x;
    for(int i=0;i<x;i++){
        solve();
        cout<<endl;
    }
}
