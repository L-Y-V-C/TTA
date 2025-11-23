#include "../hh/topicwindow.hh"

TopicWindow::TopicWindow(int size) :
    windowSize(size), currentMaxFreq(0){
    window.reserve(size + 10);
}

void TopicWindow::recalculateMaxFreq(){
    currentMaxFreq = 0;
    for(size_t i = 0; i < window.size(); i++){
        if(window[i].frequency > currentMaxFreq)
            currentMaxFreq = window[i].frequency;
    }
    //cout<<"TW new maxFreq: "<<currentMaxFreq
    //    <<" size: "<<window.size()<<endl;
}

void TopicWindow::addAppearance(int topicId, int time, int frequency){
    if(window.size() >= (size_t)windowSize){
        int removedFreq = window[0].frequency;
        //cout<<"TW -- out oldest "<<" time: "<<window[0].time<<
        //    " freq: "<<removedFreq<<"\n";
        window.erase(0);
        if(removedFreq == currentMaxFreq){
            bool stillExists = false;
            for(size_t i = 0; i < window.size(); i++){
                if(window[i].frequency == currentMaxFreq){
                    stillExists = true;
                    break;
                }
            }
            if(!stillExists){
                //cout<<"TW -- maxFreq "<<currentMaxFreq<<" erased\n";
                recalculateMaxFreq();
            }
        }
    }
    window.push_back(TopicAppearance(topicId, time, frequency));
    if(frequency > currentMaxFreq)
        currentMaxFreq = frequency;
}

int TopicWindow::getMaxFrequency() const{
    return currentMaxFreq;
}

size_t TopicWindow::size() const{
    return window.size();
}

void TopicWindow::clear(){
    window.clear();
    currentMaxFreq = 0;
}
