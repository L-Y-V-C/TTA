#include <iostream>

#include "hh/tta.hh"

int
main(){
    int batchSize = 1500;  // initial batch
    int intervalSec = 1;    // interval between news
    int topK = 10;          // quantity to show in wordcloud
    int lastMNews = 50;    // last M insertions
    int maxWordsPerFile = 5;

    TTA tta(batchSize, intervalSec, topK, lastMNews, maxWordsPerFile);

    tta.initialize("lovins/stopwords.txt", "lovins/AppendixA.txt");
    tta.loadNewsFiles("AP_BBC_CNN");

    string cmd = "start python main.py " + to_string(intervalSec);
    system(cmd.c_str());

    tta.processNews();

    return 0;
}
