#ifndef LOVINS_HH
#define LOVINS_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <cctype>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

namespace fs = std::filesystem;
using namespace std;

class Lovins{
public:
    Lovins();
    void readAppendixA();
    void readStopwords();
    void readNews(const string& folderName);
    string aplyStemming(const string& token);
    bool evaluateCondition(const string& cond, const string& stem);
    string recoding(const string& inStem);

    unordered_map<string, string> appA;
    unordered_set<string> stopwords;
};

#endif
