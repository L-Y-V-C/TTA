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
        line2.clear();
        line2.str(line1);
        while(line2 >> line1){
            token.clear();
            for(char& c : line1){
                if(isalpha(c) || c == '\'')
                    token += tolower(c);
            }
            if(token.empty() || stopwords.count(token))
                continue;
            /*
            if(token.empty())
                continue;
            if(stopwords.find(token) != stopwords.end())
                continue;
            */
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
    topWords.reserve(maxWords);
    for(const auto& w : wordCount){
        if(w.second == maxFreq && topWords.size() < maxWords)
            topWords.push_back({w.first, w.second});
    }
    return topWords;
}

string Lovins::aplyStemming(const string& token){
    //string cond, stem, finalToken = token;
    const int minStemSize = 2;
    size_t tokenLen = token.size();
    if(tokenLen < 3)
        return token;
    for(size_t i = 0; i < tokenLen; i++){
        string suffix = token.substr(i);
        auto itr = appA.find(suffix);
        if(itr != appA.end()){
            //cond = itr->second;
            //stem = token.substr(0, token.size() - itr->first.size());
            size_t stemLen = tokenLen - itr->first.size();
            if(stemLen >= (size_t)minStemSize){
                string stem = token.substr(0, stemLen);
                if(evaluateCondition(itr->second, stem))
                    return recoding(stem);
            }
            //if(stem.size() >= minStemSize && evaluateCondition(cond, stem))
                //finalToken = recoding(stem);
            break;
        }
    }
    return token;
    //return finalToken;
}

bool Lovins::evaluateCondition(const string& cond, const string& stem){
    size_t len = stem.size();
    if(len == 0) return false;

    char last = stem[len - 1];

    // Función auxiliar segura para comparar sufijos
    auto endsWith = [&](const char* suffix, size_t suffixLen) -> bool {
        if(len < suffixLen) return false;
        for(size_t i = 0; i < suffixLen; i++){
            if(stem[len - suffixLen + i] != suffix[i]) return false;
        }
        return true;
    };

    if(cond == "A") return true;
    if(cond == "B") return len >= 3;
    if(cond == "C") return len >= 4;
    if(cond == "D") return len >= 5;
    if(cond == "E") return last != 'e';
    if(cond == "F") return len >= 3 && last != 'e';
    if(cond == "G") return len >= 3 && last == 'f';

    if(cond == "H"){
        return last == 't' || endsWith("ll", 2);
    }
    if(cond == "I") return last != 'o' && last != 'e';
    if(cond == "J") return last != 'a' && last != 'e';

    if(cond == "K"){
        if(len < 3) return false;
        if(last == 'l' || last == 'i') return true;
        return len >= 3 && stem[len - 3] == 'u' && last == 'e';
    }

    if(cond == "L"){
        if(last == 'u' || last == 'x') return false;
        if(last == 's'){
            return len >= 2 && stem[len - 2] == 'o';
        }
        return true;
    }

    if(cond == "M"){
        return last != 'a' && last != 'c' && last != 'e' && last != 'm';
    }

    if(cond == "N"){
        if(len < 3) return false;
        return (stem[len - 3] == 's') ? (len >= 4) : true;
    }

    if(cond == "O") return last == 'l' || last == 'i';
    if(cond == "P") return last != 'c';
    if(cond == "Q") return len >= 3 && last != 'l' && last != 'n';
    if(cond == "R") return last == 'n' || last == 'r';

    if(cond == "S"){
        if(endsWith("dr", 2)) return true;
        if(last == 't'){
            return !(len >= 2 && stem[len - 2] == 't');
        }
        return false;
    }

    if(cond == "T"){
        if(last == 's') return true;
        if(last == 't'){
            return !(len >= 2 && stem[len - 2] == 'o');
        }
        return false;
    }

    if(cond == "U"){
        return last == 'l' || last == 'm' || last == 'n' || last == 'r';
    }
    if(cond == "V") return last == 'c';
    if(cond == "W") return last != 's' && last != 'u';

    if(cond == "X"){
        if(len < 3) return false;
        if(last == 'l' || last == 'i') return true;
        return stem[len - 3] == 'u' && last == 'e';
    }

    if(cond == "Y") return endsWith("in", 2);
    if(cond == "Z") return last != 'f';

    if(cond == "AA"){
        if(last == 'd' || last == 'f' || last == 't' || last == 'l') return true;
        if(len < 2) return false;
        return endsWith("ph", 2) || endsWith("th", 2) ||
               endsWith("er", 2) || endsWith("or", 2) || endsWith("es", 2);
    }

    if(cond == "BB"){
        if(len < 3) return false;
        if(endsWith("met", 3)) return false;
        if(endsWith("ryst", 4)) return false;
        return true;
    }

    if(cond == "CC") return last == 'l';

    return false;
}

string Lovins::recoding(const string& inStem){
    size_t len = inStem.size();
    if(len < 2) return inStem;

    string stem = inStem;
    char last = stem[len - 1];
    char prev = stem[len - 2];

    // Función auxiliar segura
    auto endsWith = [&](const char* suffix, size_t suffixLen) -> bool {
        if(stem.size() < suffixLen) return false;
        for(size_t i = 0; i < suffixLen; i++){
            if(stem[stem.size() - suffixLen + i] != suffix[i]) return false;
        }
        return true;
    };

    // Regla 1: Remover doble consonante
    if(last == prev && (last == 'b' || last == 'd' || last == 'g' ||
        last == 'l' || last == 'm' || last == 'n' || last == 'p' ||
        last == 'r' || last == 's' || last == 't')){
        stem.pop_back();
        len--;
        if(len < 2) return stem;
        last = stem[len - 1];
    }

    // Reglas de 3+ letras
    if(len >= 3){
        if(endsWith("iev", 3)) return stem.substr(0, len - 1) + "f";
        if(endsWith("uct", 3)) return stem.substr(0, len - 1);
        if(endsWith("rpt", 3)) return stem.substr(0, len - 2) + "b";
        if(endsWith("urs", 3)) return stem.substr(0, len - 1);
    }

    if(len >= 4){
        if(endsWith("umpt", 4)) return stem.substr(0, len - 2);
        if(endsWith("istr", 4)) return stem.substr(0, len - 1) + "er";
        if(endsWith("metr", 4)) return stem.substr(0, len - 1) + "er";
    }

    if(len >= 3){
        if(endsWith("olv", 3)) return stem.substr(0, len - 1) + "ut";

        if(endsWith("ul", 2)){
            char check = stem[len - 3];
            if(check != 'a' && check != 'o' && check != 'i')
                return stem.substr(0, len - 2) + "l";
            return stem;
        }

        if(endsWith("bex", 3) || endsWith("dex", 3) ||
           endsWith("pex", 3) || endsWith("tex", 3))
            return stem.substr(0, len - 2) + "ic";

        if(endsWith("lux", 3)) return stem.substr(0, len - 1) + "c";

        if(endsWith("uad", 3) || endsWith("vad", 3) ||
           endsWith("cid", 3) || endsWith("lid", 3))
            return stem.substr(0, len - 1) + "s";

        if(endsWith("end", 3)){
            if(len >= 4 && stem[len - 4] != 's')
                return stem.substr(0, len - 1) + "s";
            return stem;
        }

        if(endsWith("ond", 3) || endsWith("lud", 3) || endsWith("rud", 3))
            return stem.substr(0, len - 1) + "s";

        if(endsWith("her", 3)){
            if(len >= 4 && stem[len - 4] != 'p' && stem[len - 4] != 't')
                return stem.substr(0, len - 1) + "s";
            return stem;
        }

        if(endsWith("mit", 3)) return stem.substr(0, len - 1) + "s";

        if(endsWith("ent", 3)){
            if(len >= 4 && stem[len - 4] != 'm')
                return stem.substr(0, len - 1) + "s";
            return stem;
        }

        if(endsWith("ert", 3)) return stem.substr(0, len - 1) + "s";
    }

    if(len >= 4){
        if(endsWith("erid", 4) || endsWith("pand", 4))
            return stem.substr(0, len - 1) + "s";
    }

    if(len >= 2){
        if(endsWith("ax", 2) || endsWith("ex", 2) || endsWith("ix", 2))
            return stem.substr(0, len - 1) + "c";

        if(endsWith("et", 2)){
            if(len >= 3 && stem[len - 3] != 'n')
                return stem.substr(0, len - 1) + "s";
            return stem;
        }

        if(endsWith("yt", 2) || endsWith("yz", 2))
            return stem.substr(0, len - 1) + "s";
    }

    return stem;
}
