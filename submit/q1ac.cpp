#include <iostream>
#include <unistd.h>

using namespace std; 

int main(){
    int times;
    cin>>times;
    while(times--){
        sleep(2);
        int a, b; 
        cin>>a>>b; 
        cout<<"sum: "<< a+b <<endl;  
    }

}