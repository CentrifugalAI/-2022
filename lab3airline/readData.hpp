#ifndef READDATA_H
#define READDATA_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;

class ReadData
{
public:
    vector<vector<string>>* operator()();
};

vector<vector<string>>* ReadData::operator()()
{
    ifstream inFile("I://GitHub//homeworks//Arithmetic//homework//lab3airline//data.csv", ios::in);
    if (!inFile)
    {
        cout << "fail to open file !!!";
        exit(1);
    }
    vector<vector<string>>* strArray = new vector<vector<string>>();
    string wholeLine;
    while (getline(inFile, wholeLine))
    {
        // cout << wholeLine << endl;
        stringstream _wholeLine(wholeLine); // 字符串流类型，用 wholeLine 初始化 ss
        string str;
        vector<string> single_line_vec; // 向量形式的单行变量
        //按照逗号进行分割
        while (getline(_wholeLine, str, ',')) // 把每次把按照逗号分割之后的每一个字符串都保存在 str 中
            single_line_vec.push_back(str);
        strArray->push_back(single_line_vec); // 载入单行向量
    }
    // cout << "--------------------------------------------------print what we have saved--------------------------------------------------";
    // for (auto vec : *strArray)
    // {
    //     for (auto ele : vec)
    //         cout << ele << " ";
    //     cout << endl;
    // }
    return strArray;
}

#endif