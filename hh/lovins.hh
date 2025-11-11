#ifndef LOVINS_HH
#define LOVINS_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <cctype>

using namespace std;

class Lovins{
public:
    Lovins();
    void readAppendixA();
    void readStopwords();

    vector<vector<pair<string, string>>> appA;
    vector<string> stopwords;
};

#endif
