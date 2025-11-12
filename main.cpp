#include <iostream>

#include "hh/lovins.hh"
#include "hh/mvector.hh"

int main()
{
    Lovins lv;
    /*
    lv.readAppendixA();
    lv.readStopwords();
    // ==================================================
    lv.readNews("AP_test");*/

    Mvector<int> v1;
    Mvector<int> v2(4);
    Mvector<int> v3(v2);

    v1.push_back(2);
    v1.push_back(3);
    v1.pop_back();
    v2.reserve(10);
    v3.clear();
    v1.erase(1);
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
