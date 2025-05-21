#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
using namespace std;

int main(int argc, char** argv){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string algoExe = argv[1];
    const string TESTS = "test.txt";
    ifstream fin(TESTS.c_str());
    if(!fin){ cerr << "Не удалось открыть файл " << TESTS << "\n"; return 1; }

    vector<string> lines;
    string ln;
    while(getline(fin, ln))
        lines.push_back(ln);

    vector<vector<string>> blocks;
    vector<string> cur;
    for(auto &s: lines){
        if(!s.empty() && s[0]=='#'){
            if(!cur.empty()){ blocks.push_back(cur); cur.clear(); }
        } else if(!s.empty()){
            cur.push_back(s);
        }
    }
    if(!cur.empty()) blocks.push_back(cur);

    cout << "Найдено " << blocks.size() << " тестов" << "\n\n";

    for(size_t t=0; t<blocks.size(); ++t){
        ostringstream oss;
        for(auto &s: blocks[t]) oss << s << '\n';
        string input = oss.str();

        const char* tmp_in = "__temp_input.txt";
        FILE* ftmp = fopen(tmp_in, "w");
        fwrite(input.c_str(), 1, input.size(), ftmp);
        fclose(ftmp);

        cout << "Ввод:\n" << input;
        cout << "Вывод:\n" << flush;


        string cmd = algoExe + " < " + string(tmp_in);
        int ret = system(cmd.c_str());
        if(ret != 0) cerr << "Ошибка при выполнении: " << cmd << "\n";
        cout << "\n";

        remove(tmp_in);
    }

    return 0;
}