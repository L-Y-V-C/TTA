#include "../hh/lovins.hh"

Lovins::Lovins(){}

void Lovins::readAppendixA(){
    ifstream file("lovins/AppendixA.txt");
    if(!file.is_open()){
        cerr<<"ERROR OPENING APPENDIX A\n";
        return;
    }
    string line1, word, cond;
    stringstream line2;
    while(getline(file, line1)){
        if(isdigit(line1[0]))
            continue;
        line2 = (stringstream)line1;
        while(line2>>word>>cond){
            appA[word] = cond;
        }
    }
    file.close();
}

void Lovins::readStopwords(){
    ifstream file("lovins/stopwords.txt");
    if(!file.is_open()){
        cerr<<"ERROR OPENING STOPWORDS\n";
        return;
    }
    string line;
    while(getline(file, line)){
        stopwords.insert(line);
    }
    file.close();
}

vector<pair<string, int>> Lovins::readFile(const string& fileName,
                                           int maxWords){
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr<<"ERROR OPENING FILE\n";
        return vector<pair<string, int>>();
    }
    unordered_map<string, int> wordCount;
    string line1, token;
    stringstream line2;
    while(getline(file, line1)){
        line2 = (stringstream)line1;
        while(line2 >> line1){
            token.clear();
            for(char& c : line1){
                if(isalpha(c) || c == '\'')
                    token += tolower(c);
            }
            if(token.empty())
                continue;
            if(stopwords.find(token) != stopwords.end())
                continue;
            string stem = aplyStemming(token);
            wordCount[stem]++;
        }
    }
    file.close();
    int maxFreq = 0;
    for(const auto& w : wordCount){
        if(w.second > maxFreq)
            maxFreq = w.second;
    }
    vector<pair<string, int>> topWords;
    for(const auto& w : wordCount){
        if(w.second == maxFreq && topWords.size() < maxWords)
            topWords.push_back({w.first, w.second});
    }
    return topWords;
}

string Lovins::aplyStemming(const string& token){
    string cond, stem, finalToken = token;
    const int minStemSize = 2;
    for(size_t i = 0; i < token.size(); i++){
        string suffix = token.substr(i);
        auto itr = appA.find(suffix);
        if(itr != appA.end()){
            cond = itr->second;
            stem = token.substr(0, token.size() - itr->first.size());
            if(stem.size() >= minStemSize && evaluateCondition(cond, stem))
                finalToken = recoding(stem);
            break;
        }
    }
    return finalToken;
}

bool Lovins::evaluateCondition(const string& cond, const string& stem){
    int stemSize = stem.size();
    if(stemSize == 0)
        return false;
    char lastChar = stem.back();

    // ---
    auto has2 = (stemSize >= 2);
    auto has3 = (stemSize >= 3);
    auto has4 = (stemSize >= 4);

    auto endsWith = [&](const string& s) -> bool {
        size_t n = s.size();
        if (stemSize < n) return false;
        return stem.compare(stemSize - n, n, s) == 0;
    };
    // ---

    if(cond == "A"){
        return true;
    }else if(cond == "B"){
        return stemSize >= 3;
    }
    else if(cond == "C"){
        return stemSize >= 4;
    }
    else if(cond == "D"){
        return stemSize >= 5;
    }
    else if(cond == "E"){
        return lastChar != 'e';
    }
    else if(cond == "F"){
        return stemSize >= 3 && lastChar != 'e';
    }
    else if(cond == "G"){
        return stemSize >= 3 && lastChar == 'f';
    }
    else if(cond == "H"){
        return lastChar == 't' || (has2 && endsWith("ll"));
        //return lastChar == 't' || stem.substr(stemSize - 2) == "ll";
    }
    else if(cond == "I"){
        return lastChar != 'o' && lastChar != 'e';
    }
    else if(cond == "J"){
        return lastChar != 'a' && lastChar != 'e';
    }
    else if(cond == "K"){
        if(stemSize < 3)
            return false;
        if(lastChar == 'l' || lastChar == 'i')
            return true;
        if(stem[stemSize - 3] == 'u' && lastChar == 'e')
            return true;
        return false;
    }
    else if(cond == "L"){
        if(lastChar == 'u' || lastChar == 'x')
            return false;
        if(lastChar == 's'){
            return (has2 && stem[stemSize - 2] == 'o');
            //if (stem[stemSize - 2] == 'o')
                //return true;
            //return false;
        }
        return true;
    }
    else if(cond == "M"){
        return lastChar != 'a' && lastChar != 'c' && lastChar != 'e' && lastChar != 'm';
    }
    else if(cond == "N"){
        if(!has3)
            return false; // ---
        if(stemSize >= 3 && stem[stemSize - 3] == 's')
            return stemSize >= 4;
        return stemSize >= 3;
    }
    else if(cond == "O"){
        return lastChar == 'l' || lastChar == 'i';
    }
    else if(cond == "P"){
        return lastChar != 'c';
    }
    else if(cond == "Q"){
        return stemSize >= 3 && lastChar != 'l' && lastChar != 'n';
    }
    else if(cond == "R"){
        return lastChar == 'n' || lastChar == 'r';
    }
    else if(cond == "S"){
        if(has2 && endsWith("dr"))
            return true; // ---
        //if(stem.substr(stemSize - 2) == "dr")
            //return true;
        if(lastChar == 't'){
            //if(stem[stemSize - 2] == 't')
            if(has2 && stem[stemSize - 2] == 't')
                return false;
            return true;
        }
        return false;
    }
    else if(cond == "T"){
        if(lastChar == 's')
            return true;
        if(lastChar == 't'){
            //if(stem[stemSize - 2] == 'o')
            if(has2 && stem[stemSize - 2] == 'o')
                return false;
            return true;
        }
        return false;
    }
    else if(cond == "U"){
        return lastChar == 'l' || lastChar == 'm' || lastChar == 'n' || lastChar == 'r';
    }
    else if(cond == "V"){
        return lastChar == 'c';
    }
    else if(cond == "W"){
        return lastChar != 's' && lastChar != 'u';
    }
    else if(cond == "X"){
        if(!has3)
            return false; //---
        //if(stemSize < 3)
            //return false;
        if(lastChar == 'l' || lastChar == 'i')
            return true;
        if(stem[stemSize - 3] == 'u' && lastChar == 'e')
            return true;
        return false;
    }
    else if(cond == "Y"){
        return has2 && endsWith("in");
        //return stem.substr(stemSize - 2) == "in";
    }
    else if(cond == "Z"){
        return lastChar != 'f';
    }
    else if(cond == "AA"){
        if(lastChar == 'd' || lastChar == 'f' || lastChar == 't' || lastChar == 'l')
            return true;
        if(has2){
            string End2 = stem.substr(stemSize - 2);
            return (End2 == "ph" || End2 == "th" || End2 == "er" || End2 == "or" || End2 == "es");
        }
        //string End = stem.substr(stemSize - 2);
        //if (End == "ph" || End == "th" || End == "er" || End == "or" || End == "es")
        //    return true;
        return false;
    }
    else if(cond == "BB"){
        if(stemSize < 3)
            return false;
        if(has3 && endsWith("met"))
            return false;
        if(has4 && endsWith("ryst"))
            return false;
        /*
        if(stem.substr(stemSize - 3) == "met")
            return false;
        if(stemSize >= 4 && stem.substr(stemSize - 4) == "ryst")
            return false;
        */
        return true;
    }
    else if(cond == "CC"){
        return lastChar == 'l';
    }
    return false;
}

string Lovins::recoding(const string& inStem){
    string suffix, stem = inStem;
    int stemSize = stem.size();
    if(stemSize == 0)
        return inStem;
    char last = stem[stemSize - 1];
    char prev = stem[stemSize - 2];
    if(last == prev && (last == 'b' || last == 'd' || last == 'g' ||
        last == 'l' || last == 'm' || last == 'n' || last == 'p' ||
        last == 'r' || last == 's' || last == 't')){
        stem = stem.substr(0, stemSize - 1);
        stemSize--;
    }
    if(stemSize >= 3){
        if(stem.substr(stemSize - 3) == "iev")
            return stem.substr(0, stemSize - 1) + "f";
        if(stem.substr(stemSize - 3) == "uct")
            return stem.substr(0, stemSize - 1);
        if(stemSize >= 4 && stem.substr(stemSize - 4) == "umpt")
            return stem.substr(0, stemSize - 2);
        if(stem.substr(stemSize - 3) == "rpt")
            return stem.substr(0, stemSize - 2) + "b";
        if(stem.substr(stemSize - 3) == "urs")
            return stem.substr(0, stemSize - 1);
    }
    if(stemSize >= 4){
        if(stem.substr(stemSize - 4) == "istr")
            return stem.substr(0, stemSize - 1) + "er";
        if(stem.substr(stemSize - 4) == "metr")
            return stem.substr(0, stemSize - 1) + "er";
    }
    if(stemSize >= 3){
        if(stem.substr(stemSize - 3) == "olv")
            return stem.substr(0, stemSize - 1) + "ut";
        if(stem.substr(stemSize - 2) == "ul"){
            char check = stem[stemSize - 3];
            if (check != 'a' && check != 'o' && check != 'i')
                return stem.substr(0, stemSize - 2) + "l";
            return stem;
        }
        suffix = stem.substr(stemSize - 3);
        if(suffix == "bex" || suffix == "dex" || suffix == "pex" || suffix == "tex")
            return stem.substr(0, stemSize - 2) + "ic";
    }
    if(stemSize >= 2){
        suffix = stem.substr(stemSize - 2);
        if(suffix == "ax" || suffix == "ex" || suffix == "ix")
            return stem.substr(0, stemSize - 1) + "c";
        return stem;
    }
    if(stemSize >= 3){
        if(stem.substr(stemSize - 3) == "lux")
            return stem.substr(0, stemSize - 1) + "c";
        suffix = stem.substr(stemSize - 3);
        if(suffix == "uad" || suffix == "vad" || suffix == "cid" || suffix == "lid")
            return stem.substr(0, stemSize - 1) + "s";
        if(stemSize >= 4) {
            suffix = stem.substr(stemSize - 4);
            if(suffix == "erid" || suffix == "pand")
                return stem.substr(0, stemSize - 1) + "s";
            return stem;
        }
        if(stem.substr(stemSize - 3) == "end"){
            if(stemSize >= 4 && stem[stemSize - 4] != 's')
                return stem.substr(0, stemSize - 1) + "s";
            return stem;
        }
        suffix = stem.substr(stemSize - 3);
        if(suffix == "ond" || suffix == "lud" || suffix == "rud")
            return stem.substr(0, stemSize - 1) + "s";
        if(stem.substr(stemSize - 3) == "her"){
            if (stemSize >= 4 && (stem[stemSize - 4] != 'p' && stem[stemSize - 4] != 't'))
                return stem.substr(0, stemSize - 1) + "s";
            return stem;
        }
        if(stem.substr(stemSize - 3) == "mit")
            return stem.substr(0, stemSize - 1) + "s";
        if(stem.substr(stemSize - 3) == "ent"){
            if (stemSize >= 4 && stem[stemSize - 4] != 'm')
                return stem.substr(0, stemSize - 1) + "s";
            return stem;
        }
        if(stem.substr(stemSize - 3) == "ert")
            return stem.substr(0, stemSize - 1) + "s";
    }
    if(stemSize >= 2){
        if(stem.substr(stemSize - 2) == "et"){
            if (stemSize >= 3 && stem[stemSize - 3] != 'n')
                return stem.substr(0, stemSize - 1) + "s";
            return stem;
        }
        string suffix = stem.substr(stemSize - 2);
        if(suffix == "yt" || suffix == "yz")
            return stem.substr(0, stemSize - 1) + "s";
    }
    return stem;
}
