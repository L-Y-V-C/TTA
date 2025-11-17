#include "../hh/tta.hh"

TTA::TTA(int batchSize, int intervalSec, int k, int m, int maxWords) :
    nextWordId(0), currentTime(0), maxFrequency(0),
    initialBatchSize(batchSize), readIntervalSeconds(intervalSec),
    topK(k), lastMNews(m), maxWordsPerFile(maxWords), currentFileIndex(0){}

void TTA::initialize(const string& stopwordsPath, const string& appendixPath){
    stemmer.readStopwords();
    stemmer.readAppendixA();
}

void TTA::loadNewsFiles(const string& newsFolder){
    if(!fs::exists(newsFolder)){
        cerr<<"ERROR OPENING DIRECTORY\n";
        return;
    }
    for(const auto& entry : fs::directory_iterator(newsFolder)){
        if(entry.is_regular_file())
            allNewsFiles.push_back(entry.path().string());
    }
}

void TTA::insertWord(const string& word, int frequency){
    if(!wordHash.find(word)){
        wordHash[word] = TopicData(nextWordId, frequency);
        idToWord[nextWordId] = word;
        rtree.insertPoint(nextWordId, currentTime, frequency);
        nextWordId++;
    }else{
        int wordId = wordHash[word].id;
        wordHash[word].frequency += frequency;
        int newFreq = wordHash[word].frequency;
        rtree.insertPoint(wordId, currentTime, newFreq);
    }
    if(wordHash[word].frequency > maxFrequency)
        maxFrequency = wordHash[word].frequency;
}

void TTA::processNews() {
    if(allNewsFiles.empty()){
        cerr<<"ERROR: NO NEWS\n";
        return;
    }
    cout<<"\n========== INITIAL BATCH PROCESING ==========\n";
    cout<<"files to process: "<<initialBatchSize<<endl;
    auto batchStartTime = high_resolution_clock::now();
    auto stemmingTotalTime = duration<double>::zero();
    auto insertionTotalTime = duration<double>::zero();
    // INITIAL D
    for(size_t i = 0; i < initialBatchSize && i < allNewsFiles.size(); i++){
        // STEMMING
        auto stemmingStart = high_resolution_clock::now();
        vector<pair<string, int>> topWords =
            stemmer.readFile(allNewsFiles[i], maxWordsPerFile);
        auto stemmingEnd = high_resolution_clock::now();
        stemmingTotalTime += duration_cast<duration<double>>(stemmingEnd - stemmingStart);
        // INSERTION
        auto insertionStart = high_resolution_clock::now();
        for(const auto& wordPair : topWords)
            insertWord(wordPair.first, wordPair.second);
        currentTime++;
        auto insertionEnd = high_resolution_clock::now();
        insertionTotalTime += duration_cast<duration<double>>(insertionEnd - insertionStart);

        currentFileIndex = i + 1;
    }
    auto batchEndTime = high_resolution_clock::now();
    auto batchTotalTime = duration_cast<duration<double>>(batchEndTime - batchStartTime);
    cout<<"\n--- INITIAL BATCH RESULT ---\n"
        <<fixed<<setprecision(6)
        <<"total stemming time: "<<stemmingTotalTime.count()<<" s\n"
        <<"total insertion time: "<<insertionTotalTime.count()<<" s\n"
        <<"total batch time: "<<batchTotalTime.count()<<" s\n"
        <<"average per file: "<<(batchTotalTime.count() / initialBatchSize)<<" s\n"
        <<"actual time(fig): "<<currentTime<<" - maximun frequency: "<<maxFrequency<<"\n";

    // INITIAL EXPORT
    auto queryStart = high_resolution_clock::now();
    Mvector<Mpair<string, int>> topTopics = getTopKTopics();
    exportToCSV(topTopics, "output/trendingTopics.csv");
    auto queryEnd = high_resolution_clock::now();
    auto queryTime = duration_cast<duration<double>>(queryEnd - queryStart);
    cout<<"initial query time: "<<queryTime.count()<<" s\n";

    cout<<"\n========== CONTINUOS PROCESING ==========\n"
        <<"read 1 file per "<<readIntervalSeconds<<" secs\n\n";

    // CONTINUOS
    for(size_t i = currentFileIndex; i < allNewsFiles.size(); i++){
        this_thread::sleep_for(chrono::seconds(readIntervalSeconds));
        cout<<"\n[new "<<(i + 1)<<"/"<<allNewsFiles.size()<<"] "
            <<allNewsFiles[i]<<endl;
        // STEMMING
        auto stemmingStart = high_resolution_clock::now();
        vector<pair<string, int>> topWords =
            stemmer.readFile(allNewsFiles[i], maxWordsPerFile);
        auto stemmingEnd = high_resolution_clock::now();
        auto stemmingTime = duration_cast<duration<double>>(stemmingEnd - stemmingStart);
        // INSERTION
        auto insertionStart = high_resolution_clock::now();
        for(const auto& wordPair : topWords)
            insertWord(wordPair.first, wordPair.second);
        currentTime++;
        auto insertionEnd = high_resolution_clock::now();
        auto insertionTime = duration_cast<duration<double>>(insertionEnd - insertionStart);
        // QUERY
        auto queryStart = high_resolution_clock::now();
        Mvector<Mpair<string, int>> topTopics = getTopKTopics();
        exportToCSV(topTopics, "output/trendingTopics.csv");
        auto queryEnd = high_resolution_clock::now();
        auto queryTime = duration_cast<duration<double>>(queryEnd - queryStart);
        cout<<"stemming: "<<stemmingTime.count()<<" s | "
            <<"insertion: "<<insertionTime.count()<<" s | "
            <<"query: "<<queryTime.count()<<" s\n"
            <<"actual time(fig): "<<currentTime<<" - maximum frequency: "<<maxFrequency
            <<" | top topics: "<<topTopics.size()<<endl;
        currentFileIndex = i + 1;
    }
    cout<<"\n========== PROCESS FINISHED ==========\n"
        <<"processed news: "<<currentFileIndex<<endl;
}

int TTA::getPercentile(){
    return maxFrequency / 4;
}

Mvector<Mpair<string, int>> TTA::getTopKTopics() {
    int minTime = currentTime - lastMNews;
    int minFreq = getPercentile();

    Rect query(minTime, minFreq, currentTime, maxFrequency);
    Mvector<Entry> results;
    rtree.search(query.minX, query.minY, query.maxX, query.maxY, results);

    MhashTable<int, int> topicMaxFreq;
    for(size_t i = 0; i < results.size(); i++){
        int tid = results[i].topicId;
        int freq = results[i].y;
        if(!topicMaxFreq.find(tid))
            topicMaxFreq[tid] = freq;
        else if(freq > topicMaxFreq[tid])
            topicMaxFreq[tid] = freq;
    }

    Mvector<Mpair<string, int>> topKResult;
    for(size_t i = 0; i < topicMaxFreq.capacity; i++){
        if (topicMaxFreq.table[i].used) {
            int wordId = topicMaxFreq.table[i].key;
            int freq = topicMaxFreq.table[i].value;
            string word = idToWord[wordId];
            topKResult.push_back(Mpair<string, int>(word, freq));
            if (topKResult.size() >= topK)
                break;
        }
    }
    return topKResult;
}

void TTA::exportToCSV(const Mvector<Mpair<string, int>>& topics,
                      const string& filepath){
    ofstream file(filepath);
    if(!file.is_open()){
        cerr<<"ERROR MAKING FILE\n";
        return;
    }
    file<<"topic,frequency\n";
    for(size_t i = 0; i < topics.size(); i++)
        file<<topics[i].first<<","<<topics[i].second<<"\n";
    file.close();
}
