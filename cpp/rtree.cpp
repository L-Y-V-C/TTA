#include "../hh/rtree.hh"

RTree::RTree(){
    rootIndex = pool.assignNode(true);
    reinsertedAtLevel.push_back(false);
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
    bool childrenAreLeaves = !node.isLeaf && pool[node.entries[0].childIdx].isLeaf;
    if(childrenAreLeaves){
        // overlap minimize
        int minOverlap = numeric_limits<int>::max();
        int minEnlargement = numeric_limits<int>::max();
        int minArea = numeric_limits<int>::max();
        for(size_t i = 0; i < node.entries.size(); i++){
            Rect combined = node.entries[i].rect.combine(rect);
            int enlargement = combined.area() - node.entries[i].rect.area();
            // overlap calculus
            int overlap = calculateOverlap(combined, node.entries, i);
            if(overlap < minOverlap ||
               (overlap == minOverlap && enlargement < minEnlargement) ||
               (overlap == minOverlap && enlargement == minEnlargement &&
                node.entries[i].rect.area() < minArea)){
                minOverlap = overlap;
                minEnlargement = enlargement;
                minArea = node.entries[i].rect.area();
                bestIdx = i;
            }
        }
    }else{
        // area minimize
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
    }
    return bestIdx;
}

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
    // R* split
    Mpair<int, int> splitInfo = chooseSplitAxisAndIndex(allEntries);
    int axis = splitInfo.first;
    int distIndex = splitInfo.second;
    // regen choose distro
    Mvector<Entry> entriesCopy = allEntries;
    Mvector<Distribution> finalDists = generateDistributions(entriesCopy, axis == 0);
    Distribution& chosen = finalDists[distIndex];
    node.entries = chosen.group1;
    int siblingIndex = pool.assignNode(node.isLeaf);
    Node& sibling = pool[siblingIndex];
    sibling.entries = chosen.group2;
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
    int level = 0;
    while(!current->isLeaf){
        int idx = chooseSubtree(currentIndex, entry.rect);
        path.push_back(currentIndex);
        indices.push_back(idx);
        currentIndex = current->entries[idx].childIdx;
        current = &pool[currentIndex];
        level++;
    }
    current->entries.push_back(entry);
    if(current->entries.size() > MAX_ENTRIES){
        // verify level reinsert
        while(reinsertedAtLevel.size() <= (size_t)level)
            reinsertedAtLevel.push_back(false);
        if(!reinsertedAtLevel[level]){
            // force reinsert
            reinsertedAtLevel[level] = true;
            reinsert(currentIndex, level);
            // no adjust
            return;
        }else{
            // normal split
            int siblingIndex = splitNode(currentIndex);
            adjustTree(path, indices, currentIndex, siblingIndex);
        }
    }else
        adjustTree(path, indices, currentIndex, -1);
    // reset reinsert flags
    for(size_t i = 0; i < reinsertedAtLevel.size(); i++)
        reinsertedAtLevel[i] = false;
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

int RTree::calculateOverlap(const Rect& rect, const Mvector<Entry>& entries,
                            int excludeIdx){
    int totalOverlap = 0;
    for(size_t i = 0; i < entries.size(); i++){
        if(i == (size_t)excludeIdx)
            continue;
        Rect intersection;
        int ix1 = max(rect.minX, entries[i].rect.minX);
        int iy1 = max(rect.minY, entries[i].rect.minY);
        int ix2 = min(rect.maxX, entries[i].rect.maxX);
        int iy2 = min(rect.maxY, entries[i].rect.maxY);
        if(ix1 < ix2 && iy1 < iy2)
            totalOverlap += (ix2 - ix1) * (iy2 - iy1);
    }
    return totalOverlap;
}

Mvector<Distribution> RTree::generateDistributions(Mvector<Entry>& entries,
                                                   bool sortByLower){
    Mvector<Distribution> distributions;
    size_t M = entries.size() - 1;
    size_t m = MIN_ENTRIES;
    // sort entries
    if(sortByLower){
        // by minX
        for(size_t i = 0; i < entries.size(); i++){
            for(size_t j = i + 1; j < entries.size(); j++){
                if(entries[j].rect.minX < entries[i].rect.minX){
                    Entry temp = entries[i];
                    entries[i] = entries[j];
                    entries[j] = temp;
                }
            }
        }
    }else{
        // by maxX
        for(size_t i = 0; i < entries.size(); i++){
            for(size_t j = i + 1; j < entries.size(); j++){
                if(entries[j].rect.maxX < entries[i].rect.maxX){
                    Entry temp = entries[i];
                    entries[i] = entries[j];
                    entries[j] = temp;
                }
            }
        }
    }
    // M - 2m + 2 distros
    for(size_t k = 0; k <= M - 2*m + 1; k++){
        Distribution dist;
        // first (m + k) elements
        for(size_t i = 0; i < m + k; i++)
            dist.group1.push_back(entries[i]);
        // rest
        for(size_t i = m + k; i < entries.size(); i++)
            dist.group2.push_back(entries[i]);
        // goodness values
        Rect bb1 = boundingRect(dist.group1);
        Rect bb2 = boundingRect(dist.group2);
        // margin = perimeter
        dist.marginSum = 2 * ((bb1.maxX - bb1.minX) + (bb1.maxY - bb1.minY)) +
                        2 * ((bb2.maxX - bb2.minX) + (bb2.maxY - bb2.minY));
        dist.areaSum = bb1.area() + bb2.area();
        // overlap
        int ix1 = max(bb1.minX, bb2.minX);
        int iy1 = max(bb1.minY, bb2.minY);
        int ix2 = min(bb1.maxX, bb2.maxX);
        int iy2 = min(bb1.maxY, bb2.maxY);
        if(ix1 < ix2 && iy1 < iy2)
            dist.overlap = (ix2 - ix1) * (iy2 - iy1);
        else
            dist.overlap = 0;
        distributions.push_back(dist);
    }
    return distributions;
}

Mpair<int, int> RTree::chooseSplitAxisAndIndex(const Mvector<Entry>& entries){
    Mvector<Entry> entriesCopy = entries;
    // gen distros for axis
    Mvector<Distribution> distsLower = generateDistributions(entriesCopy, true);
    entriesCopy = entries;
    Mvector<Distribution> distsUpper = generateDistributions(entriesCopy, false);
    // margins sum per axes
    int sumMarginLower = 0;
    for(size_t i = 0; i < distsLower.size(); i++)
        sumMarginLower += distsLower[i].marginSum;
    int sumMarginUpper = 0;
    for(size_t i = 0; i < distsUpper.size(); i++)
        sumMarginUpper += distsUpper[i].marginSum;
    // axi with less margin sum
    Mvector<Distribution>& chosenDists = (sumMarginLower < sumMarginUpper) ? distsLower : distsUpper;
    int chosenAxis = (sumMarginLower < sumMarginUpper) ? 0 : 1;
    // distro with min overlap
    int bestDistIdx = 0;
    int minOverlap = chosenDists[0].overlap;
    int minArea = chosenDists[0].areaSum;
    for(size_t i = 1; i < chosenDists.size(); i++){
        if(chosenDists[i].overlap < minOverlap ||
           (chosenDists[i].overlap == minOverlap && chosenDists[i].areaSum < minArea)){
            minOverlap = chosenDists[i].overlap;
            minArea = chosenDists[i].areaSum;
            bestDistIdx = i;
        }
    }
    return Mpair<int, int>(chosenAxis, bestDistIdx);
}

void RTree::reinsert(int nodeIndex, int level){
    Node& node = pool[nodeIndex];
    Mvector<Entry> entries = node.entries;
    // MBR center
    Rect nodeMBR = boundingRect(entries);
    int centerX = (nodeMBR.minX + nodeMBR.maxX) / 2;
    int centerY = (nodeMBR.minY + nodeMBR.maxY) / 2;
    // distances to center
    Mvector<Mpair<int, int>> distances;
    for(size_t i = 0; i < entries.size(); i++){
        int entryX = (entries[i].rect.minX + entries[i].rect.maxX) / 2;
        int entryY = (entries[i].rect.minY + entries[i].rect.maxY) / 2;
        int dist = (entryX - centerX) * (entryX - centerX) +
                   (entryY - centerY) * (entryY - centerY);
        distances.push_back(Mpair<int, int>(dist, i));
    }
    // sort by dist desc
    for(size_t i = 0; i < distances.size(); i++){
        for(size_t j = i + 1; j < distances.size(); j++){
            if(distances[j].first > distances[i].first){
                Mpair<int, int> temp = distances[i];
                distances[i] = distances[j];
                distances[j] = temp;
            }
        }
    }
    // p = 30% M
    int p = max(1, (int)(MAX_ENTRIES * 0.3));
    // remove far p entries
    Mvector<Entry> toReinsert;
    Mvector<Entry> remaining;
    for(int i = 0; i < p; i++)
        toReinsert.push_back(entries[distances[i].second]);
    for(size_t i = p; i < distances.size(); i++)
        remaining.push_back(entries[distances[i].second]);
    node.entries = remaining;
    // reinsert
    for(size_t i = 0; i < toReinsert.size(); i++)
        insertEntry(rootIndex, toReinsert[i]);
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
