// homework.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include<fstream>
#include<vector>
#include<algorithm>

#define ErrNumber   100
#define ErrClass   10
#define ERR		1

#define LeftComment 0
#define RightComment 1

using namespace std;

const char* SrcFile = "C:\\Users\\Administrator\\Desktop\\1.txt";
const char* TrgFile = "C:\\Users\\Administrator\\Desktop\\2.txt";
const char* OutFile = "C:\\Users\\Administrator\\Desktop\\out.txt";

class CGramCheck
{
private:
	int textline;   //文件总行数
	int errSum;	//错误总数
	vector<int>	curlyBracesError;
	vector<vector<int> >	commentError;
public:
	CGramCheck();
	int GetTextline(const char* Sourcefile);            //获取文件总行数
	int CommentCheck(const char* Sourcefile, const char* Tempfile);   //对注释内容进行检查，并找出错误 
	int ParentheseCheck(const char* Tempfile);	//检查括号错误，返回错误总数
	int SemicolonCheck(const char* Tempfile);	//分号检查
	static bool isblank(char i) { return i == ' ' || i == '\t'; }
};

int main()
{
	CGramCheck MyCheck;
	/*
	fin.open(SourceFile);
	fout.open(OutputFile, ios::out | ios::trunc);
	if (!fin) {
		cout << "Can't open file " << SourceFile << "!" << endl;
		return -1;
	}
	if (!fout) {
		cout << "Can't open file " << OutputFile << "!" << endl;
		return -1;
	}
	*/
	MyCheck.GetTextline(SrcFile);
	MyCheck.CommentCheck(SrcFile,TrgFile);
	MyCheck.ParentheseCheck(TrgFile);
	MyCheck.SemicolonCheck(TrgFile);
}

CGramCheck::CGramCheck()
{
	textline = 0;
	errSum = 0;
	vector<vector<int>>commentFlag(0, vector<int>(2));
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
	ifstream fin;ofstream fout;

	bool doublequotes = false;
	bool singlequotes = false;
	bool comflag = false;
	int line(0);

	fin.open(Sourcefile);
	fout.open(Tempfile, ios::out | ios::trunc);
	if (!fin) {
		cout << "Can't open file " << Sourcefile << "!" << endl;
		return -1;
	}
	if (!fout) {
		cout << "Can't open file " << Tempfile << "!" << endl;
		return -1;
	}

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
					if (singlequotes) {
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
					cout << "缺少/*"<<endl;
					vector<int>tp;
					tp.push_back(line);
					tp.push_back(LeftComment);
					commentError.push_back(tp);
					fout << temp[i];
					i++;
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
				if (!comflag && !singlequotes && !doublequotes) {
					fout << temp[i];
				}
				i++;
			}
		}
		if (i == temp.length() - 1)fout << temp[i];
		fout << endl;
		if (doublequotes) {
			cout << "缺少双引号！" << line << endl;
			doublequotes = false;
		}
		if (singlequotes) {
			cout << "缺少单引号！" << line << endl;
			singlequotes = false;
		}
	}
	if (comflag) {
		cout << "缺少*/" << endl;
		vector<int>tp;
		tp.push_back(line);
		tp.push_back(RightComment);
		commentError.push_back(tp);
	}
	fin.close();
	fout.close();
}

void CGramCheck::ParentheseCheck(const char* Tempfile, const char* Outputfile)
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
					cout << "缺失左园括号：line " << line << endl;
				}
				else {
					while (parentheseFlag.size() && parentheseFlag.back()[1] != '(') {
						cout << "缺失右花括号：line" << line << endl;
						parentheseFlag.pop_back();
					}
					if (!parentheseFlag.size()) {
						cout << "缺失左园括号：line " << line << endl;
					}
					else {
						parentheseFlag.pop_back();
					}
				}
			}
			else if (i == '}') {
				if (!parentheseFlag.size()) {
					cout << "缺失左花括号：line " << line << endl;
				}
				else {
					while (parentheseFlag.size() && parentheseFlag.back()[1] != '{') {
						cout << "缺失右园括号：" <<line << endl;
						parentheseFlag.pop_back();
					}
					if (!parentheseFlag.size()) {
						cout << "缺失左花括号：line " << line << endl;
					}
					else {
						parentheseFlag.pop_back();
					}
				}
			}
		}
	}
	while (parentheseFlag.size()) {
		cout << "右" << char(parentheseFlag.back()[1]) << "缺失：line" << parentheseFlag.back()[0] << endl;
		parentheseFlag.pop_back();
	}
	fin.clear();
	fin.seekg(0, ios::beg);
}

void CGramCheck::SemicolonCheck(const char* Tempfile, const char* Outputfile)
{
	string temp; int line(0);
	while (getline(fin, temp)) {
		++line;
		if (!temp.length()) { continue; }
		temp.erase(remove_if(temp.begin(), temp.end(),isblank), temp.end());
		if (temp.find(";") == -1&& temp.length()) {
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
				cout << "缺少分号:line"<<line<<endl;
			}
		}
	}
	fin.clear();
	fin.seekg(0, ios::beg);
}