#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "relation2D.h"
#include "quadcode.h"
#include "trie.h"
#include "basics.h"
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

void auxFunc(Trie* t, int i1, int i2, int i3, int i4, int i5, int i6) {
    vector<bool> v;
    v.clear();
    v.push_back(i1);
    v.push_back(i2);
    v.push_back(i3);
    v.push_back(i4);
    v.push_back(i5);
    v.push_back(i6);
    t->AddVector(&v);  
}

int main()
{
    //uint wt = mybasics::bits(5);
    /*
    uint wt = 0;
    bitset(&wt, 129);
    uint value = bitget(&wt, 129);
    cout << value << endl;
    */
    //mybasics::bitwrite(wt, 0, 5, 0);

    ofstream outputFile;
    outputFile.open("Test/log_SanMarino.txt");
    parseTXTFile("Test/SanMarino.txt");
    relation2D.SetLogStream(&outputFile);
    relation2D.SetCellSize(0.00001f, 0.00001f);
    relation2D.DetermineArrayLimits();

    Trie relation2DTrie;
    relation2D.FillTrie(&relation2DTrie);
   
    Trie tTrie;

    auxFunc(&tTrie, 0, 0, 1, 0, 1, 0); 
    auxFunc(&tTrie, 0, 0, 1, 1, 0, 1);
    auxFunc(&tTrie, 0, 0, 1, 1, 1, 0);
    auxFunc(&tTrie, 0, 0, 1, 1, 1, 1);
    auxFunc(&tTrie, 0, 1, 1, 0, 1, 1);
    auxFunc(&tTrie, 1, 1, 0, 1, 0, 1); 
    auxFunc(&tTrie, 1, 1, 1, 0, 1, 0);
    auxFunc(&tTrie, 1, 1, 1, 1, 0, 1);
    
    tTrie.CalculateNumberOfLeafsOfEachNode();
    tTrie.BuildPathDecomposition();
  
    outputFile.close();
    
    return 0;
}
	    
