#include "../hh/rtree.hh"


RTree::RTree(){
    rootIndex = pool.assignNode(true);
}

Rect RTree::boundingRect(const Mvector<Entry>& entries){
    if(entries.empty())
        return Rect();
    int minX = numeric_limits<int>::max();
    int minY = numeric_limits<int>::max();
    int maxX = numeric_limits<int>::min();
    int maxY = numeric_limits<int>::min();
    for(size_t i = 0; i < entries.size(); i++){
        const Entry& e = entries[i];
        minX = min(minX, e.rect.minX);
        minY = min(minY, e.rect.minY);
        maxX = max(maxX, e.rect.maxX);
        maxY = max(maxY, e.rect.maxY);
    }
    return Rect(minX, minY, maxX, maxY);
}

Rect RTree::getNodeMBR(int nodeIndex){
    Node& node = pool[nodeIndex];
    return boundingRect(node.entries);
}

int RTree::chooseSubtree(int nodeIndex, const Rect& rect){
    Node& node = pool[nodeIndex];
    int bestIdx = 0;
    int minEnlargement = numeric_limits<int>::max();
    int minArea = numeric_limits<int>::max();
    for(size_t i = 0; i < node.entries.size(); i++){
        Rect combined = node.entries[i].rect.combine(rect);
        int enlargement = combined.area() - node.entries[i].rect.area();
        if(enlargement < minEnlargement ||
            (enlargement == minEnlargement && node.entries[i].rect.area() < minArea)){
            minEnlargement = enlargement;
            minArea = node.entries[i].rect.area();
            bestIdx = i;
        }
    }
    return bestIdx;
}

// ========== SPLIT NODE (Quadratic) ==========

Mpair<int, int> RTree::pickSeeds(const Mvector<Entry>& entries){
    int maxWaste = -1;
    Mpair<int, int> seeds(0, 1);
    for(size_t i = 0; i < entries.size(); i++){
        for(size_t j = i + 1; j < entries.size(); j++){
            Rect combined = entries[i].rect.combine(entries[j].rect);
            int waste = combined.area() - entries[i].rect.area() - entries[j].rect.area();
            if(waste > maxWaste){
                maxWaste = waste;
                seeds = Mpair<int, int>(i, j);
            }
        }
    }
    return seeds;
}

int RTree::pickNext(const Mvector<Entry>& remaining,
             const Mvector<Entry>& group1,
             const Mvector<Entry>& group2){
    Rect r1 = boundingRect(group1);
    Rect r2 = boundingRect(group2);
    int maxDiff = -1;
    int best = 0;
    for(size_t i = 0; i < remaining.size(); i++){
        int d1 = r1.combine(remaining[i].rect).area() - r1.area();
        int d2 = r2.combine(remaining[i].rect).area() - r2.area();
        int diff = abs(d1 - d2);
        if(diff > maxDiff){
            maxDiff = diff;
            best = i;
        }
    }
    return best;
}

int RTree::splitNode(int nodeIndex){
    Node& node = pool[nodeIndex];
    Mvector<Entry> allEntries = node.entries;
    node.entries.clear();
    int siblingIndex = pool.assignNode(node.isLeaf);
    Node& sibling = pool[siblingIndex];
    Mpair<int, int> seeds = pickSeeds(allEntries);
    node.entries.push_back(allEntries[seeds.first]);
    sibling.entries.push_back(allEntries[seeds.second]);
    Mvector<Entry> remaining;
    for(size_t i = 0; i < allEntries.size(); i++){
        if (i != (size_t)seeds.first && i != (size_t)seeds.second)
            remaining.push_back(allEntries[i]);
    }
    while(!remaining.empty()){
        if(node.entries.size() + remaining.size() == MIN_ENTRIES){
            for(size_t i = 0; i < remaining.size(); i++)
                node.entries.push_back(remaining[i]);
            break;
        }
        if(sibling.entries.size() + remaining.size() == MIN_ENTRIES){
            for(size_t i = 0; i < remaining.size(); i++)
                sibling.entries.push_back(remaining[i]);
            break;
        }
        int next = pickNext(remaining, node.entries, sibling.entries);
        Entry e = remaining[next];
        remaining.erase(next);
        Rect r1 = boundingRect(node.entries);
        Rect r2 = boundingRect(sibling.entries);
        int enlarge1 = r1.combine(e.rect).area() - r1.area();
        int enlarge2 = r2.combine(e.rect).area() - r2.area();
        if(enlarge1 < enlarge2)
            node.entries.push_back(e);
        else if(enlarge2 < enlarge1)
            sibling.entries.push_back(e);
        else{
            int area1 = r1.area();
            int area2 = r2.area();
            if(area1 < area2)
                node.entries.push_back(e);
            else if(area2 < area1)
                sibling.entries.push_back(e);
            else{
                if(node.entries.size() <= sibling.entries.size())
                    node.entries.push_back(e);
                else
                    sibling.entries.push_back(e);
            }
        }
    }
    return siblingIndex;
}

void RTree::adjustTree(Mvector<int>& path, Mvector<int>& indices,
                int node1Index, int node2Index){
    int i = path.size() - 1;
    int child1 = node1Index;
    int child2 = node2Index;
    while(i >= 0){
        int parentIndex = path[i];
        int entryIdx = indices[i];
        Node& parent = pool[parentIndex];
        parent.entries[entryIdx].rect = getNodeMBR(child1);
        parent.entries[entryIdx].childIdx = child1;
        if(child2 != -1){
            Entry newEntry(getNodeMBR(child2), child2);
            parent.entries.push_back(newEntry);
            if(parent.entries.size() > MAX_ENTRIES){
                child2 = splitNode(parentIndex);
                child1 = parentIndex;
            }else
                child2 = -1;
        }
        i--;
    }
    if(child2 != -1){
        int oldRoot = rootIndex;
        rootIndex = pool.assignNode(false);
        Node& newRoot = pool[rootIndex];
        Entry e1(getNodeMBR(oldRoot), oldRoot);
        Entry e2(getNodeMBR(child2), child2);
        newRoot.entries.push_back(e1);
        newRoot.entries.push_back(e2);
    }
}

void RTree::insertEntry(int startIndex, const Entry& entry){
    Mvector<int> path;
    Mvector<int> indices;
    int currentIndex = startIndex;
    Node* current = &pool[currentIndex];
    while(!current->isLeaf){
        int idx = chooseSubtree(currentIndex, entry.rect);
        path.push_back(currentIndex);
        indices.push_back(idx);
        currentIndex = current->entries[idx].childIdx;
        current = &pool[currentIndex];
    }
    current->entries.push_back(entry);
    if(current->entries.size() > MAX_ENTRIES){
        int siblingIndex = splitNode(currentIndex);
        adjustTree(path, indices, currentIndex, siblingIndex);
    }else
        adjustTree(path, indices, currentIndex, -1);
}

void RTree::searchRect(int nodeIndex, const Rect& query, Mvector<Entry>& result){
    Node& node = pool[nodeIndex];
    for(size_t i = 0; i < node.entries.size(); i++){
        if(node.entries[i].rect.overlaps(query)){
            if(node.isLeaf)
                result.push_back(node.entries[i]);
            else
                searchRect(node.entries[i].childIdx, query, result);
        }
    }
}


void RTree::insertPoint(int topicId, int time, int frequency){
    Entry e(time, frequency, topicId);
    insertEntry(rootIndex, e);
}

void RTree::search(int minTime, int minFreq, int maxTime, int maxFreq,
            Mvector<Entry>& result){
    Rect query(minTime, minFreq, maxTime, maxFreq);
    searchRect(rootIndex, query, result);
}

void RTree::printStats() {
    cout << "Total de nodos en pool: " << pool.size() << endl;
    cout << "Root index: " << rootIndex << endl;
    cout << "Entradas en root: " << pool[rootIndex].entries.size() << endl;
}

void RTree::printTree(){
    printNode(rootIndex, 0);
}

void RTree::printNode(int nodeIndex, int depth){
    const Node& node = pool[nodeIndex];
    auto indent = [&](int d){
        for(int i = 0; i < d; i++) std::cout << "  ";
    };
    Rect mbr = node.entries[0].rect;
    for(size_t i = 1; i < node.entries.size(); i++)
        mbr = mbr.combine(node.entries[i].rect);
    indent(depth);
    std::cout << "Nodo " << nodeIndex
              << (node.isLeaf ? " (Hoja)" : " (Interno)")
              << "  MBR=("
              << mbr.minX << "," << mbr.minY << ")-("
              << mbr.maxX << "," << mbr.maxY << ")\n";
    for(size_t i = 0; i < node.entries.size(); i++){
        const Entry& e = node.entries[i];
        indent(depth + 1);
        if(node.isLeaf)
            std::cout << "- Entry[" << i << "]  Punto=("
                      << e.x << ", " << e.y
                      << ") topicId=" << e.topicId
                      << "  Rect=("
                      << e.rect.minX << "," << e.rect.minY << ")-("
                      << e.rect.maxX << "," << e.rect.maxY << ")\n";
        else
            std::cout << "- Entry[" << i << "] -> child=" << e.childIdx
                      << "  Rect=("
                      << e.rect.minX << "," << e.rect.minY << ")-("
                      << e.rect.maxX << "," << e.rect.maxY << ")\n";
    }
    if(!node.isLeaf){
        for(size_t i = 0; i < node.entries.size(); i++)
            printNode(node.entries[i].childIdx, depth + 1);
    }
}
