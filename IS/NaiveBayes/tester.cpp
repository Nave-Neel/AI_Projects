#include "Naive_Bayes.h"

using namespace std;

int main(){
	for(int i=100; i<1000; i+=100){
		Naive_Bayes n_Bayes("/home/naveen/AI_Projects/IS/data/set2", i);
		cout<<n_Bayes.classify()<<endl;
	}
	//Naive_Bayes n_Bayes("/home/naveen/AI_Projects/IS/data/set10", 1000);
	//cout<<n_Bayes.classify()<<endl;
}

// 60, 68, 61, 56, 56, 59, 61, 55, 49, 56    

// 55, 59, 56, 58, 60, 60, 60, 60, 60, 60 

59.37
58.46

0.5001

1
60.4
2
68.2
3
61.6
4
56.7
5
56.5
6
59.0
7
61.2
8
55.0
9
49.6
10
56.4

