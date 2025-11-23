#ifndef TOPICWINDOW_HH
#define TOPICWINDOW_HH

#include <iostream>

#include "topicappearance.hh"
#include "mvector.hh"


using namespace std;

class TopicWindow{
public:
    TopicWindow(int size);
    void recalculateMaxFreq();
    void addAppearance(int topicId, int time, int frequency);
    int getMaxFrequency() const;
    size_t size() const;
    void clear();

    Mvector<TopicAppearance> window;
    int windowSize;
    int currentMaxFreq;
};

#endif
