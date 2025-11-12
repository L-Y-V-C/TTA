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

void Lovins::readNews(const string& folderName){
    if(!fs::exists(folderName)){
        cerr<<"ERROR FINDING DIRECTORY\n";
        return;
    }
    for(const auto& entry : fs::directory_iterator(folderName)){
        if(entry.is_regular_file()){
            string fileName = entry.path().filename().string();
            cout<<"Archivo: "<<fileName<<endl;
            ifstream file(entry.path());
            if(!file.is_open()){
                cerr<<"ERROR OPENING FOLDER FILES\n";
                return;
            }
            string line1, token;
            stringstream line2;
            vector<string> words;
            while(getline(file, line1)){
                line2 = (stringstream)line1;
                while(line2>>line1){
                    token.clear();
                    for(char& c : line1){
                        if(isalpha(c) || c == '\'')
                            token += tolower(c);
                    }
                    if(token.empty())
                        continue;
                    if(stopwords.find(token) == stopwords.end())
                        words.push_back(token);
                }
            }
            file.close();

            for(auto& w : words){
                cout<<w<<" ";
                string finalToken = aplyStemming(w);
                cout<<"-"<<finalToken<<"-";
                //if(!cond.empty())
                    //cout<<"cond(f) "<<cond;
                //else
                    //cout<<cond<<" ";
                cout<<endl;
            }
            cout<<endl;

            cout<<"======================================================\n";
        }
    }
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
            if(stem.size() >= minStemSize && evaluateCondition(cond, stem)){
                finalToken = recoding(stem);
                //finalToken = recoding(token.substr(0, token.size() - ), stem);
            }
            //cout<<" -"<<finalToken<<"- ";
            //cout<<itr->first<<" "<<stem<<" ";
            //else
                //cout<<"f "<<stem<<"-";
            break;
        }
    }
    return finalToken;
}

bool Lovins::evaluateCondition(const string& cond, const string& stem){
    int stemSize = stem.size();
    char lastChar = stem.back();
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
        return lastChar == 't' || stem.substr(stemSize - 2) == "ll";
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
            if (stem[stemSize - 2] == 'o')
                return true;
            return false;
        }
        return true;
    }
    else if(cond == "M"){
        return lastChar != 'a' && lastChar != 'c' && lastChar != 'e' && lastChar != 'm';
    }
    else if(cond == "N"){
        if(stem[stemSize - 3] == 's')
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
        if(stem.substr(stemSize - 2) == "dr")
            return true;
        if(lastChar == 't'){
            if(stem[stemSize - 2] == 't')
                return false;
            return true;
        }
        return false;
    }
    else if(cond == "T"){
        if(lastChar == 's')
            return true;
        if(lastChar == 't'){
            if(stem[stemSize - 2] == 'o')
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
        if(stemSize < 3)
            return false;
        if(lastChar == 'l' || lastChar == 'i')
            return true;
        if(stem[stemSize - 3] == 'u' && lastChar == 'e')
            return true;
        return false;
    }
    else if(cond == "Y"){
        return stem.substr(stemSize - 2) == "in";
    }
    else if(cond == "Z"){
        return lastChar != 'f';
    }
    else if(cond == "AA"){
        if(lastChar == 'd' || lastChar == 'f' || lastChar == 't' || lastChar == 'l')
            return true;
        string End = stem.substr(stemSize - 2);
        if (End == "ph" || End == "th" || End == "er" || End == "or" || End == "es")
            return true;
        return false;
    }
    else if(cond == "BB"){
        if(stemSize < 3)
            return false;
        if(stem.substr(stemSize - 3) == "met")
            return false;
        if(stemSize >= 4 && stem.substr(stemSize - 4) == "ryst")
            return false;
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
