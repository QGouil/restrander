#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "utilities.h"
#include "fuzzyMatch.h"

/*
    gets the complement of an individual character
*/
char
complement(char c)
{
    switch(c) {
        case 'A':
            return 'T';
        case 'T':
            return 'A';
        case 'G':
            return 'C';
        case 'C':
            return 'G';
    }
    return ' ';
}

/*
    gets the complement of an entire string,
    by getting the complement of each character
*/
std::string
complement(std::string * s)
{
    std::string sComplement;
    for (const auto & c : *s) {
        sComplement += complement(c);
    }
    return sComplement;
}

/*
    copies a string, reverses the copy, and returns it
*/
std::string
reverse(std::string * s)
{
    std::string sReverse (*s);
    std::reverse(sReverse.begin(), sReverse.end());
    return sReverse;
}

/*
    gets the reverse complement of an entire string
*/
std::string
reverseComplement(std::string * s) 
{
    std::string sReverse = reverse(s);
    return complement(&sReverse);
}

/*
    checks whether a polyA tail is present in the last countThreshold characters of a sequence
*/
bool
hasPolyATail(std::string * seq, int polyAThreshold, int countThreshold)
{
    int polyA = 0;
    int counted = 0;
    
    for (auto it = (*seq).end(); it != (*seq).begin() && counted < countThreshold; --it) {
        if (*it == 'A') {
            polyA++;
        } else {
            polyA = 0;
        }

        if (polyA >= polyAThreshold) {
            return true;
        }

        counted++;
    }
    return false;
}

/*
    checks whether a polyT tail is present in the first countThreshold characters of a sequence
*/
bool
hasPolyTTail(std::string * seq, int polyTThreshold, int countThreshold)
{
    int polyT = 0;
    int counted = 0;
    int seqLength = (*seq).size();

    for (auto it = (*seq).begin(); it != (*seq).end() && counted < countThreshold; ++it) {
        if (*it == 'T') {
            polyT++;
        } else {
            polyT = 0;
        }

        if (polyT >= polyTThreshold) {
            return true;
        }

        counted++;
    }
    return false;
}

/*
    checks whether a seq has a VNP in the first searchSize characters, 
    with some edit distance tolerance
*/
bool
hasVNP(std::string* seq, int thresholdDist, std::string VNP, int searchSize)
{
    return isFuzzyMatchPresent(seq, &VNP, 200, thresholdDist);
}


/*
    checks whether a seq has an SSP in the first searchSize characters, 
    with some edit distance tolerance
*/
bool
hasSSP(std::string* seq, int thresholdDist, std::string SSP, int searchSize)
{
    return isFuzzyMatchPresent(seq, &SSP, 200, thresholdDist);
}