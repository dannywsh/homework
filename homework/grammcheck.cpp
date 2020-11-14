﻿// homework.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include<algorithm>

#define ERR		1

#define LeftComment 0
#define RightComment 1

#define	Singlequotation 0
#define	Doublequotation 1

#define Leftparenth 0
#define Rightparenth 1
#define Leftbracket 2
#define Rightbracket 3

using namespace std;

const char* SrcFile = "C:\\Users\\Administrator\\Desktop\\1.cpp";
const char* TrgFile = "C:\\Users\\Administrator\\Desktop\\2.txt";

class CGramCheck
{
private:
	int textline;   //文件总行数
	int errSum;	//错误总数
	vector<vector<int> >	commentError;		//注释错误<[错误行数],[错误类型]>
	vector<vector<int> >	quotationError;		//单双引号错误<[错误行数],[错误类型]>
	vector<vector<int> >	bracketError;		//花圆括号错误<[错误行数],[错误类型]>
	vector<int>	semicolonError;		//引号错误<[错误行数]>
	vector<int>	calculateError;	//计算错误<[错误行数]>
	static bool ispareth(char i) { return i == ' ' || i == '\t' || i == '}'; }

public:
	CGramCheck();
	int GetTextline(const char* Sourcefile);            //获取文件总行数
	int CommentCheck(const char* Sourcefile, const char* Tempfile);   //对注释内容进行检查，并找出错误 
	int ParentheseCheck(const char* Tempfile);	//检查括号错误，返回错误总数
	int SemicolonCheck(const char* Tempfile);	//分号检查
	int CalculateCheck(const char* Tempfile);		//计算检查
	int OutputResult(const char* OutFile);	//输出结果
};

int main()
{
	CGramCheck MyCheck;
	MyCheck.GetTextline(SrcFile);
	MyCheck.CommentCheck(SrcFile, TrgFile);
	MyCheck.ParentheseCheck(TrgFile);
	MyCheck.SemicolonCheck(TrgFile);
	MyCheck.CalculateCheck(TrgFile);
	MyCheck.OutputResult(TrgFile);
}

CGramCheck::CGramCheck()
{
	textline = 0;
	errSum = 0;
}//初始化构造函数

int CGramCheck::GetTextline(const char* Sourcefile)
{
	string temp;
	ifstream fin;
	fin.open(Sourcefile);
	if (!fin) {
		cout << "Can't open file " << Sourcefile << "!" << endl;
		return ERR;
	}
	while (getline(fin, temp)) { textline++; }
	fin.close();
	return 0;
}

int CGramCheck::CommentCheck(const char* Sourcefile, const char* Tempfile) {
	string temp;
	bool doublequotes = false;
	bool singlequotes = false;
	bool comflag = false;
	int line(0);

	ifstream fin; ofstream fout;
	fin.open(Sourcefile);
	fout.open(Tempfile, ios::out | ios::trunc);
	if (!fin) {
		cout << "Can't open file " << Sourcefile << "!" << endl;
		return ERR;
	}
	if (!fout) {
		cout << "Can't open file " << Tempfile << "!" << endl;
		return ERR;
	}

	while (getline(fin, temp)) {
		++line;
		int i(0);
		if (!temp.length()) {
			fout << endl;
			continue;
		}
		while (i < temp.length() - 1) {
			if (temp[i] == '"') {
				if (!comflag) {
					if (singlequotes) {
						fout << temp[i];
						i++;
					}
					else {
						doublequotes = !doublequotes;
						fout << temp[i];
						i++;
					}
				}
				else {
					i++;
				}
			}
			else if (temp[i] == '\'') {
				if (!comflag) {
					if (doublequotes) {
						fout << temp[i];
						i++;
					}
					else {
						singlequotes = !singlequotes;
						fout << temp[i];
						i++;
					}
				}
				else {
					i++;
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
					//fout << temp[i];
					i++;
				}
				else {
					cout << "还有这种情况？？？" << endl;
				}
			}
			else if (temp[i] == '*' && temp[i + 1] == '/') {
				if ((!doublequotes) && (!singlequotes) && !comflag) {
					cout << "缺少/*" << endl; errSum++;
					vector<int>tp;
					tp.push_back(line);
					tp.push_back(LeftComment);
					commentError.push_back(tp);
					//fout << temp[i];
					i += 2;
				}
				else if ((!doublequotes) && (!singlequotes) && comflag) {
					i += 2;
					comflag = false;
				}
				else if (doublequotes || singlequotes) {
					//fout << temp[i];
					i++;
				}
				else {
					cout << "还有这种情况？？？" << endl;
				}
			}
			else if (temp[i] == '/' && temp[i + 1] == '/') {
				if ((!doublequotes) && (!singlequotes))break;
				else {
					//fout << temp[i];
					i++;
				}
			}
			else if (temp[i] == '\\') {
				i += 2;
			}
			else {
				if (!comflag) {
					fout << temp[i];
				}
				i++;
			}
		}
		if (i == temp.length() - 1) {
			if (temp[i] == '\"' && !comflag) {
				doublequotes = !doublequotes;
			}
			if (temp[i] == '\'' && !comflag) {
				singlequotes = !singlequotes;
			}
			if (doublequotes) {
				cout << "缺少双引号！" << line << endl; errSum++;
				vector<int>tp;
				tp.push_back(line);
				tp.push_back(Doublequotation);
				quotationError.push_back(tp);
				doublequotes = false;
			}
			else if (singlequotes) {
				cout << "缺少单引号！" << line << endl; errSum++;
				vector<int>tp;
				tp.push_back(line);
				tp.push_back(Singlequotation);
				quotationError.push_back(tp);
				singlequotes = false;
			}
			fout << temp[i];
		}
		fout << endl;
	}
	if (comflag) {
		cout << "缺少*/" << endl; errSum++;
		vector<int>tp;
		tp.push_back(line);
		tp.push_back(RightComment);
		commentError.push_back(tp);
	}
	fin.close();
	fout.close();
	return 0;
}

int CGramCheck::ParentheseCheck(const char* Tempfile)
{
	string temp;
	int line(0);
	vector<vector<int> >parentheseFlag(0);
	ifstream fin;
	fin.open(Tempfile);
	if (!fin) {
		cout << "Can't open file " << Tempfile << "!" << endl;
		return ERR;
	}
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
					vector<int>tmp(0);
					tmp.push_back(line);
					tmp.push_back(Rightbracket);
					bracketError.push_back(tmp);
					cout << "缺失左圆括号：line " << line << endl; errSum++;
				}
				else {
					if (parentheseFlag.back()[1] != '(') {
						vector<int>tmp(0);
						tmp.push_back(line);
						tmp.push_back(Rightbracket);
						bracketError.push_back(tmp);
						cout << "缺失左圆括号：line " << line << endl; errSum++;
					}
					else {
						parentheseFlag.pop_back();
					}
				}
			}
			else if (i == '}') {
				if (!parentheseFlag.size()) {
					vector<int>tmp(0);
					tmp.push_back(line);
					tmp.push_back(Leftparenth);
					bracketError.push_back(tmp);
					cout << "缺失左花括号：line " << line << endl; errSum++;
				}
				else {
					while (parentheseFlag.size() && parentheseFlag.back()[1] != '{') {
						vector<int>tmp(0);
						tmp.push_back(parentheseFlag.back()[0]);
						tmp.push_back(Rightbracket);
						bracketError.push_back(tmp);
						cout << "缺失右圆括号：" << line << endl; errSum++;
						parentheseFlag.pop_back();
					}
					if (!parentheseFlag.size()) {
						vector<int>tmp(0);
						tmp.push_back(line);
						tmp.push_back(Leftparenth);
						bracketError.push_back(tmp);
						cout << "缺失左花括号：line " << line << endl; errSum++;
					}
					else {
						parentheseFlag.pop_back();
					}
				}
			}
		}
	}
	while (parentheseFlag.size()) {
		vector<int>tmp(0);
		tmp.push_back(parentheseFlag.back()[0]);
		if (parentheseFlag.back()[1] == Leftparenth)
		{
			tmp.push_back(Rightparenth);
		}
		else
		{
			tmp.push_back(Rightbracket);
		}
		bracketError.push_back(tmp);
		cout << "缺失相对应的右" << char(parentheseFlag.back()[1]) << "括号：line" << parentheseFlag.back()[0] << endl; errSum++;
		parentheseFlag.pop_back();
	}
	fin.close();
}

int CGramCheck::SemicolonCheck(const char* Tempfile)
{
	string temp; int line(0);
	ifstream fin;
	fin.open(Tempfile);
	if (!fin) {
		cout << "Can't open file " << Tempfile << "!" << endl;
		return ERR;
	}
	while (getline(fin, temp)) {
		++line;
		if (!temp.length()) { continue; }
		temp.erase(remove_if(temp.begin(), temp.end(), ispareth), temp.end());
		if (temp.length() && temp.back() != ';')
		{
			if (temp.find("if") == -1
				&& temp.find("while") == -1
				&& temp.find("else") == -1
				&& temp.find("class") == -1
				&& temp.find("public") == -1
				&& temp.find("private") == -1
				&& temp.find("protected") == -1
				&& temp.find("main") == -1
				&& temp.find("struct") == -1
				&& temp.find_first_of("{}#()") == -1) {
				semicolonError.push_back(line);
				cout << "缺少分号:line" << line << endl; errSum++;
			}
		}
	}
	fin.close();
	return 0;
}

int CGramCheck::CalculateCheck(const char* Tempfile)
{
	string temp;
	ifstream fin;
	int line(0);
	vector<char>symbolstack;
	vector<char>numstack;
	fin.open(Tempfile);
	if (!fin) {
		cout << "Can't open file " << Tempfile << "!" << endl;
		return ERR;
	}
	while (getline(fin, temp)) {
		++line;
		bool numflag = false;
		int p = temp.find("=");
		if (p != -1 && p != temp.length() - 1 && temp[p + 1] != '=') {
			p++;
			if (p >= temp.length())continue;
			while (p < temp.length()) {
				if (isalnum(temp[p])) {
					if (!symbolstack.empty()) { symbolstack.pop_back(); }
					else if (!numflag) {
						numstack.push_back(temp[p]);
						numflag = true;
					}
				}
				else if (temp[p] == '+' || temp[p] == '-' || temp[p] == '*' || temp[p] == '/') {
					numflag = false;
					if (numstack.empty()) {
						cout << "缺少运算前项"<<line<<endl; errSum++;
						calculateError.push_back(line);
					}
					else if (!symbolstack.empty()) {
						cout << "缺少前加项"<<line<<endl; errSum++;
						calculateError.push_back(line);
					}
					else {
						symbolstack.push_back(temp[p]);
					}
				}
				else {}
				p++;
			}
		}
		else {
			continue;
		}
		if (!symbolstack.empty()) {
			cout << "缺少运算后值" << line<<endl; errSum++;
			calculateError.push_back(line);
		}
		symbolstack.clear();
		numstack.clear();
	}
	fin.close();
	return 0;
}

int CGramCheck::OutputResult(const char* OutFile)
{
	ofstream fout;
	fout.open(OutFile, ios::trunc);
	if (!fout) {
		cout << "Can't open file " << OutFile<< "!" << endl;
		return ERR;
	}
	fout << "该程序代码总行数为：" << textline << endl;
	fout << "该程序错误总数为：" << errSum << endl;
	fout << endl;

	fout << "注释错误为：" << endl;
	for (auto i : commentError) {
		if (i[1] == LeftComment)fout << "缺少左注释符号：\tline：" << i[0] << endl;
		if (i[1] == RightComment)fout << "缺少右注释符号：\tline：" << i[0] << endl;
	}
	fout << endl;

	fout << "引号错误为：" << endl;
	for (auto i : quotationError) {
		if (i[1] == Singlequotation)fout << "缺少单引号：\tline：" << i[0] << endl;
		if (i[1] == Doublequotation)fout << "缺少双引号：\tline：" << i[0] << endl;
	}
	fout << endl;

	fout << "括号错误为：" << endl;
	for (auto i : bracketError) {
		if (i[1] == Leftbracket)fout << "缺少左圆括号：\tline：" << i[0] << endl;
		if (i[1] == Rightbracket)fout << "缺少右圆符号：\tline：" << i[0] << endl;
		if (i[1] == Leftparenth)fout << "缺少左花符号：\tline：" << i[0] << endl;
		if (i[1] == Rightparenth)fout << "缺少右花符号：\tline：" << i[0] << endl;
	}
	fout << endl;

	fout << "分号错误为：" << endl;
	for (auto i : semicolonError) {
		fout << "缺少分号：\tline：" << i << endl;
	}
	fout << endl;

	fout << "计算错误为："<< endl;
	for (auto i : calculateError) {
		fout << "计算错误：\tline：" << i << endl;
	}
	fout << endl;
	fout.close();
	return 0;
}