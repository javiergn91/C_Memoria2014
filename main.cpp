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
using namespace std;

Relation2D relation2D;

void parseTXTFile(string filename)
{
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
        }
    }

    myFile.close();
}

void auxFunc(Trie* t, int i1, int i2, int i3, int i4, int i5, int i6, int i7, int i8) {
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

void RunCheckPointTest(QuadCodeStructure* structure, bool bCheckResult)
{
  structure->PrintBitmaps();
  
  int x, y;
  BitmapWrapper bitW;
  while(cin >> x >> y)
  {
    /*
    cout << "(" << x << ", " << y << ") ";
    
    Utils::CreateQuadCode(x, y, &bitW, relation2D.GetQuadCodeSize());
    
    for(int i = 0; i < relation2D.GetQuadCodeSize(); i++)
    {
      cout << bitget(bitW.bitmap, i);
    }
    cout << " ";
    */
    Utils::CreateQuadCode(x, y, &bitW, relation2D.GetQuadCodeSize());

      for(int i = 0; i < (int)bitW.len; i++)
      {
	cout << bitget(bitW.bitmap, i);
      }
      cout << " ";
    
    bool b1 = structure->CheckBitmap(bitW.bitmap, bitW.len);
    bool b2 = relation2D.IsInRelation(x, y);
    
    if(b1 != b2 || true)
    {   
      cout << "(" << x << ", " << y << ") ";
      

      
      cout << ((b1) ? 1 : 0) << " " << ((b2) ? 1 : 0) <<  endl;
    }
    
    //cout << "===" << endl;
    /*
    if(structure->CheckBitmap(bitW.bitmap, bitW.len))
    {
      cout << "1 ";
    }
    else
    {
      cout << "0 ";
    }
    
    if(!bCheckResult)
    {
      continue;
    }
    
    if(relation2D.IsInRelation(x, y))
    {
      cout << "1" << endl;
    }
    else
    {
      cout << "0" << endl;
    }
    */
  }
}

int main(int argc, char** argv)
{
    parseTXTFile("Test/SanMarino.txt");
    
    Trie relation2DTrie;
    QuadCodeStructure* structure = new QuadCodeStructure();
    
    relation2D.SetCellSize(0.00001f, 0.00001f);
    relation2D.DetermineArrayLimits();
    relation2D.FillTrie(&relation2DTrie);
    relation2DTrie.CalculateNumberOfLeafsOfEachNode();
    relation2DTrie.BuildPathDecomposition(structure);
    
    relation2D.PrintPointList();
    //relation2D.PrintRandomTestCaseCheckPoint(10);
    RunCheckPointTest(structure, true);
    
    delete structure;
    
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
	    
