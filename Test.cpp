//Chain Code Algorithm
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

struct Property{
	int label;
	int numPixels;
	int minRow, minCol, maxRow, maxCol;
};

class image{
	public:
		
	int numRows, numCols, minVal, maxVal;

	//constructors
	image(int numRows, int numCols, int minVal, int maxVal){
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
	}
	
	image(int numRows, int numCols, int minVal, int maxVal, int** imageAry, int** CCAry){
			
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
			
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				imageAry[i][j] = 0;
			}
		}
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				CCAry[i][j] = 0;
			}
		}
	}
	
	//methods
	void zeroFramed(int** imageAry){
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2;j++){
				imageAry[0][j] = 0;
				imageAry[numRows+1][j] = 0;
				imageAry[i][0] = 0;
				imageAry[i][numCols+1] = 0;
			}
		}
	}
	
	void loadImage(string input, int** imageAry){
		ifstream in;
		in.open(input.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;

		for(int i=1;i<numRows+1;i++){
			for(int j=1;j<numCols+1;j++){
				in>>imageAry[i][j];
			}
		}
	}
	
	void clearCC(int** CCAry){
		for(int rows=0;rows<numRows+2;rows++){
			for(int cols=0;cols<numCols+2;cols++){
				CCAry[rows][cols] = 0;
			}
		}
	}
	
	void loadCC(int** CCAry, int** imageAry, int label){
		for(int i=1;i<numRows+1;i++){
			for(int j=1;j<numCols+1;j++){
				if(imageAry[i][j]==label)
					CCAry[i][j] = imageAry[i][j];
			}
		}
	}
	/*
	void getChainCode(int** CCAry){
		for(int i=1;i<numRows+1;i++){
			for(int j=1;j<numCols+1;j++){
				cout<<CCAry[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;
	}*/
};

class connectCC{
	public:
	
	int label,totalLabels;
	int numPixels;
	int minRow, minCol, maxRow, maxCol;
	int numRows, numCols, minVal, maxVal;

	connectCC(int totalLabels, int numRows, int numCols, int minVal, int maxVal, Property CC[], string propFile){
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
		label = 0;
		image Image(numRows, numCols, minVal, maxVal);
		ifstream in;
		in.open(propFile.c_str());
		in>>this->numRows>>this->numCols>>this->minVal>>this->maxVal;
		in>>this->totalLabels;
		for(int i=0;i<totalLabels;i++){
			in>>CC[i].label;
			in>>CC[i].numPixels;
			in>>CC[i].minRow;
			in>>CC[i].maxRow;
			in>>CC[i].minCol;
			in>>CC[i].maxCol;
		}
	}
	
	void clearCC(int** CCAry){
		image Image(numRows, numCols, minVal, maxVal);
		Image.clearCC(CCAry);
	}
	
	void loadCC(int** CCAry, int** imageAry, int label){
		image Image(numRows, numCols, minVal, maxVal);		
		Image.loadCC(CCAry, imageAry, label);
	}
	/*
	void getChainCode(int** CCAry){
		image Image(numRows, numCols, minVal, maxVal);			
		Image.getChainCode(CCAry);
	}
	*/
};

class chainCode{
	public:
	
	int label;
	int numPixels;
	int minRow, maxRow, minCol, maxCol;
	
	chainCode(Property CC[], int component){
		this->label = component;
		this->numPixels = CC[label].numPixels;
		this->minRow = CC[label].minRow;
		this->maxRow = CC[label].maxRow;
		this->minCol = CC[label].minCol;
		this->maxCol = CC[label].maxCol;
	}
	
	getChainCode(int label, int** CCAry){
		for(int i=minRow;i<maxRow;i++){
			for(int j=minCol;j<maxCol;j++){
				
			}
		}
	}
	
};

int main(int argc, char* argv[]){
	ifstream labelFile (argv[1]);
	string fileName1 = argv[1];
	ifstream propFile (argv[2]);
	string fileName2 = argv[2];
	ofstream outFile1 (argv[3]);
	ofstream outFile2 (argv[4]);
	int numRows, numCols, minVal, maxVal, totalLabels, label = 0;
	int** imageAry;
	int** CCAry;
	
	if(labelFile.is_open()){
		labelFile>>numRows>>numCols>>minVal>>maxVal;
		outFile1<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		outFile2<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		
		imageAry = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++){
			imageAry[i] = new int[numCols+2];
		}
		
		CCAry = new int*[numRows+2];
		for(int i = 0;i < numRows+2; i++){
			CCAry[i] = new int[numCols+2];
		}
		
		image init(numRows, numCols, minVal, maxVal, imageAry, CCAry);
		init.loadImage(fileName1,imageAry);	
			
		for(int i=0;i<numRows+2;i++){
			for(int j=0;j<numCols+2; j++){
				cout<<imageAry[i][j];
			}
			cout<<endl;
		}
		init.zeroFramed(imageAry);
		propFile>>numRows>>numCols>>minVal>>maxVal>>totalLabels;
		cout<<"Total Number of Components: "<<totalLabels<<endl<<endl;
		Property Component[totalLabels];
		connectCC CC(totalLabels,numRows, numCols, minVal, maxVal, Component, fileName2);
		while(label<totalLabels){
			chainCode Chain(Component, label);
			label++;
			cout<<"This is component #"<<label<<endl;
			
			CC.clearCC(CCAry);
			CC.loadCC(CCAry, imageAry, label);
			Chain.getChainCode(label, CCAry);
		}
	}
	
	else cout<<"Couldn't retrieve data.";
	labelFile.close();
	propFile.close();
	return 0;
}
