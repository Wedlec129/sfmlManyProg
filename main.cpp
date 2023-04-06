#include <iostream>
#include <cmath>
using namespace std;

int main() {
    
    int n;
    cin>>n;
    
    int s=0;
    
    if(abs(n)<10000)
    for(int i=1; i<=abs(n); i++){
        
        s+=i;
        
    }
    
    
    if(s==0)s=1;
    
    cout<<s;

    return 0;
}

    

