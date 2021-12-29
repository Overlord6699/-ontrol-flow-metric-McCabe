#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>
#include <algorithm>

using namespace std;

static const regex rIf(R"((if)\s)");
static const regex rElse(R"((else)\s\{)");
static const regex rFor(R"((for)\s)");
static const regex rWhile(R"((while)\s(\()(.*)(\))\s(\{))");
static const regex rDoWhile(R"((do)\s(\{))");
static const regex rSwitch(R"((switch)\s)");
static const regex rCase(R"((case)\s)");
static const regex rDefault(R"((default)(\:))");
static const regex rBreak(R"((break)(\;))");
static const regex rOpenedCurlyBrace(R"(\{)");
static const regex rClosedCurlyBrace(R"(\})");

static const int TAB = 2;

string inputPathToFile() {
    bool isIncorrect;
    string path;

    do {
        isIncorrect = false;
        cin >> path;
        ifstream file(path);

        if (!file.is_open()) {
            cout << "Файл не найден! Введите правильную ссылку: ";
            isIncorrect = true;
        }

    } while (isIncorrect);

    return path;
}

vector<string> readLines(string path) {
    fstream fStream(path);
    vector<string> lines;
    for (string line; getline(fStream, line);) {
        lines.push_back(line);
    }
    return lines;
}

int findVertexNumber(vector<string> lines) {

    int counter = 0;
    int i;

    int choice = 0, receive = 0;

    // вершина до первого условного оператора
    for (i = 0; i < lines.size(); i++) {
        if (regex_search(lines.at(i), rIf) || regex_search(lines.at(i), rFor) ||
            regex_search(lines.at(i), rWhile) || regex_search(lines.at(i), rDoWhile) ||
            regex_search(lines.at(i), rSwitch)) {
            if (i != 0) {
                counter++;
                receive++;
            }
            break;
        }
    }

    for (; i < lines.size(); i++) {

        // if, for, while, do while, case
        if (regex_search(lines.at(i), rIf) || regex_search(lines.at(i), rFor) ||
            regex_search(lines.at(i), rWhile) || regex_search(lines.at(i), rDoWhile) ||
            regex_search(lines.at(i), rCase)) {
            counter++;
            choice++;
        }

        // после {, default и case есть что-то кроме if, for, while, case, switch или default
        if ((regex_search(lines.at(i), rOpenedCurlyBrace) || regex_search(lines.at(i), rCase) || regex_search(lines.at(i), rDefault)) &&
            !(regex_search(lines.at(i + 1), rIf) || regex_search(lines.at(i + 1), rSwitch) ||
                regex_search(lines.at(i + 1), rFor) || regex_search(lines.at(i + 1), rWhile) ||
                regex_search(lines.at(i + 1), rCase) || regex_search(lines.at(i + 1), rDefault))) {
            counter++;
            receive++;
        }

        // после } есть что то кроме else, if, for, while, break, case, switch, do while и }
        if ((i + 1 < lines.size()) && regex_search(lines.at(i), rClosedCurlyBrace) &&
            !(regex_search(lines.at(i + 1), rIf) || regex_search(lines.at(i + 1), rElse) ||
                regex_search(lines.at(i + 1), rFor) || regex_search(lines.at(i + 1), rWhile) ||
                regex_search(lines.at(i + 1), rBreak) || regex_search(lines.at(i + 1), rCase) ||
                regex_search(lines.at(i + 1), rSwitch) || regex_search(lines.at(i + 1), rDoWhile) ||
                regex_search(lines.at(i + 1), rClosedCurlyBrace))) {
            counter++;
            receive++;
        }
    }

    //cout << "Принимающие: " << receive << "\nВыбора: " << choice << endl;

    return counter;
}

int findEdgesNumber(vector<string> lines) {

    int counter = 1;

    // если в первой строчке не условный оператор
    if (!(regex_search(lines.at(0), rIf) || regex_search(lines.at(0), rFor) ||
        regex_search(lines.at(0), rWhile) || regex_search(lines.at(0), rDoWhile) ||
        regex_search(lines.at(0), rSwitch))) {
        counter++;
    }

    for (int i = 0; i < lines.size(); i++) {

        // if, for, while, do while, case дают 2 ребра и если после них не if, for, while, do while, switch +1 ребро
        if (regex_search(lines.at(i), rIf) || regex_search(lines.at(i), rFor) ||
            regex_search(lines.at(i), rWhile) || regex_search(lines.at(i), rDoWhile) ||
            regex_search(lines.at(i), rCase)) {
            counter += 2;
            if (!(regex_search(lines.at(i + 1), rIf) || regex_search(lines.at(i + 1), rFor) ||
                regex_search(lines.at(i + 1), rWhile) || regex_search(lines.at(i + 1), rDoWhile) ||
                regex_search(lines.at(i + 1), rSwitch))) {
                counter++;
            }
        }

        // если после default не if, for, while, do while, switch +1 ребро
        if (regex_search(lines.at(i), rDefault) &&
            !(regex_search(lines.at(i + 1), rIf) || regex_search(lines.at(i + 1), rFor) ||
                regex_search(lines.at(i + 1), rWhile) || regex_search(lines.at(i + 1), rDoWhile) ||
                regex_search(lines.at(i + 1), rSwitch))) {
            counter++;

        }

        // else дает 1 ребро если после него не if, for, while, do while, switch
        if (regex_search(lines.at(i), rElse) && !(regex_search(lines.at(i + 1), rIf) ||
            regex_search(lines.at(i + 1), rFor) || regex_search(lines.at(i + 1), rWhile) ||
            regex_search(lines.at(i), rDoWhile) || regex_search(lines.at(i), rCase))) {
            counter++;
        }

        // после } есть что то кроме else, if, for, while, break, case, switch, do while и }
        if ((i + 1 < lines.size()) && regex_search(lines.at(i), rClosedCurlyBrace) &&
            !(regex_search(lines.at(i + 1), rIf) || regex_search(lines.at(i + 1), rElse) ||
                regex_search(lines.at(i + 1), rFor) || regex_search(lines.at(i + 1), rWhile) ||
                regex_search(lines.at(i + 1), rBreak) || regex_search(lines.at(i + 1), rCase) ||
                regex_search(lines.at(i + 1), rSwitch) || regex_search(lines.at(i + 1), rDoWhile) ||
                regex_search(lines.at(i + 1), rClosedCurlyBrace))) {
            counter++;
        }
    }

    // если программа заканчивается не условным оператором
    if (!regex_search(lines.at(lines.size() - 1), rClosedCurlyBrace) && !regex_search(lines.at(lines.size() - 1), rDoWhile)) {
        counter++;
    }

    return counter;
}

int findCondOperatorsNumber(vector<string> lines) {
    int counter = 0;
    for (int i = 0; i < lines.size(); i++) {
        if (regex_search(lines.at(i), rIf) || regex_search(lines.at(i), rFor) ||
            regex_search(lines.at(i), rWhile) || regex_search(lines.at(i), rDoWhile) ||
            regex_search(lines.at(i), rCase)) {
            counter++;
        }
    }
    return counter;
}

vector<regex> readOperatorsReg(string path) {
    fstream fStream(path);
    vector<regex> operators;
    for (string oper; getline(fStream, oper);) {
        operators.push_back((regex)oper);
    }
    return operators;
}

string findString(vector<string> lines) {
    string str;
    for (int i = 0; i < lines.size(); i++) {
        str += lines.at(i);
    }
    return str;
}

int findOperatorsNumber(vector<string> lines, vector<regex> operatorsReg) {
    string mainString = findString(lines);
    int counter = 0;
    int opers;
    for (int i = 0; i < operatorsReg.size(); i++) {
        opers = distance(sregex_iterator(mainString.begin(), mainString.end(), operatorsReg.at(i)), sregex_iterator());
        counter += opers;
        //cout << i + 1 << "\t" << opers << endl;
    }
    return counter;
}

int findTabulation(string str) {
    int tab = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str.at(i) == ' ') tab++;
        else break;
    }
    return tab;
}

string getTabulation(int tab) {
    string s = "";
    for (int i = 0; i < tab; i++) {
        s += " ";
    }
    return s;
}

void removeBreak(int i, int switchTab, vector<string>& lines) {
    for (; i < lines.size(); i++) {
        if ((findTabulation(lines.at(i)) == (switchTab + TAB * 2)) && regex_search(lines.at(i), rBreak)) {
            lines.erase(lines.begin() + i);
            break;
        }
    }
}

void replaceSwitch(vector<string>& lines) {

    for (int i = 0; i < lines.size(); i++) {

        if (regex_search(lines.at(i), rSwitch)) {

            // обработка первого case
            int switchTab = findTabulation(lines.at(i));
            lines.erase(lines.begin() + i); // стирает switch
            lines.erase(lines.begin() + i); // стрирает case
            removeBreak(i, switchTab, lines);
            lines.emplace(lines.begin() + i, getTabulation(switchTab) + "if () {");

            // обработка остальных case и default
            int counter = 0;
            int braces = 0;
            int j;
            for (j = i; j < lines.size(); j++) {
                if (regex_search(lines.at(j), rCase) && (findTabulation(lines.at(j)) == (switchTab + TAB))) {
                    lines.erase(lines.begin() + j);
                    lines.emplace(lines.begin() + j, getTabulation(switchTab + counter * TAB) + "}");
                    lines.emplace(lines.begin() + j + 1, getTabulation(switchTab + counter * TAB) + "else {");
                    lines.emplace(lines.begin() + j + 2, getTabulation(switchTab + counter * TAB) + "  if () {");
                    removeBreak(i, switchTab, lines);
                    counter++;
                }
                if (regex_search(lines.at(j), rDefault) && (findTabulation(lines.at(j)) == (switchTab + TAB))) {
                    lines.erase(lines.begin() + j);
                    lines.emplace(lines.begin() + j, getTabulation(switchTab + counter * TAB) + "}");
                    lines.emplace(lines.begin() + j + 1, getTabulation(switchTab + counter * TAB) + "else {");
                    removeBreak(i, switchTab, lines);
                    counter++;
                }
                int q = (findTabulation(lines.at(j)) == switchTab);
                if ((findTabulation(lines.at(j)) == switchTab) && regex_search(lines.at(j), rClosedCurlyBrace)) {
                    braces++;
                    if (braces > 1) {
                        break;
                    }
                }
            }

            // обработка последних }
            lines.erase(lines.begin() + j);
            counter--;
            for (; counter > -1; counter--) {
                lines.emplace(lines.begin() + j, getTabulation(switchTab + counter * TAB) + "}");
                j++;
            }
        }
    }
}

vector<int> findNests(vector<string> lines) {
    string mainString = findString(lines);
    int openBrace = 0;
    int closedBrace = 0;
    vector<int> nests;
    int end = 0;
    for (int i = 0; i < mainString.size(); i++) {
        if (mainString.at(i) == '{') {
            openBrace++;
            for (int j = i + 1; j < mainString.size(); j++) {
                if (mainString.at(j) == '{') {
                    if (closedBrace > 0) {
                        nests.push_back(openBrace);
                        openBrace -= closedBrace;
                        closedBrace = 0;
                    }
                    openBrace++;
                }
                if (mainString.at(j) == '}') {
                    closedBrace++;
                    if (openBrace == closedBrace) {
                        nests.push_back(closedBrace);
                        closedBrace = 0;
                        openBrace = 0;
                        end = j;
                        break;
                    }
                }
            }
            i = end;
        }
    }
    return nests;
}

bool isSymbol(string str, char ch) {
    for (int i = 0; i < str.size(); i++) {
        if (str.at(i) == ch) return true;
    }
    return false;
}

void removeElse(vector<string>& lines, int lineNum) {
    int openBrace = 0;
    int closedBrace = 0;
    for (int i = lineNum; i < lines.size(); i++) {
        if (isSymbol(lines.at(i), '{')) openBrace++;
        if (isSymbol(lines.at(i), '}')) closedBrace++;
        if (openBrace == closedBrace) {
            lines.erase(lines.begin() + lineNum, lines.begin() + i + 1);
            break;
        }
    }
}

// чистит код так, чтобы с заданной строчки остался только ее подграф
vector<string> findNewLines(vector<string> lines, int lineNum) {

    vector<string> newLines = lines;

    for (int i = lineNum - 1; i > -1; i--) {
        if (isSymbol(lines.at(i), '}')) {
            break;
        }
        if (regex_search(lines.at(i), rFor) || regex_search(lines.at(i), rWhile) ||
            regex_search(lines.at(i), rDoWhile)) {
            lineNum = i;
        }
    }

    newLines.erase(newLines.begin(), newLines.begin() + lineNum);

    int tab = findTabulation(lines.at(lineNum)) - TAB;
    for (int i = tab; i > -1; i -= TAB) {
        for (int j = 0; j < newLines.size(); j++) {
            if (findTabulation(newLines.at(j)) == i) {
                newLines.erase(newLines.begin() + j);
                if ((j < newLines.size()) && regex_search(newLines.at(j), rElse)) {
                    removeElse(newLines, j);
                }
                break;
            }
        }
    }

    return newLines;
}

int findAbsoluteComplexity(vector<string> lines) {

    int counter = 0;
    int i;

    // вершина до первого условного оператора
    for (i = 0; i < lines.size(); i++) {
        if (regex_search(lines.at(i), rIf) || regex_search(lines.at(i), rFor) ||
            regex_search(lines.at(i), rWhile) || regex_search(lines.at(i), rDoWhile)) {
            if (i != 0) {
                counter++;
            }
            break;
        }
    }

    for (; i < lines.size(); i++) {

        // if, for, while, do while
        if (regex_search(lines.at(i), rIf) || regex_search(lines.at(i), rFor) ||
            regex_search(lines.at(i), rWhile) || regex_search(lines.at(i), rDoWhile)) {
            counter += findVertexNumber(findNewLines(lines, i)) - 1;
        }

        // после { есть что-то кроме if, for, while, do while
        if (regex_search(lines.at(i), rOpenedCurlyBrace) && !(regex_search(lines.at(i + 1), rIf) ||
            regex_search(lines.at(i + 1), rFor) || regex_search(lines.at(i + 1), rWhile) ||
            regex_search(lines.at(i + 1), rDoWhile))) {
            counter++;
        }

        // после } есть что то кроме else, if, for, while, do while и }
        if ((i + 1 < lines.size()) && regex_search(lines.at(i), rClosedCurlyBrace) &&
            !(regex_search(lines.at(i + 1), rIf) || regex_search(lines.at(i + 1), rElse) ||
                regex_search(lines.at(i + 1), rFor) || regex_search(lines.at(i + 1), rWhile) ||
                regex_search(lines.at(i + 1), rClosedCurlyBrace) || regex_search(lines.at(i + 1), rDoWhile))) {
            counter++;
        }
    }

    counter++;

    return counter;
}

int main() {

    setlocale(LC_ALL, "RUS");

    cout << "Введите путь к файлу с кодом: ";
    vector<string> lines = readLines(inputPathToFile());
    //vector<string> lines = readLines("code4.txt");

    int vertexNumber = findVertexNumber(lines);
    vertexNumber += 2;

    int edgesNumber = findEdgesNumber(lines);

    int condOperatorsNumber = findCondOperatorsNumber(lines);

    vector<regex> operatorsReg = readOperatorsReg("path1.txt");
    int operatorsNumber = findOperatorsNumber(lines, operatorsReg);

    replaceSwitch(lines);

    vector<int> nests = findNests(lines);
    int maxNest = *std::max_element(nests.begin(), nests.end()) - 1;

    int absoluteComplexity = findAbsoluteComplexity(lines);


    cout << "Вершины: " << vertexNumber << endl;
    cout << "Ребра: " << edgesNumber << endl;
    cout << "Условные операторы: " << condOperatorsNumber << endl;
    cout << "Операторы: " << operatorsNumber << endl;
    cout << "Максимальная вложенность: " << maxNest << endl;
    cout << "Абсолютная граничная сложность программы: " << absoluteComplexity;


    /*
    cout << "\nМетрики:\n";
    int McCabeMetric = edgesNumber - vertexNumber + 2;
    int CL = condOperatorsNumber;
    double cl = (double) CL / operatorsNumber;
    int CLI = maxNest;
    int Sa = absoluteComplexity;
    double So = 1 - ((vertexNumber - 1.0) / Sa);

    cout << "Метрика Маккейба Z(G): " << McCabeMetric << endl;
    cout << "Абсолютная сложность программы CL по метрике Джилба: " << CL << endl;
    cout << "Относительная  сложность программы cl по метрике Джилба: " << cl << endl;
    cout << "Максимальный уровень вложенности условного оператора CLI по метрике Джилба: " << CLI << endl;
    cout << "Метрика граничных значений (абсолютная граничная сложность программы): " << Sa << endl;
    cout << "Метрика граничных значений (относительная граничная сложность программы): " << So << endl;
    */
}