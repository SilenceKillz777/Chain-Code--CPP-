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
};

class chainCode{
	public:
	
	class Point{
		public:
		int row;
		int col;
	};
	
	Point neighborCoord[8];
	Point currentP;
	Point nextP;
	int label;
	int numPixels;
	int minRow, maxRow, minCol, maxCol;
	int nextDirTable[8];
	
	chainCode(Property CC[], int component){
		this->label = component;
		this->numPixels = CC[label].numPixels;
		this->minRow = CC[label].minRow;
		this->maxRow = CC[label].maxRow;
		this->minCol = CC[label].minCol;
		this->maxCol = CC[label].maxCol;
		neighborCoord[8];
		
		for(int i=0;i<8;i++){
			nextDirTable[i] = i;	
		}
	}
	
	void loadNeighborCoord(Point currentP){
		int rows = currentP.row;
		int cols = currentP.col;
		neighborCoord[0].row = rows;
		neighborCoord[0].col = cols+1;
		neighborCoord[1].row = rows-1;
		neighborCoord[1].col = cols+1;
		neighborCoord[2].row = rows-1;
		neighborCoord[2].col = cols;
		neighborCoord[3].row = rows-1;
		neighborCoord[3].col = cols-1;
		neighborCoord[4].row = rows;
		neighborCoord[4].col = cols-1;
		neighborCoord[5].row = rows+1;
		neighborCoord[5].col = cols-1;
		neighborCoord[6].row = rows+1;
		neighborCoord[6].col = cols;
		neighborCoord[7].row = rows+1;
		neighborCoord[7].col = cols+1;
	}
	
	int findNextP(Point currentP, int nextQ, Point& nextP, int** CCAry, int label){
		loadNeighborCoord(currentP);
		int chainDir = (nextQ%8);
		int row, col;
		bool status = false;
		for(int i=chainDir;i<(chainDir+8);i++){
			row = neighborCoord[i%8].row;
			col = neighborCoord[i%8].col;
			if(CCAry[row+1][col+1]==label){
				chainDir = (i%8);
				status = true;
			}
			if(status)break;
		}
		nextP.row = neighborCoord[chainDir].row;
		nextP.col = neighborCoord[chainDir].col;
		return chainDir;
	}
	
	void getChainCode(int label, int** CCAry, ofstream& outFile1, ofstream& outFile2){
		int lastQ = 4;
		int nextQ = 0;
		int PchainDir = 0;
		Point startP;
		Point currentP;
		Point nextP; 
		bool status = false;
		for(int iRow=minRow;iRow<maxRow+1;iRow++){
			for(int jCol=minCol;jCol<maxCol+1;jCol++){				
				if(CCAry[iRow+1][jCol+1]>0){
					outFile1<<label<<" "<<iRow<<" "<<jCol<<" ";
					outFile2<<"Label_"<<label<<" StartRow: "<<iRow<<" StartCol: "<<jCol<<endl;;
					startP.row = iRow;
					startP.col = jCol;
					currentP.row = iRow;
					currentP.col = jCol;
					lastQ = 4;
					status = true;	
				}
				if(status)break;
			}
			if(status)break;
		}

		for(int i=(lastQ-1);i<(lastQ+7);i++){
			int row = neighborCoord[i%8].row;
			int col = neighborCoord[i%8].col;
		}

		nextQ = (lastQ-1)%8;
		PchainDir = findNextP(currentP, nextQ, nextP, CCAry, label);
		CCAry[currentP.row][currentP.col] = (0 - (CCAry[currentP.row][currentP.col]));
		outFile1<<PchainDir<<" ";
		outFile2<<PchainDir<<" ";
		lastQ = nextDirTable[PchainDir];
		currentP.row = nextP.row;
		currentP.col = nextP.col;
		int count = 1;
		while (!(currentP.row == startP.row && currentP.col == startP.col)){
			count++;
			nextQ = (lastQ-1+8)%8;
			PchainDir = findNextP(currentP, nextQ, nextP, CCAry, label);
			outFile1<<PchainDir<<" ";
			outFile2<<PchainDir<<" ";
			lastQ = nextDirTable[PchainDir];
			currentP.row = nextP.row;
			currentP.col = nextP.col;
			if(count==20){
				outFile2<<endl;
				count=0;
			}
		}	
		
		outFile1<<endl;
		outFile2<<endl<<endl;
		
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
		init.zeroFramed(imageAry);
		propFile>>numRows>>numCols>>minVal>>maxVal>>totalLabels;
		Property Component[totalLabels];
		connectCC CC(totalLabels,numRows, numCols, minVal, maxVal, Component, fileName2);
		while(label<totalLabels){
			chainCode Chain(Component, label);
			label++;			
			CC.clearCC(CCAry);
			CC.loadCC(CCAry, imageAry, label);
			Chain.getChainCode(label, CCAry, outFile1, outFile2);
		}
	}
	
	else cout<<"Couldn't retrieve data.";
	labelFile.close();
	propFile.close();
	return 0;
}
