#include "KNN.h"

using namespace std;

int main(){
	KNN kNN("/home/naveen/AI_Projects/IS/data", "/home/naveen/AI_Projects/IS/data/set3");
	int best_k = kNN.get_best_k();
	cout<<"Best K:"<<best_k<<endl;
	cout<<"Accuracy on Test:"<<kNN.classify(best_k)<<endl;
	kNN.feature_select(best_k);
	return 0;
}
