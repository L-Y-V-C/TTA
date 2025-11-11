#include "../hh/lovins.hh"

Lovins::Lovins(){
    appA.resize(11);
}

void Lovins::readAppendixA(){
    ifstream file("lovins/AppendixA.txt");
    if(!file.is_open()){
        cerr<<"ERROR OPENING APPENDIX A\n";
        return;
    }
    string line1;
    stringstream line2;
    int idx;
    while(getline(file, line1)){
        if(isdigit((line1[0]))){
            idx = 11 - stoi(line1);
            continue;
        }
        line2 = (stringstream)line1;
        string word, cond;
        while(line2>>word>>cond){
            appA[idx].push_back({word, cond});
        }
    }
    file.close();
}

void Lovins::readStopwords(){
    ifstream file("lovins/stopwords.txt");
    if(!file.is_open()){
        cerr<<"ERROR OPENING STOPWORDS\n";
        return;
    }
    string line;
    while(getline(file, line)){
        stopwords.push_back(line);
    }
    file.close();
}
