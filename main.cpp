#include<String>
#include<iostream>
using namespace std;

int main (){


	return 0;
}

bool CompareStrings (string str1, string str2){
	int points = 0;
	for(int i = 0; i < str1.length(); i++){
		if(str1[i] == str2[i]){
			points++;
		}
		else{
			points--;
		}
	}
	if(points == str1.length()){
		return true;
	}
	else{
		return false;
	}
}
