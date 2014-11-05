/********************************************************************
 * Author: Javier González N.
 * 
 * Description: main class, there is an terminal-based interface here. 
 * You can type ./program --help to list all available commands.
 */

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
#include <stdio.h>
using namespace std;

Relation2D relation2D;

/* Time meassuring */
double ticks;
struct tms t1,t2;

void start_clock() {
        times (&t1);
}

double stop_clock() {
        times (&t2);
        return (t2.tms_utime-t1.tms_utime)/ticks;
}
/* end Time meassuring */

//Given a file named "testfile" with the following format...
void RunCheckPointTest(QuadCodeStructure* structure, const char* testfile)
{   
  int n;
  ifstream myFile(testfile);
  
  myFile.read((char*)&n, sizeof(int));
  
  while(n--)
  {
    int x, y;
    myFile.read((char*)&x, sizeof(int));
    myFile.read((char*)&y, sizeof(int));
  }
  
  myFile.close();
}

void RunEmptyQueryTest(QuadCodeStructure* structure)
{
  int x1, y1, x2, y2;
  while(cin >> x1 >> y1 >> x2 >> y2)
  {    
    cout << x1 << " " << y1 << " " << x2 << " " << y2 << ": ";
    
    structure->RangeEmptyQuery(Point(x1, y1), Point(x2, y2));
  }
}

void PrintAllQuadboxes(int x1, int y1, int x2, int y2, int amount)
{
  if(amount <= 0)
    return;
  
  cout << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
 
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
  structure->universeSize = relation2D.N;
 
  return structure;
}

int main(int argc, char** argv)
{
  if(argc <= 1)
  {
    cout << "--help to view the command list" << endl;
  
    return 0;
  }
  
  if(strcmp(argv[1], "--help") == 0)
  {
    cout << "-rbin filename: Read a binary file containing an adjacency list." << endl << endl;
    //cout << "-GNSCountryFileWriteBin datasetfilename filename: Create a binary file with an adjancency list based on a dataset from http://earth-info.nga.mil/gns/html/namefiles.html" << endl <<endl;
    cout << "-CreateTestForCheckPoint binaryfilename number_tests: Create a test cases (random points) for binaryfilename dataset" << endl << endl;
    cout << "-CheckPoint name newfile rep: receive a pair of ints (x and y) until end of file is reached." << endl << endl;
    cout << "-CreateTestForRangeEmptyQuery binaryfilename number_tests: Create a test cases (random points) for binaryfilename dataset" << endl << endl;
    cout << "-RangeEmptyQuery name: (x1, y1); (x2, y2) until end of file is reached." << endl << endl;
    cout << "-Size binaryfilename: Size of the structure (bitmaps + rank/select)" << endl << endl;
    cout << "-Info binaryfilename: Size of each bitmap, number of 1's and 0's of each bitmap." << endl << endl;
    cout << "-RebuildTreeCheckPoint binaryfilename newbinaryfilename: Read a binary dataset and write it again into newbinaryfilename using CheckPoint operation." << endl << endl;
    cout << "-CreateStructBin binaryfilename name: Create the structure and store it in three binary files name.path, name.nextpath, name.lenpath" << endl << endl;
    cout << "-GenerateTrueQueries binaryfilename newfile numqueries" << endl << endl;
    cout << "-GenerateFalseQueries binaryfilename newfile numqueries" << endl << endl;
    cout << "-TimeCheckPoint structfile testfile expectedResult reps" << endl << endl;
    return 0;
  }
  
  if(strcmp(argv[1], "-TimeCheckPoint") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
      FILE * list_fp = fopen(argv[3],"r");
      int expectedResult = atoi(argv[4]);
    int reps = atoi(argv[5]);

    uint queries;
    fread(&queries, sizeof(uint), 1, list_fp);
    uint *qry = (uint *) malloc(sizeof(uint)*queries*2);
    fread(qry, sizeof(uint), queries*2, list_fp);
    fclose(list_fp);

    double t = 0;
    ticks= (double)sysconf(_SC_CLK_TCK);
    start_clock();

    for(int i=0; i<reps; i++) {
        for(int j=0; j<queries; j+=2) {
            if(structure->CheckPoint(Utils::QuadCode(qry[j], qry[j+1]), structure->quadCodeSize) != expectedResult) {
                cout << "Unexpected result" << endl;
                return -1;
            }
        }
    }

    t += stop_clock();
    t *= 1000; // to milliseconds
    cout << "Total time (ms)" << t << endl;

    delete structure;

    return 0;
  }
  
  if(strcmp(argv[1], "-rbin") == 0)
  {
    relation2D.ReadBinaryFile(argv[2]);
    relation2D.PrintPointList();
  
    return 0;
  }
  
  if(strcmp(argv[1], "-GenerateFalseQueries") == 0)
  {
    srand(time(NULL));
    
    relation2D.ReadBinaryFile(argv[2]);
    
    int numQueries = atoi(argv[4]);
    int numPoints = relation2D.points.size();
    int N = relation2D.getN();
    
    ofstream myFile(argv[3]);
    
    myFile.write((char*)&numQueries, sizeof(int));
    
    while(numQueries)
    {
        int pX = rand() % N;
	int pY = rand() % N;
	
	if(!relation2D.IsInRelation(pX, pY))
	{
	  numQueries--;
	  myFile.write((char*)&pX, sizeof(int));
	  myFile.write((char*)&pY, sizeof(int));
	}
    }
    
    myFile.close();
  
    return 0;
  }
  
  if(strcmp(argv[1], "-GenerateTrueQueries") == 0)
  {
    srand(time(NULL));
    
    relation2D.ReadBinaryFile(argv[2]);
    
    int numQueries = atoi(argv[4]);
    int numPoints = relation2D.points.size();
        
    ofstream myFile(argv[3], ios::binary);
    
    myFile.write((char*)&numQueries, sizeof(int));
    
    while(numQueries--)
    {
        int p = rand() % numPoints;
	myFile.write((char*)&relation2D.points[p].x, sizeof(int));
	myFile.write((char*)&relation2D.points[p].y, sizeof(int));
    }
    
    myFile.close();
  
    return 0;
  }
  
  if(strcmp(argv[1], "-GNSCountryFileWriteBin") == 0)
  {
    Utils::ParseTXTFile(argv[2], relation2D);

    float pr = atof(argv[4]);
    relation2D.SetCellSize(pr, pr);
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
  
  if(strcmp(argv[1], "-PrintStructure") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    structure->pathBitmap->PrintBitmap(-1); cout << endl << endl;
    
    delete structure;
  }
 
  if(strcmp(argv[1], "-CheckPoint") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    
    int reps = atoi(argv[4]);
    while(reps--)
      RunCheckPointTest(structure, argv[3]);
    
    delete structure;
    
    return 0;
  }
  
  if(strcmp(argv[1], "-RangeEmptyQuery") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    
    structure->Load(argv[2]);
    
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
      
      int N = structure->universeSize;
      long numElements = structure->pathNextBitmap->bitSeq->countOnes() + 1;
      myFile.write((char*)&N, sizeof(int));
      myFile.write((char*)&numElements, sizeof(long));
      
      int* tmpArr = new int[N];
      
      for(int i = 0; i < N; i++)
      {	
	int nNode = -(i + 1);
	printf("%d/%d\n", -nNode, N);
	
	int cnt = 0;
	tmpArr[cnt++] = nNode;
	for(int j = 0; j < N; j++)
	{
	  int n = j + 1;
	  if(structure->CheckPoint(Utils::QuadCode(j, i), structure->quadCodeSize))
	    tmpArr[cnt++] = n;
	}
	
	myFile.write((char*)tmpArr, sizeof(int) * cnt);
      }
      
      delete structure;
      myFile.close();
  }
  
  if(strcmp(argv[1], "-RangeReporting") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);

    int x1, y1, x2, y2;
    while(cin >> x1 >> y1 >> x2 >> y2)
    {
      structure->GetPoints(x1, y1, x2, y2);
      structure->PrintPointList();
    }

    delete structure;       
  }
  
  if(strcmp(argv[1], "-GetPoints") == 0)
  {
    QuadCodeStructure* structure = new QuadCodeStructure();
    structure->Load(argv[2]);
    
    int n = atoi(argv[3]);
    
    ofstream myFile(argv[4], ios::binary);
    
    vector< pair<int, int> > v = structure->PrintFirstPoints(n);

    myFile.write((char*)&n, sizeof(int));
    
    for(int i = 0; i < v.size(); i++)
    {
      //cout << v[i].first << " " << v[i].second << endl;
	myFile.write((char*)&v[i].first, sizeof(int));
	myFile.write((char*)&v[i].second, sizeof(int));
    }
    
    myFile.close();
    
    delete structure;       
  }
  
  return 0;
}