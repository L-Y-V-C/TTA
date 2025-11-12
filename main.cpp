#include <iostream>

#include "hh/lovins.hh"

int main()
{
    Lovins lv;
    lv.readAppendixA();
    lv.readStopwords();
    // ==================================================
    lv.readNews("AP_test");
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
