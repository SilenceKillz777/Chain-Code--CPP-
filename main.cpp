//Chain Code Algorithm
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class image{
	public:
		
	int numRows, numCols, minVal, maxVal;
	int** imageAry;
	int** CCAry;
		
	//constructor
	image(int numRows, int numCols, int minVal, int maxVal){
			
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
			
		imageAry = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++){
			imageAry[i] = new int[numCols+2];
		}
		
		CCAry = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++){
			CCAry[i] = new int[numCols+2];
		}
		
	}
	
	void zeroFramed(int** imageAry){
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				imageAry[0][j] = 0;
				imageAry[numRows+1][j] = 0;
				imageAry[i][0] = 0;
				imageAry[i][numCols+1] = 0;
			}
		}
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				cout<<imageAry[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	
	void loadImage(int** imageAry, string input){
		ifstream in;
		in.open(input.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;

		for(int i=1;i<numRows+1;i++){
			for(int j=1;j<numCols+1;j++){
				in>>imageAry[i][j];
			}
		}
	}
};

class connectCC{
	public:
		
		
};

int main(int argc, char* argv[]){
	ifstream labelFile (argv[1]);
	string fileName1 = argv[1];
	ifstream propFile (argv[2]);
	string fileName2 = argv[2];
	ofstream outFile1 (argv[3]);
	ofstream outFile2 (argv[4]);
	int numRows, numCols, minVal, maxVal;
	int** imageAry;
	
	if(labelFile.is_open()){
		labelFile>>numRows>>numCols>>minVal>>maxVal;
		outFile1<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		outFile2<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		
		image init(numRows, numCols, minVal, maxVal);
		init.loadImage(imageAry,fileName1);
		init.zeroFramed(imageAry);
		cout<<"Hello";
		//setting up arrays
		
		 /*
		//function calls
		connectedComponents component(numRows, numCols, minVal, maxVal);
		component.loadImage(zeroFramedAry,fileName);
		component.zeroFrame(zeroFramedAry);
		component.ConnectCC_Pass1(zeroFramedAry);
		component.prettyPrint(zeroFramedAry, outFile1, 1);
		component.ConnectCC_Pass2(zeroFramedAry);
		component.prettyPrint(zeroFramedAry, outFile1, 2);
		component.manageEQAry();
		var = component.ConnectCC_Pass3(zeroFramedAry);
		Property CC[var];
		component.loadStruct(CC, zeroFramedAry, var);
		component.printStruct(CC, var, outFile3);
		component.printEQAry(outFile1, "manageEQAry");
		component.prettyPrint(zeroFramedAry, outFile1, 3);
		component.printImage(zeroFramedAry, outFile2);
		*/
	}
	
	else cout<<"Couldn't retrieve data.";
	labelFile.close();
	propFile.close();
	return 0;
}
