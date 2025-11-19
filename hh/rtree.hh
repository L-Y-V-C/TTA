#ifndef RTREE_HH
#define RTREE_HH

#include "pool.hh"
#include "mpair.hh"
#include "distribution.hh"

#include <limits>
#include <cmath>

using namespace std;

class RTree{
public:
    RTree();
    Rect boundingRect(const Mvector<Entry>& entries);
    Rect getNodeMBR(int nodeIndex);
    int chooseSubtree(int nodeIndex, const Rect& rect);
    Mpair<int, int> pickSeeds(const Mvector<Entry>& entries);
    int pickNext(const Mvector<Entry>& remaining,
                 const Mvector<Entry>& group1,
                 const Mvector<Entry>& group2);
    int splitNode(int nodeIndex);
    void adjustTree(Mvector<int>& path, Mvector<int>& indices,
                    int node1Index, int node2Index);
    void insertEntry(int startIndex, const Entry& entry);
    void searchRect(int nodeIndex, const Rect& query,
                   Mvector<Entry>& result);
    void insertPoint(int topicId, int time, int frequency);
    void search(int minTime, int minFreq, int maxTime, int maxFreq,
                Mvector<Entry>& result);
    int calculateOverlap(const Rect& rect, const Mvector<Entry>& entries,
                         int excludeIdx);
    Mvector<Distribution> generateDistributions(Mvector<Entry>& entries,
                                                bool sortByLower);
    Mpair<int, int> chooseSplitAxisAndIndex(const Mvector<Entry>& entries);
    void reinsert(int nodeIndex, int level);
    void printStats();
    void printTree();
    void printNode(int nodeIndex, int depth);

    Pool pool;
    int rootIndex;
    Mvector<bool> reinsertedAtLevel;
};

#endif
