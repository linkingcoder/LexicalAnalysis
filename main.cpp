#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>

using namespace std;

unordered_set<string> keywords = {
        "auto", "short", "int", "long", "float", "double", "char", "struct",
        "union", "enum", "typedef", "const", "unsigned", "signed", "extern",
        "register", "static", "volatile", "void", "if", "else", "switch", "case", "for", "do", "while",
        "goto", "continue", "break", "default", "sizeof", "return"};
unordered_set<string> header = {
        "#include", "stdio.h", "math.h", "stdlib.h"
};
set<char> ilegalcharacter = {
        '~', '@', '`', '$',
};

bool isErrorKeyWord(string &word) {
    for (auto &k: keywords) {
        if (k.size() == word.size() && k != word) {
            string sortedWord = word;
            string sortedKeyword = k;
            sort(sortedWord.begin(), sortedWord.end());
            sort(sortedKeyword.begin(), sortedKeyword.end());
            if (sortedKeyword == sortedWord) {
                word = k; // ����word��ֵΪƥ��Ĺؼ���
                return true;
            }
        }
    }
    return false; // �ڱ��������йؼ��ֺ󷵻�false
}


bool isilegal(string &word) {
    string legalWord;
    int cnt = 0;
    for (auto c: word) {
        if (ilegalcharacter.count(c) == 0) {
            legalWord += c; // ����ַ��Ϸ�����׷�ӵ��Ϸ��ַ�����
        } else {
            cnt++;
        }
    }
    word = legalWord; // ����wordΪ�Ϸ��ַ���
    return cnt == 1; // �����Ƿ��зǷ��ַ�
}

bool isKeyword(const string &word) {
    return keywords.count(word) > 0;
}

bool isHeader(const string &word) {
    return header.count(word);
}

bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isFloat(const string &word) {
    for (auto c: word) {
        if (c == '.')
            return true;
    }
    return false;
}

bool isIdStartNum(string &word) {
    if (isDigit(word[0]) && isLetter(word[1])) {
        word = word.substr(1);
        return true;
    }
    return false;
};

bool isFormat(string &word) {
    if ((word[0] == '%' || word[0] == '&') && !isDigit(word[1])) {
        return true;
    }
    return false;
}//�ж��Ƿ�Ϊռλ��
int main() {
    string filename = "./Source/test2.c";
    ifstream inputFile(filename);
    int LineCount = 0;
    int characterCount = 0;
    int keywordCount = 0; // �ؼ�������
    int symbolCount = 0;//�������
    int operatorCount = 0;//���������
    int leftParentheses = 0;//��������
    int rightParentheses = 0;//��������
    if (!inputFile.is_open()) {
        cout << "�����޷����ļ���" << filename << endl;
        return 1;
    }
    string line;
    map<string, string> word_id_table;
    string word;
    while (getline(inputFile, line)) {
        int len = line.size();
        if (isilegal(line)) {
            cout << LineCount << "�к��зǷ��ַ�" << endl;
        }
        for (int i = 0; i < len; i++) {
            if (line[i] != ' ') characterCount++;
            if (line[i] == '/') {
                if ((line[i + 1] == '/') && (i < len - 1)) {
                    break;
                } else if (line[i + 1] == '*') {
                    i += 2;
                    while (i < len - 1 && !(line[i] == '*' && line[i + 1] == '/')) {
                        i++;
                    }
                    i++;
                }
            }
            if ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z')
                || isDigit(line[i]) || line[i] == '_' || line[i] == '.' || line[i] == '%' || line[i] == '#' ||
                line[i] == '&') {
                word += line[i];
            } else {
                if (!word.empty()) {
                    if (isErrorKeyWord(word)) {
                        cout << LineCount << "�йؼ��ʴ���" << endl;
                    }

                    if (isHeader(word)) {
                        word_id_table[word] = "header";
                    } else if (isFormat(word)) {
                        word_id_table[word] = "format";
                    } else if (isKeyword(word)) {
                        word_id_table[word] = word; // ���Ϊ�ؼ���
                        keywordCount++;
                    } else if (isFloat(word)) {
                        word_id_table[word] = "float";
                    } else if (isDigit(word[0])) {
                        word_id_table[word] = "num";
                    } else {
                        if (isIdStartNum(word)) {
                            cout << LineCount << "�б�ʶ�������ֿ�ͷ" << endl;
                        }
                        word_id_table[word] = "id";
                    }
                    word.clear();
                }

                if (line[i] == '=' && i < len - 1 && line[i + 1] == '=') {
                    word_id_table["=="] = "relop";
                    i++;
                } else if (line[i] == '=') {
                    word_id_table["="] = "assign-op";
                } else if (line[i] == '<') {
                    if (i < len - 1 && line[i + 1] == '=') {
                        word_id_table["<="] = "relop";
                        i++;
                    } else {
                        word_id_table["<"] = "relop";
                    }
                } else if (line[i] == '>') {
                    if (i < len - 1 && line[i + 1] == '=') {
                        word_id_table[">="] = "relop";
                        i++;
                    } else {
                        word_id_table[">"] = "relop";
                    }
                } else if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%' ||
                           line[i] == ')' ||
                           line[i] == '(' || line[i] == ';' || line[i] == ',' || line[i] == ':' || line[i] == '}' ||
                           line[i] == '{') {
                    if ((line[i] == ')' || line[i] == '(' || line[i] == ';' || line[i] == ',' || line[i] == ':' ||
                         line[i] == '}' || line[i] == '{')) {
                        if (line[i] == '{' || line[i] == '(' || line[i] == '[') {
                            leftParentheses++;
                        }
                        if (line[i] == '}' || line[i] == ')' || line[i] == ']') {
                            rightParentheses++;
                        }
                        symbolCount++;
                    } else operatorCount++;
                    string operatorStr(1, line[i]);
                    word_id_table[operatorStr] = operatorStr;
                }

            }
        }
        if (!word.empty()) {
            if (isKeyword(word)) {
                word_id_table[word] = word; // ���Ϊ�ؼ���
                keywordCount++;
            } else if (isFloat(word)) {
                word_id_table[word] = "float";
            } else if (isdigit(word[0])) {
                word_id_table[word] = "num";
            } else {
                word_id_table[word] = "id";
            }
            word.clear();
        }
        LineCount++;
    }
    if (leftParentheses != rightParentheses) {
        cout << "����ƥ�����" << endl;
    }
    cout << "������" << LineCount << endl;
    cout << "�ַ�����" << characterCount << endl;
    cout << "�ؼ��֣�" << keywordCount << endl;
    cout << "�����" << symbolCount << endl;
    cout << "�������" << operatorCount << endl;
    cout << "���ʱ�" << endl;
    for (const auto &entry: word_id_table) {
        cout << entry.first << " " << entry.second << " ";
    }

    inputFile.close();

    return 0;
}