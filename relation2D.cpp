#include "relation2D.h"
#include "math.h"
#include "quadcode.h"

Relation2D::Relation2D()
{
    elementList.clear();
    logStream = NULL;
}

void Relation2D::AddPair(float element1, float element2)
{
    elementList.push_back(make_pair(element1, element2));
}

void Relation2D::SetCellSize(float x, float y)
{
    xCellSize = x;
    yCellSize = y;
}

void Relation2D::DetermineArrayLimits()
{
    cout << "Calculing array limits" << endl;

    if(elementList.size() > 0)
    {
        minDegreeX = elementList[0].first;
        minDegreeY = elementList[0].second;
        maxDegreeX = elementList[0].first;
        maxDegreeY = elementList[0].second;

        for(int i = 1; i < (int)elementList.size(); i++)
        {
            float currX = elementList[i].first;
            float currY = elementList[i].second;

            minDegreeX = min(minDegreeX, currX);
            minDegreeY = min(minDegreeY, currY);
            maxDegreeX = max(maxDegreeX, currX);
            maxDegreeY = max(maxDegreeY, currY);
        }

        xArraySize = (int)ceil((fabs(maxDegreeX - minDegreeX)) / xCellSize);
        yArraySize = (int)ceil((fabs(maxDegreeY - minDegreeY)) / yCellSize);

        *logStream << "Latitude X degree: (min, max) - (" << minDegreeX << ", " << maxDegreeX << ")" << endl;
        *logStream << "Longitude Y degree: (min, max) - (" << minDegreeY << ", " << maxDegreeY << ")" << endl;
        *logStream << "Each matrix position represents " << xCellSize << " degree latitude and " << yCellSize << " degree longitude" << endl;
        *logStream << "Matrix size: x-dimension = " << xArraySize << ", y-dimension = " << yArraySize << endl;
    }
}

void Relation2D::FillTrie(Trie* trie)
{
    vector<bool> quadcode;

    int numBits = 0;
    int maxSize = max(xArraySize, yArraySize);

    while(maxSize)
    {
        numBits++;
        maxSize /= 2;
    }

    *logStream << "Quadcode size: " << numBits << " bits." << endl;

    for(int i = 0; i < (int)elementList.size(); i++)
    {
        int xPos = (int)ceil((elementList[i].first - minDegreeX) / xCellSize);
        int yPos = (int)ceil((elementList[i].second - minDegreeY) / yCellSize);;

        *logStream << "Point (" << elementList[i].first << ", " << elementList[i].second << ") set at position (" << xPos << ", " << yPos << ") -> QuadCode: ";

        QuadCode::CreateQuadCode(numBits, &quadcode, xPos, yPos);
        /*
        for(int i = 0; i < (int)quadcode.size(); i++)
        {
            *logStream << quadcode[i];
        }

        *logStream << " (" << Utils::GetDecimalRepresentation(&quadcode) << ")";
        */
        trie->AddVector(&quadcode);

        //*logStream << endl;
    }

    trie->CalculateNumberOfLeafsOfEachNode();
}

void Relation2D::SetLogStream(ofstream* newStream)
{
    logStream = newStream;
}
