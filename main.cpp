#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "relation2D.h"
#include "quadcode.h"
#include "trie.h"
#include "utils.h"
#include "quadcodestruct.h"
#include <string.h>
using namespace std;

Relation2D relation2D;

void parseTXTFile(string filename)
{
    //cout << filename << endl;
  
    string line;
    ifstream myFile(filename.c_str());

    bool bIsTagLine = true;
    
    if(myFile.is_open())
    {
        while(getline(myFile, line))
	{
            if(bIsTagLine)
            {
                bIsTagLine = false;
                continue;
            }

            istringstream iss(line);
            string token;
            string lat = "";
            string lon = "";
            int parameterNum = 0;
            while(getline(iss, token, '\t'))
            {
                if(parameterNum == 3)
                    lat = token;
                else if(parameterNum == 4)
                    lon = token;

                parameterNum++;
            }

            float latValue = (float)atof(lat.c_str());
            float lonValue = (float)atof(lon.c_str());

           relation2D.AddPair(latValue, lonValue);
	   //cout << latValue << " " << lonValue << endl;
        }
    }

    myFile.close();
}

void auxFunc(Trie* t, int i1, int i2, int i3, int i4) {
    vector<bool> v;
    v.clear();
    v.push_back(i1);
    v.push_back(i2);
    v.push_back(i3);
    v.push_back(i4);
    //v.push_back(i5);
    //v.push_back(i6);
    //v.push_back(i7);
    //v.push_back(i8);
    t->AddVector(&v);
}

void RunCheckPointTest(QuadCodeStructure* structure)
{   
  int x, y;
  BitmapWrapper bitW;
  
  bool bOk = true;
  
  while(cin >> x >> y)
  {
    Utils::CreateQuadCode(x, y, &bitW, structure->quadCodeSize);
 
    bool b1 = structure->CheckBitmap(bitW.bitmap, bitW.len, NULL);
    
    if(!bOk)
    {
      cout << "Error!" << endl;
      
      return;
    }
    
    cout << "(" << x << ", " << y << ") ";
  
    if(b1)
      cout << "Y" << endl;
    else
      cout << "N" << endl;
  }
  
  if(bOk)
  {
    cout << "OK!" << endl;
  }
}

void RunEmptyQueryTest(QuadCodeStructure* structure)
{
  int x1, y1, x2, y2;
  while(cin >> x1 >> y1 >> x2 >> y2)
  {    
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << ": ";
    
    bool bResult = structure->RangeEmptyQuery(Point(x1, y1), Point(x2, y2));
    /*
    int n = 0;
    for(int k = 0; k < (int)relation2D.GetPointVector().size(); k++)
    {
	Point p = relation2D.GetPointVector()[k];
	if(relation2D.GetPointVector()[k].x >= x1 
	  && relation2D.GetPointVector()[k].x <= x2
	  && relation2D.GetPointVector()[k].y >= y1
	  && relation2D.GetPointVector()[k].y <= y2)
	{
	  cout << "(" << p.x << ", " << p.y << ") - ";
	  
	  n++;
	}
    }
    cout << endl;
	
    if(n == 0 && bResult || n > 0 && !bResult)
      cout << "WRONG!" << endl;
    else 
      cout << "OK! => " << ((bResult) ? "Y" : "N") << endl;
    
    cout << endl;
    */
    cout << ((bResult) ? "Y" : "N") << endl;
  }
}

void PrintAllQuadboxes(int x1, int y1, int x2, int y2, int amount)
{
  if(amount <= 0)
    return;
  
  cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
 
  //int a;
  //cin >> a;
  
  if(x1 == x2 && y1 == y2 || x1 > x2 || y1 > y2)
    return;
  
  PrintAllQuadboxes(x1, y1, (x1 + x2) / 2, (y1 + y2) / 2, amount - 4);
  PrintAllQuadboxes((x1 + x2) / 2 + 1, y1, x2, (y1 + y2) / 2, amount - 4);
  PrintAllQuadboxes(x1, (y1 + y2) / 2 + 1, (x1 + x2) / 2, y2, amount - 4);
  PrintAllQuadboxes((x1 + x2) / 2 + 1, (y1 + y2) / 2 + 1, x2, y2, amount - 4);
}

QuadCodeStructure* GetStructureFromBinFile(const char* filename)
{
  QuadCodeStructure* structure = new QuadCodeStructure();
   
  Trie relation2DTrie;
  relation2D.ReadBinaryFile(filename);
  relation2D.FillTriePointsDefined(&relation2DTrie);
  relation2DTrie.CalculateNumberOfLeafsOfEachNode();
  relation2DTrie.BuildPathDecomposition(structure);
  structure->setQuadCodeSize(relation2D.GetQuadCodeSize());  

  return structure;
}

int main(int argc, char** argv)
{
  BitmapWrapper bw;
  Utils::CreateQuadCode(21603134, 2796050, &bw, 52);
  
  /*
  ofstream file("ToyExample.bin", ios::binary);
  int n = 8;
  long m = 6;
  file.write((char*)&n, sizeof(int));
  file.write((char*)&m, sizeof(long));
  
  n = -1; file.write((char*)&n, sizeof(int));
  n = 8;  file.write((char*)&n, sizeof(int));
  n = -2; file.write((char*)&n, sizeof(int));
  n = 2;  file.write((char*)&n, sizeof(int));
  n = 8;  file.write((char*)&n, sizeof(int));
  n = -3; file.write((char*)&n, sizeof(int));
  n = 4;  file.write((char*)&n, sizeof(int));
  n = 8;  file.write((char*)&n, sizeof(int)); 
  n = -4; file.write((char*)&n, sizeof(int));
  n = -5; file.write((char*)&n, sizeof(int));
  n = -6; file.write((char*)&n, sizeof(int));
  n = 3;  file.write((char*)&n, sizeof(int));
  n = -7; file.write((char*)&n, sizeof(int));
  n = -8; file.write((char*)&n, sizeof(int));
  
  file.close();
  */
  if(argc <= 1)
  {
    cout << "--help to view the command list" << endl;
  
    return 0;
  }
  
  if(strcmp(argv[1], "--help") == 0)
  {
    cout << "-rbin filename: Read a binary file containing an adjacency list." << endl << endl;
    cout << "-GNSCountryFileWriteBin datasetfilename filename: Create a binary file with an adjancency list based on a dataset from http://earth-info.nga.mil/gns/html/namefiles.html" << endl <<endl;
    cout << "-CreateTestForCheckPoint binaryfilename number_tests: Create a test cases (random points) for binaryfilename dataset" << endl << endl;
    cout << "-CheckPoint name: receive a pair of ints (x and y) until end of file is reached." << endl << endl;
    cout << "-CreateTestForRangeEmptyQuery binaryfilename number_tests: Create a test cases (random points) for binaryfilename dataset" << endl << endl;
    cout << "-RangeEmptyQuery name: (x1, y1); (x2, y2) until end of file is reached." << endl << endl;
    cout << "-Size binaryfilename: Size of the structure (bitmaps + rank/select)" << endl << endl;
    cout << "-Info binaryfilename: Size of each bitmap, number of 1's and 0's of each bitmap." << endl << endl;
    cout << "-RebuildTreeCheckPoint binaryfilename newbinaryfilename: Read a binary dataset and write it again into newbinaryfilename using CheckPoint operation." << endl << endl;
    cout << "-CreateStructBin binaryfilename name: Create the structure and store it in three binary files name.path, name.nextpath, name.lenpath" << endl << endl;
    return 0;
  }
  
  if(strcmp(argv[1], "-rbin") == 0)
  {
    relation2D.ReadBinaryFile(argv[2]);
    relation2D.PrintPointList();
  
    return 0;
  }
  
  if(strcmp(argv[1], "-GNSCountryFileWriteBin") == 0)
  {
    parseTXTFile(argv[2]);

    relation2D.SetCellSize(0.01f, 0.01f);
    relation2D.DetermineArrayLimits();    
    
    Trie relation2DTrie;
    relation2D.FillTrie(&relation2DTrie);
    relation2D.PreprocessPointListUniqueValue();
    
    relation2D.WriteBinaryFile(argv[3]);
    
    return 0;
  }
  
  if(strcmp(argv[1], "-CreateTestForCheckPoint") == 0)
  {
    relation2D.ReadBinaryFile(argv[2]);
    relation2D.PrintRandomTestCaseCheckPoint(atoi(argv[3]));
    
    return 0;
  }
  
  if(strcmp(argv[1], "-CreateStructBin") == 0)
  {
    QuadCodeStructure* structure = GetStructureFromBinFile(argv[2]);
    
    structure->Save(argv[3]);
    
    delete structure;    
  }
  
  if(strcmp(argv[1], "-LoadStruct") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    
    delete structure;    
  }
 
  if(strcmp(argv[1], "-CheckPoint") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    
    structure->pathBitmap->PrintBitmap(-1); cout << endl << endl;
    
    int len = structure->pathBitmap->len;
    for(int i = 0; i < len; i++)
    {
	if(structure->pathNextBitmap->bitSeq->access(i))
	{
	    cout << "1";
	}
	else
	{
	    cout << "0";
	}
    }
    cout << endl << endl;
    
    for(int i = 0; i < len; i++) 
    {
      if(structure->pathLenBitmap->bitSeq->access(i))
      {
	cout << "1";
      }
      else
      {
	 cout << "0";
      }
    }
    cout << endl << endl;
    RunCheckPointTest(structure);
    
    delete structure;
    
    return 0;
  }
  
  if(strcmp(argv[1], "-RangeEmptyQuery") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    
    structure->Load(argv[2]);
    
    /*
    Trie relation2DTrie;
    relation2D.ReadBinaryFile(argv[2]);
    relation2D.FillTriePointsDefined(&relation2DTrie);
    relation2DTrie.CalculateNumberOfLeafsOfEachNode();
    relation2DTrie.BuildPathDecomposition(structure);
    structure->setQuadCodeSize(relation2D.GetQuadCodeSize());
    */
    RunEmptyQueryTest(structure);
    
    delete structure;   
  }
  
  if(strcmp(argv[1], "-Size") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    cout << structure->GetBytes() << endl;
    delete structure;
    
    return 0;
  }
  
  if(strcmp(argv[1], "-Info") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    
    cout << "Size(bytes): " << structure->GetBytes() << endl << endl;
    cout << "----- Size -----" << endl;
    cout << "Path bitmap size: " << structure->getPathBitmap()->GetSize() << endl;
    cout << "Next bitmap size: " << structure->getPathNextBitmap()->GetSize() << endl;
    cout << "Len bitmap size: " << structure->getPathLenBitmap()->GetSize() << endl << endl;
    cout << "----- Number of (0/1) ------" << endl;
    cout << "Next bitmap: (" << structure->pathNextBitmap->bitSeq->countZeros() << "/" << structure->pathNextBitmap->bitSeq->countOnes() << ")" << endl;
    cout << "Len bitmap: (" << structure->pathLenBitmap->bitSeq->countZeros() << "/" << structure->pathLenBitmap->bitSeq->countOnes() << ")" << endl;
    
    delete structure;
    
    return 0;
  }
  
  if(strcmp(argv[1], "-CreateTestForRangeEmptyQuery") == 0)
  {
    relation2D.ReadBinaryFile(argv[2]);
    int N = 1 << (relation2D.GetQuadCodeSize() / 2);
    //cout << "N: " << N << endl;
    PrintAllQuadboxes(0, 0, N - 1, N - 1, atoi(argv[3]));
  }
  
  if(strcmp(argv[1], "-RebuildTreeCheckPoint") == 0)
  {
      ofstream myFile(argv[3], ios::out | ios::binary);
    
      QuadCodeStructure* structure = new QuadCodeStructure();
      structure->Load(argv[2]);  
      
      int N = 1 << (structure->quadCodeSize / 2);
      long numElements = structure->pathNextBitmap->bitSeq->countOnes() + 1;
      cout << numElements << endl;
      myFile.write((char*)&N, sizeof(int));
      myFile.write((char*)&numElements, sizeof(long));
      
      for(int i = 0; i < N; i++)
      {	
	int nNode = -(i + 1);
	myFile.write((char*)&nNode, sizeof(int));
	for(int j = 0; j < N; j++)
	{
	  int n = j + 1;
	  //myFile.write((char*)&n, sizeof(int));
	  
	  BitmapWrapper bw;
	  Utils::CreateQuadCode(j, i, &bw, structure->quadCodeSize);
	  
	  if(structure->CheckBitmap(bw.bitmap, structure->quadCodeSize, NULL))
	  {
	      myFile.write((char*)&n, sizeof(int));
	      //cout << "(" << j << ", " << i << ") found." << endl;
	  }
	  
	  delete bw.bitmap;
	}
      }
      
      delete structure;
      myFile.close();
  }
  
  //NOT FULLY IMPLEMENTED.
  if(strcmp(argv[1], "-RangeReporting") == 0)
  {
    //QuadCodeStructure* structure = GetStructureFromBinFile(argv[2]);
    
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    
    //RunCheckPointTest(structure);
    
    //delete structure;
    
    /*
    Trie relation2DTrie;
    relation2D.ReadBinaryFile(argv[2]);
    relation2D.FillTriePointsDefined(&relation2DTrie);
    relation2DTrie.CalculateNumberOfLeafsOfEachNode();
    relation2DTrie.BuildPathDecomposition(structure);
    structure->setQuadCodeSize(relation2D.GetQuadCodeSize());
    */
    int x1, y1, x2, y2;
    while(cin >> x1 >> y1 >> x2 >> y2)
    {
      structure->GetPoints(x1, y1, x2, y2);
      structure->PrintPointList();
    }
    //cout << "Range Reporting" << endl;
    //RunEmptyQueryTest(structure);
    
    delete structure;       
  }
  
  /*
    parseTXTFile("Test/SanMarino.txt");
    
    Trie relation2DTrie;
    QuadCodeStructure* structure = new QuadCodeStructure();
    
    relation2D.SetCellSize(0.00001f, 0.00001f);
    relation2D.DetermineArrayLimits();
    relation2D.FillTrie(&relation2DTrie);
    relation2DTrie.CalculateNumberOfLeafsOfEachNode();
    relation2DTrie.BuildPathDecomposition(structure);
    
    structure->setQuadCodeSize(relation2D.GetQuadCodeSize());
    
    //cout << "QuadCode Size: " << relation2D.GetQuadCodeSize() << endl;
    int N = 1 << (relation2D.GetQuadCodeSize() / 2);
    
    relation2D.PreprocessPointListUniqueValue();
    
    relation2D.WriteBinaryFile("SanMarino.bin");
    */
    //relation2D.PrintPointList();
    
    //PrintAllQuadboxes(0, 0, N - 1, N - 1, 1);
    //relation2D.PrintRandomTestCaseCheckPoint(1000);
    //RunCheckPointTest(structure, true);
    //RunEmptyQueryTest(structure);
    
    
    //Trie tTrie;
    //auxFunc(&tTrie, 0, 0, 1, 1);
    //auxFunc(&tTrie, 0, 1, 0, 1);
    //auxFunc(&tTrie, 1, 0, 1, 1);
    //auxFunc(&tTrie, 1, 1, 0, 1);
    
    //structure->setQuadCodeSize(4);
    
    //tTrie.CalculateNumberOfLeafsOfEachNode();
    //tTrie.BuildPathDecomposition(structure);
    
    //structure->getPathBitmap()->PrintBitmap(-1); cout << endl;
    //structure->getPathNextBitmap()->PrintBitmap(-1); cout << endl;
    //structure->getPathLenBitmap()->PrintBitmap(-1); cout << endl;
    //RunEmptyQueryTest(structure);
    
    //cout << structure->getPathNextBitmap()->GetLen() << endl;
    
    //structure->GetPoints(0, 0, 16383, 16383);
    //structure->PrintPointList();
    
    /*
    if(structure->RangeEmptyQuery(Point(0, 0), Point(1, 1)))
    {
      cout << "Y" << endl;
    }
    else
    {
      cout << "N" << endl;
    }
    */
    //delete structure;
    
    return 0;
}

/*
 *  Trie tTrie;

    auxFunc(&tTrie, 0, 0, 0, 1, 1, 0, 0, 1); 
    auxFunc(&tTrie, 0, 1, 0, 0, 0, 1, 0, 1);
    auxFunc(&tTrie, 0, 1, 0, 1, 1, 0, 0, 1);
    //auxFunc(&tTrie, 0, 0, 1, 1, 1, 1, 0, 1);
    auxFunc(&tTrie, 1, 0, 1, 0, 1, 1, 0, 1);
    auxFunc(&tTrie, 1, 0, 1, 1, 0, 1, 0, 1); 
    //auxFunc(&tTrie, 1, 1, 1, 0, 1, 0, 0, 1);
    //auxFunc(&tTrie, 1, 1, 1, 1, 0, 1, 0, 1);
    
    tTrie.CalculateNumberOfLeafsOfEachNode();
    tTrie.BuildPathDecomposition(structure);

    string bitCode = argv[1];
    
    cout << "SIZE(bytes) = " << structure->GetBytes() << endl;
    
    if(structure->CheckBitmap(Utils::CreateBitSequence(bitCode), bitCode.size()))
    {
	cout << "YES" << endl;
    }
    else
    {
	cout << "NO" << endl;
    }
    //int initPos = 32;
    
    //tTrie.getPathBitmap()->PrintBitmap(initPos);
    
    //tTrie.getPathBitmap()->Select(1, 2);
    //tTrie.getPathBitmap()->Select(0, 1);
    //tTrie.getPathBitmap()->Rank(1, 5);
    //tTrie.getPathBitmap()->Rank(0, 3);
    
    //cout << "XOR with " << bitCode << endl;
    
    //tTrie.getPathBitmap()->XOR(Utils::CreateBitSequence(bitCode), initPos, bitCode.size());
    
    
  
    outputFile.close();
 */
	    
