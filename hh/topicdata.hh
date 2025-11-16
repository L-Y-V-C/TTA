#ifndef TOPICDATA_HH
#define TOPICDATA_HH

class TopicData{
public:
    TopicData(): id(-1), frequency(0){}
    TopicData(int inId, int inFrequency) :
        id(inId), frequency(inFrequency){}

    int id;
    int frequency;
};

#endif
