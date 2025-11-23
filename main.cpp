#include <iostream>

#include "hh/tta.hh"

int
main(){

    int batchSize;
    int intervalSec;
    int topK;
    int lastMNews;
    int maxWordsPerFile;

    ifstream config("config.txt");
    string par;
    while(config>>par){
        if(par == "batchSize")
            config>>batchSize;
        else if(par == "intervalSec")
            config>>intervalSec;
        else if(par == "topK")
            config>>topK;
        else if(par == "lastMNews")
            config>>lastMNews;
        else if(par == "maxWordsPerFile")
            config>>maxWordsPerFile;
    }
    config.close();

    TTA tta(batchSize, intervalSec, topK, lastMNews, maxWordsPerFile);

    tta.initialize("lovins/stopwords.txt", "lovins/AppendixA.txt");
    cout<<"===============FINISH INITALIZE================\n";
    tta.loadNewsFiles("AP_BBC_CNN");
    cout<<"===============FINISH LOAD NEWS FILES================\n";

    string cmd = "start python main.py " + to_string(intervalSec);
    system(cmd.c_str());

    tta.processNews();

    return 0;
}
