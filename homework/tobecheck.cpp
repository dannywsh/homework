// homework.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include<algorithm>

#define ErrNumber   100
#define ErrClass   10

using namespace std;

const char* SourceFile = "C:\\Users\\Administrator\\Desktop\\1.txt";
const char* TargetFile = "C:\\Users\\Administrator\\Desktop\\out.txt";
const char* numalp = "1234567890abcdefghijklmnopqrstuvwxyz";

class CGramCheck
{
private:
	int textline;   //文件总行数
	int errSum;	//错误总数
	vector<int>	curlyBracesError;
public:
	CGramCheck();
	void GetTextline(ifstream& fin);            //获取文件总行数
	void ParentheseCheck(ifstream& fin);	//检查括号错误，返回错误总数
	void CommentCheck(ifstream& fin);   //对注释内容进行检查，并找出错误 
	void SemicolonCheck(ifstream& fin);	//分号检查

};

int main()
{
	CGramCheck MyCheck;
	ifstream fin; ofstream fout;    //输入输出文件流
	fin.open(SourceFile);
	if (!fin) {
		cout << "Can't open file " << SourceFile << "!" << endl;
		return -1;
	}
	else {
		MyCheck.GetTextline(fin);
		MyCheck.ParentheseCheck(fin);
		MyCheck.CommentCheck(fin);
		fin.close();
	}
}
CGramCheck::CGramCheck()
{
	textline = 0;
	errSum = 0;
	vector<vector<int>>commentFlag(0, vector<int>(2));
}//初始化构造函数

void CGramCheck::GetTextline(ifstream& fin)
{
	string temp;
	while (getline(fin, temp)) { textline++; }
	fin.clear();
	fin.seekg(0, ios::beg);
}

void CGramCheck::ParentheseCheck(ifstream& fin)
{
	string temp;
	int line(0);
	vector<vector<int> >parentheseFlag(0);
	while (getline(fin, temp)) {
		++line;
		for (auto i : temp) {
			if (i == '(') {
				vector<int>temp(0);
				temp.push_back(line);
				temp.push_back('(');
				parentheseFlag.push_back(temp);
			}
			else if (i == '{') {
				vector<int>temp(0);
				temp.push_back(line);
				temp.push_back('{');
				parentheseFlag.push_back(temp);
			}
			else if (i == ')') {
				if (!parentheseFlag.size()) {
					cout << "缺失左括号：line " << line << endl;
				}
				else {
					while (parentheseFlag.back()[1] != '(') {
						cout << "缺失左花括号：" << endl;
						parentheseFlag.pop_back();
					}
					parentheseFlag.pop_back();
				}
			}
			else if (i == '}') {
				if (!parentheseFlag.size()) {
					cout << "缺失左括号：line " << line << endl;
				}
				else {
					while (parentheseFlag.back()[1] != '{') {
						cout << "缺失左⚪括号：" << endl;
						parentheseFlag.pop_back();
					}
					parentheseFlag.pop_back();
				}
			}
		}
		while (parentheseFlag.size()) {
			cout << "右括号缺失：line" << parentheseFlag.back()[0] << endl;
			parentheseFlag.pop_back();
		}
	}
	fin.clear();
	fin.seekg(0, ios::beg);
}
void CGramCheck::CommentCheck(ifstream& fin) {
	string temp;
	ofstream fout;
	bool doublequotes = false;
	bool singlequotes = false;
	bool comflag = false;

	fout.open(TargetFile, ios::out | ios::trunc);
	int line(0);

	while (getline(fin, temp)) {
		++line;
		int i(0);
		if (!temp.length()) {
			fout << endl;
			continue;
		}
		while (i < temp.length() - 1) {
			if (temp[i] == '\"') {
				if (!comflag) {
					if (doublequotes)doublequotes = false;
					fout << temp[i];
					i++;
				}
				else if (!doublequotes && comflag) {
					i++;
				}
				else {
					cout << "还有这种情况？？？" << endl;
				}
			}
			else if (temp[i] == '\'') {
				if (!comflag) {
					if (singlequotes)singlequotes = false;
					fout << temp[i];
					i++;
				}
				else if (!singlequotes && comflag) {
					i++;
				}
				else {
					cout << "还有这种情况？？？" << endl;
				}
			}
			else if (temp[i] == '/' && temp[i + 1] == '*') {
				if ((!doublequotes) && (!singlequotes) && !comflag) {
					i += 2;
					comflag = true;
				}
				else if ((!doublequotes) && (!singlequotes) && comflag) {
					i++;
				}
				else if (doublequotes || singlequotes) {
					fout << temp[i];
					i++;
				}
				else {
					cout << "还有这种情况？？？" << endl;
				}
			}
			else if (temp[i] == '*' && temp[i + 1] == '/') {
				if ((!doublequotes) && (!singlequotes) && !comflag) {
					cout << "前注释不匹配！";
					fout << temp[i];
					i++;
				}
				else if ((!doublequotes) && (!singlequotes) && comflag) {
					i += 2;
					comflag = false;
				}
				else if (doublequotes || singlequotes) {
					fout << temp[i];
					i++;
				}
				else {
					cout << "还有这种情况？？？" << endl;
				}
			}
			else if (temp[i] == '/' && temp[i + 1] == '/') {
				if ((!doublequotes) && (!singlequotes))break;
				else {
					fout << temp[i];
					i++;
				}
			}
			else {
				if (!comflag) {
					fout << temp[i];
				}
				i++;
			}
		}
		if (i == temp.length() - 1)fout << temp[i];
		fout << endl;
		if (doublequotes) {
			cout << "缺少双引号！";
			doublequotes = false;
		}
		if (singlequotes) {
			cout << "缺少单引号！";
			singlequotes = false;
		}
	}
	fout.close();
}
void CGramCheck::SemicolonCheck(ifstream& fin)
{
	string temp;
	while (getline(fin, temp)) {
		if (!temp.length()) { continue; }
		auto itor = remove(temp.begin(), temp.end(), ' ');
		temp.erase(itor, temp.end());
		if (temp.find(";") == -1) {
			if(temp.find_first_of(numalp)!=-1	|| temp.find("if") == -1|| temp.find("for") == -1 || temp.find("while") == -1 || temp.find("") == -1)
		}
	}
}
