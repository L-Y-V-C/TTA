#ifndef TTA_HH
#define TTA_HH

#include <filesystem>
#include <chrono>
#include <thread>
#include <iomanip>

#include "lovins.hh"
#include "rtree.hh"
#include "mhashtable.hh"
#include "topicdata.hh"

namespace fs = std::filesystem;
using namespace std::chrono;
using namespace std;

class TTA{
public:
    TTA(int batchSize = 100, int intervalSec = 3, int k = 10,
        int m = 25, int maxWords = 5);
    void initialize(const string& stopwordsPath, const string& appendixPath);
    void loadNewsFiles(const string& newsFolder);
    void processNews();
    void insertWord(const string& word, int frequency);
    int getPercentile();
    Mvector<Mpair<string, int>> getTopKTopics();
    void exportToCSV(const Mvector<Mpair<string, int>>& topics, const string& filepath);

    Lovins stemmer;
    RTree rtree;
    MhashTable<string, TopicData> wordHash;
    MhashTable<int, string> idToWord;
    int nextWordId;
    int currentTime;
    int maxFrequency;
    int initialBatchSize;
    int readIntervalSeconds;
    int topK;
    int lastMNews;
    int maxWordsPerFile;
    vector<string> allNewsFiles;
    size_t currentFileIndex;
};

#endif
