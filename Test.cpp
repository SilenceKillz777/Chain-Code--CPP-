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
		}/*
		for(int i=1;i<numRows+1;i++){
			for(int j=1;j<numCols+1;j++){
				cout<<CCAry[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<endl;*/
	}

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
			in>>CC[i].minCol;
			in>>CC[i].maxRow;
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
	int neighborCoord[];
	int nextDirTable[2][8];
	
	chainCode(Property CC[], int component){
		this->label = component;
		this->numPixels = CC[label].numPixels;
		this->minRow = CC[label].minRow;
		this->maxRow = CC[label].maxRow;
		this->minCol = CC[label].minCol;
		this->maxCol = CC[label].maxCol;
		neighborCoord[8];
		for(int i=0;i<8;i++){
			neighborCoord[i] = 0;
		}
		nextDirTable[0][0] = 0;
		nextDirTable[1][0] = 1;
		nextDirTable[0][1] = -1;
		nextDirTable[1][1] = 1;
		nextDirTable[0][2] = -1;
		nextDirTable[1][2] = 0;
		nextDirTable[0][3] = -1;
		nextDirTable[1][3] = -1;
		nextDirTable[0][4] = 0;
		nextDirTable[1][4] = -1;
		nextDirTable[0][5] = 1;
		nextDirTable[1][5] = -1;
		nextDirTable[0][6] = 1;
		nextDirTable[1][6] = 0;
		nextDirTable[0][7] = 1;
		nextDirTable[1][7] = 1;
	}
	
	void loadNeighborCoord(int currR, int currC, int** CCAry){
		int index = 0;
		for(int row=currR-1;row<currR+2;row++){
			for(int col=currC-1;col<currC+2;col++){
				if(!(row==currR && col==currC)){
					//cout<<CCAry[row][col];
					neighborCoord[index] = CCAry[row][col];
					index++;
				}
			}
		}
	}
	
	int findNextP(int currR, int currC, int nextQ, int** CCAry, int label, int &nextP){
		loadNeighborCoord(currR, currC, CCAry);
		int chainDir = (nextQ%8);
		bool status = false;
		for(int i=chainDir;i<chainDir+8;i++){
			if(neighborCoord[i%8]==label){
				chainDir = (i%8);
				status = true;
				break;
			}
			if(status)break;
		}
		nextP = neighborCoord[chainDir];
		/*
		for(int i=0;i<8;i++){
			cout<<neighborCoord[i]<<" ";
		}*/
		return chainDir;
	}
	
	void getChainCode(int label, int** CCAry, ofstream& outFile1, ofstream& outFile2){
		int startR = 0;
		int startC = 0;
		int currR = 0;
		int currC = 0;
		int lastQ = 4;
		int nextQ = 0;
		int chainDir = 0;
		int nextP = 0;
		bool status = false;
		for(int iRow=minRow;iRow<maxRow+1;iRow++){
			for(int jCol=minCol;jCol<maxCol+1;jCol++){				
				if(CCAry[iRow+1][jCol+1]>0){
					outFile1<<iRow<<" "<<jCol<<" "<<CCAry[iRow+1][jCol+1]<<" ";
					outFile2<<"Label "<<label<<": ";
					startR = iRow+1;
					startC = jCol+1;
					currR = iRow+1;
					currC = jCol+1;
					nextQ = (lastQ+1)%8;
					status = true;
					break;
				}
			}
			if(status)break;
		}
		cout<<"StartR: "<<startR<<endl;
		cout<<"StartC: "<<startC<<endl;
		cout<<"currR: "<<currR<<endl;
		cout<<"currC: "<<currC<<endl;
		cout<<"lastQ: "<<lastQ<<endl;
		cout<<"nextQ: "<<nextQ<<endl;
		chainDir = findNextP(currR, currC, nextQ, CCAry, label, nextP);
		cout<<"nextP: "<<nextP<<endl;
		CCAry[currR][currC] = (0 - (CCAry[currR][currC]));
		outFile1<<chainDir<<endl;
		outFile2<<chainDir<<endl;
		for(int i=0;i<2;i++){
			for(int j=0;j<8;j++){
				cout<<nextDirTable[i][j]<<" ";
			}
			cout<<endl;
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
			Chain.getChainCode(label, CCAry, outFile1, outFile2);
			cout<<endl;
		}
	}
	
	else cout<<"Couldn't retrieve data.";
	labelFile.close();
	propFile.close();
	return 0;
}
