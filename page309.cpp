#include <iostream>
#include "datas.h"
using namespace std;
void sort1(vector<int>& datas) {
	for (int i = 0; i+1 < datas.size();i++) {
		for (int j = i + 1; j < datas.size(); j++) {
			if (datas[i] > datas[j])swap(datas[i], datas[j]);
		}
	}
}
void sort2(vector<int>& datas) {
	int N = datas.size();
	int min = datas[0];
	for (int i = 0; i < N; i++) {
		for (int j = i; j < N; j++) {
			if (datas[j] < min)min = datas[j];
		}
		datas[i] = min;
	}
}
void sort3(vector<int>& datas,int lo,int hi) {
	int j = lo, k = hi;
	if (lo >= hi)return;
	int temp = datas[lo];
	while (lo < hi) {
		while (datas[lo] < temp && lo < hi) { lo++; }
		while (datas[hi] > temp && lo < hi) { hi--; }
		swap(datas[lo], datas[hi]);
	}
	sort3(datas, j, lo-1);
	sort3(datas, hi+1, k);

}
//void sort4(vector<int>& datas,int lo,int hi) {
//	if (lo >= hi)return;
//	int key = datas[lo];
//	int i = lo, j = hi;
//	while (lo < hi) {
//		while (datas[lo] < key && lo < hi) { lo++; }
//		while (datas[hi] > key && lo < hi) { hi--; }
//		swap(datas[hi], datas[lo]);
//	}
//	sort4(datas,i, lo - 1)
//	sort4(datas,hi + 1, j);
//}
//快排
int main() {
	auto datas = creatVector(10);
	sort3(datas,0,datas.size()-1);
	for (auto i : datas)cout << i << endl;
}