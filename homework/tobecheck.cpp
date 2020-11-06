// homework.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include<fstream>

#define ErrNumber   100
#define ErrClass   10

using namespace std;

const char* SourceFile = "C:\\Users\\Administrator\\Desktop\\1.txt";

class CGramCheck
{
private:
    int textline;   //文件总行数
    int errSum, err[ErrClass][ErrNumber];   //问题总数、问题数量

public:
    CGramCheck();
    int CurlyBracesMatch(ifstream& fin);    //检查大括号错误，返回错误总数
};

int main()
{
    CGramCheck MyCheck;
    ifstream fin; ofstream fout;    //输入输出文件流

    fin.open(SourceFile);
    if (!fin) {
        cout << "Can't open file " << SourceFile << "!"<<endl;
        getchar();
        return -1;
    }   //打开文件
    else {
        MyCheck.CurlyBracesMatch(fin);
    }
}

CGramCheck::CGramCheck()
{
    textline = 0;
    errSum = 0;
    memset(err, 0, sizeof(err));
}//初始化构造函数

int CGramCheck::CurlyBracesMatch(ifstream& fin) {
    string temp;
    int sum(0),line(0),j(0);
    while (getline(fin, temp)) {

        ++line;
        for (auto i : temp) {
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
