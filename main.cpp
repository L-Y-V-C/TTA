#include <iostream>

#include "hh/lovins.hh"
#include "hh/mhashtable.hh"
#include "hh/topicdata.hh"
#include "hh/rtree.hh"

#

int main()
{
    Lovins lv;

    RTree rtree;

    rtree.printTree();

    MhashTable<string, TopicData> wordHash;
    int nextId = 0;
    int currentTime = 0;

    // Procesar palabras
    vector<string> words = {"hello", "world", "hello", "code", "hello", "world"};

    for (size_t i = 0; i < words.size(); i++) {
        string word = words[i];

        if (!wordHash.find(word)) {
            // Nueva palabra
            wordHash[word] = TopicData(nextId, 1);
            rtree.insertPoint(nextId, currentTime, 1);
            nextId++;
        } else {
            // Palabra existente: incrementar frecuencia
            wordHash[word].frequency++;
            int id = wordHash[word].id;
            int freq = wordHash[word].frequency;
            rtree.insertPoint(id, currentTime, freq);
        }

        currentTime++;
    }

    rtree.printStats();
    rtree.printTree();

    cout << "\n\nCapacity: " << wordHash.capacity << endl;
    cout << "Size: " << wordHash.length << endl;
    cout << "Load Factor: " << static_cast<double>(wordHash.length) / wordHash.capacity << endl;


/*
    lv.readAppendixA();
    lv.readStopwords();
    // ==================================================
    lv.readNews("AP_test");
*/
    return 0;
}
