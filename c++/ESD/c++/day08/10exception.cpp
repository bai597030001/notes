#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;
void foo(){
	cout << "app start()" << endl;
	int  r=rand()%4;
	if(0==r){
	    cout << "程序正常" << endl;
	}else if(1==r){
	    throw 1; 
	}else if(2==r){
	    throw 2.4;
	}else {
	    throw "程序出错 r==3"; 
	}
    cout << "app end" << endl;
}
int main(){
    srand(time(NULL));
	foo();
}

