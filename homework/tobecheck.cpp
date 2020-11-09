﻿// homework.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include<fstream>
#include<vector>

#define ErrNumber   100
#define ErrClass   10

using namespace std;

const char* SourceFile = "C:\\Users\\Administrator\\Desktop\\1.txt";

class CGramCheck
{
private:
    int textline;   //文件总行数
    int errSum, err[ErrClass][ErrNumber];   //问题总数、问题数量
    vector<vector<int> >commentFlag;   //注释标记
public:
    CGramCheck();
    void GetTextline(ifstream& fin);            //获取文件总行数
    int CurlyBracesCheck(ifstream& fin);    //检查大括号错误，返回错误总数
    void CommentCheck(ifstream& fin);   //检查注释错误

};

int main()
{
    CGramCheck MyCheck;
    ifstream fin; ofstream fout;    //输入输出文件流

    fin.open(SourceFile);
    if (!fin) {
        cout << "Can't open file " << SourceFile << "!"<<endl;
        return -1;
    } 
    else {
        MyCheck.GetTextline(fin);
        MyCheck.CurlyBracesCheck(fin);
        //MyCheck.CommentCheck(fin);
        fin.close();
    }
}

CGramCheck::CGramCheck()
{
    textline = 0;
    errSum = 0;
    memset(err, 0, sizeof(err));
    vector<vector<int>>commentFlag(0, vector<int>(2));
}

void CGramCheck::GetTextline(ifstream& fin)
{
    string temp;
    while (getline(fin, temp)) { textline++; }
}
//初始化构造函数

int CGramCheck::CurlyBracesCheck(ifstream& fin) {
    string temp;
    int sum(0),line(0),j(0);
    while (getline(fin, temp)) {

        ++line;
        int char_p(0);
        for (auto i : temp) {
            char_p++;
            if (i == '{')sum++;
            if (i == '}')sum--;
            if (sum == -1) {
                err[0][j++] = line;
                sum = 0;
            }
        }
    }
    cout << "大括号错误行数：";
    for (int i = 0; i < ErrNumber; i++) {
        if (!err[0][i])break;
        cout << err[0][i] << ' ';
    }
    if (sum>0) {
        err[0][j] = line;
        cout << line << ' ';
   }
    return j + sum;
}

void CGramCheck::CommentCheck(ifstream& fin)
{
    commentFlag.reserve(textline);
    string temp;
    int line(0),flag(0);
    while (getline(fin, temp)) {
        ++line;
        for (int i = 0; i < temp.size();)
        {
            if (temp.find("/*",i) != -1) {
                flag++;
                cout << "/*位置是" << temp.find("/*", i) << endl;
                i += 2;
            }
        }

    }
}
