#ifndef TOPICAPPEARANCE_HH
#define TOPICAPPEARANCE_HH

class TopicAppearance{
public:
    TopicAppearance() :
        topicId(-1), time(0), frequency(0){}
    TopicAppearance(int id, int t, int f) :
        topicId(id), time(t), frequency(f){}

    int topicId;
    int time;
    int frequency;
};

#endif
