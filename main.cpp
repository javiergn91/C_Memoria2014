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

int main(int argc, char** argv)
{
    ofstream outputFile;
    outputFile.open("Test/log_SanMarino.txt");
    parseTXTFile("Test/SanMarino.txt");
    relation2D.SetLogStream(&outputFile);
    relation2D.SetCellSize(0.00001f, 0.00001f);
    relation2D.DetermineArrayLimits();
  
    Trie relation2DTrie;
    relation2D.FillTrie(&relation2DTrie);
   
    Trie tTrie;

    auxFunc(&tTrie, 0, 0, 0, 1, 1, 0, 0, 1); 
    auxFunc(&tTrie, 0, 1, 0, 0, 0, 1, 0, 1);
    auxFunc(&tTrie, 0, 1, 0, 1, 1, 0, 0, 1);
    //auxFunc(&tTrie, 0, 0, 1, 1, 1, 1, 0, 1);
    auxFunc(&tTrie, 1, 0, 1, 0, 1, 1, 0, 1);
    auxFunc(&tTrie, 1, 0, 1, 1, 0, 1, 0, 1); 
    //auxFunc(&tTrie, 1, 1, 1, 0, 1, 0, 0, 1);
    //auxFunc(&tTrie, 1, 1, 1, 1, 0, 1, 0, 1);
    
    tTrie.CalculateNumberOfLeafsOfEachNode();
    tTrie.BuildPathDecomposition();

    string bitCode = argv[1];
    
    if(tTrie.CheckBitmap(Utils::CreateBitSequence(bitCode), bitCode.size()))
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
    
    return 0;
}
	    
