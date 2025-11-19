#include <iostream>

#include "hh/tta.hh"

int
main(){
    int batchSize = 20;  // initial batch
    int intervalSec = 1;    // interval between news
    int topK = 30;          // quantity to show in wordcloud
    int lastMNews = 50;    // last M insertions
    int maxWordsPerFile = 3;

    TTA tta(batchSize, intervalSec, topK, lastMNews, maxWordsPerFile);

    tta.initialize("lovins/stopwords.txt", "lovins/AppendixA.txt");
    cout<<"===============FINISH INITALIZE================\n";
    tta.loadNewsFiles("AP_BBC_CNN");
    cout<<"===============FINISH LOAD NEWS FILES================\n";

    //string cmd = "start python main.py " + to_string(intervalSec);
    //system(cmd.c_str());

    tta.processNews();

    return 0;
}
