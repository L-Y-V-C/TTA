#include <iostream>

#include "hh/lovins.hh"
#include "hh/mvector.hh"
#include "hh/mhashtable.hh"
#include "hh/topicdata.hh"

int main()
{
    Lovins lv;

    MhashTable<string, TopicData> wordData;
    int nextId = 0;

    string token = "hello";

    if(!wordData.find(token)) {
        wordData[token] = TopicData(nextId, 1);
        nextId++;
    } else {
        wordData[token].frequency++;
    }

    //token = "bye";
    if(!wordData.find(token)) {
        wordData[token] = TopicData(nextId, 1);
        nextId++;
    } else {
        wordData[token].frequency++;
    }

    cout << "Capacity: " << wordData.capacity << endl;
    cout << "Size: " << wordData.length << endl;
    cout << "Load Factor: " << static_cast<double>(wordData.length) / wordData.capacity << endl;

    token = "hello";
    TopicData info = wordData[token];
    int id = info.id;
    int freq = info.frequency;

    cout<<"found:\nid:"<<id<<"\nfreq:"<<freq<<endl;

    /*
    lv.readAppendixA();
    lv.readStopwords();
    // ==================================================
    lv.readNews("AP_test");*/

    /*
    for(auto i : lv.appA){
        cout<<i[0].first.length()<<endl;
        for(auto j : i)
            cout<<j.first<<" "<<j.second<<endl;
    }
    for(auto i : lv.stopwords)
        cout<<i<<endl;
    */
    /*
    for(auto& i : lv.appA){
        //for(auto& j : i.second)
            cout<<i.first<<" "<<i.second<<endl;
    }
    */
    return 0;
}
