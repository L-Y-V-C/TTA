#include <iostream>

#include "hh/lovins.hh"

int main()
{
    Lovins lv;
    lv.readAppendixA();
    lv.readStopwords();

    /*
    for(auto i : lv.appA){
        cout<<i[0].first.length()<<endl;
        for(auto j : i)
            cout<<j.first<<" "<<j.second<<endl;
    }
    for(auto i : lv.stopwords)
        cout<<i<<endl;
    */
    return 0;
}
