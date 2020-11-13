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

#define	Singlequotation 0
#define	Doublequotation 1

#define Leftparenth 0
#define Rightparenth 1
#define Leftbracket 2
#define Rightbracket 3

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
	vector<vector<int> >	commentError;		//注释错误<[错误行数],[错误类型]>
	vector<vector<int> >	quotationError;		//单双引号错误<[错误行数],[错误类型]>
	vector<vector<int> >	bracketError;		//花圆括号错误<[错误行数],[错误类型]>
	vector<int>	semicolonError;		//引号错误<[错误行数]>
	vector<int>	calculateError;	//计算错误<[错误行数]>

public:
	CGramCheck();
	int GetTextline(const char* Sourcefile);            //获取文件总行数
	int CommentCheck(const char* Sourcefile, const char* Tempfile);   //对注释内容进行检查，并找出错误 
	int ParentheseCheck(const char* Tempfile);	//检查括号错误，返回错误总数
	int SemicolonCheck(const char* Tempfile);	//分号检查
	int CalculateCheck(const char* Tempfile);		//计算检查
	static bool ispareth(char i) { return i == ' ' || i == '\t' || i == '}'; }
};

int main()
{
	CGramCheck MyCheck;
	MyCheck.GetTextline(SrcFile);
	MyCheck.CommentCheck(SrcFile, TrgFile);
	MyCheck.ParentheseCheck(TrgFile);
	MyCheck.SemicolonCheck(TrgFile);
	MyCheck.CalculateCheck(TrgFile);
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
					cout << "缺少/*" << endl;
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
				cout << "缺少双引号！" << line << endl;
				vector<int>tp;
				tp.push_back(line);
				tp.push_back(Doublequotation);
				quotationError.push_back(tp);
				doublequotes = false;
			}
			else if (singlequotes) {
				cout << "缺少单引号！" << line << endl;
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
		cout << "缺少*/" << endl;
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
					cout << "缺失左园括号：line " << line << endl;
				}
				else {
					if (parentheseFlag.back()[1] != '(') {
						vector<int>tmp(0);
						tmp.push_back(line);
						tmp.push_back(Rightbracket);
						bracketError.push_back(tmp);
						cout << "缺失左园括号：line " << line << endl;
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
					cout << "缺失左花括号：line " << line << endl;
				}
				else {
					while (parentheseFlag.size() && parentheseFlag.back()[1] != '{') {
						vector<int>tmp(0);
						tmp.push_back(parentheseFlag.back()[0]);
						tmp.push_back(Rightbracket);
						bracketError.push_back(tmp);
						cout << "缺失右圆括号：" << line << endl;
						parentheseFlag.pop_back();
					}
					if (!parentheseFlag.size()) {
						vector<int>tmp(0);
						tmp.push_back(line);
						tmp.push_back(Leftparenth);
						bracketError.push_back(tmp);
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
		cout << "缺失相对应的右" << char(parentheseFlag.back()[1]) << "括号：line" << parentheseFlag.back()[0] << endl;
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
				cout << "缺少分号:line" << line << endl;
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
	fin.open(Tempfile);
	if (!fin) {
		cout << "Can't open file " << Tempfile << "!" << endl;
		return ERR;
	}
	while (getline(fin, temp)) {
		++line;
		int i(0);
		while (i < temp.length() - 1) {
			if (temp[i] == '') {
				if (temp[i + 1] == '+' || temp[i + 1] == '-' || temp[i + 1] == '*' || temp[i + 1] == '/') {
					cout << "运算符前无数字";
					calculateError.push_back(line);
					i++; 
				}
				else {
					i++;
				}
			}
			else if (temp[i] == '+') {
				if (temp[i + 1] == '+') {
					if (i + 2 < temp.length() && isalnum(temp[i + 2])) {
						cout << "自加后有数字";
						calculateError.push_back(line);
						i += 2;
					}
				}
				else if (temp[i + 1] == '-' || temp[i + 1] == '*' || temp[i + 1] == '/') {
					cout << "双运算符不匹配";
					calculateError.push_back(line);
				}
				else if (!isalnum(temp[i + 1])) {
					cout << "运算符后无数字";
					calculateError.push_back(line);
					i++;
				}
				else {
					i++;
				}
			}
			else if (temp[i] == '-') {
				if (temp[i + 1] == '-') {
					if (i + 2 < temp.length() && isalnum(temp[i + 2])) {
						cout << "自减后有数字";
						calculateError.push_back(line);
						i += 2;
					}
				}
				else if (temp[i + 1] == '+' || temp[i + 1] == '*' || temp[i + 1] == '/') {
					cout << "双运算符不匹配";
					calculateError.push_back(line);
				}
				else if (!isalnum(temp[i + 1])) {
					cout << "运算符后无数字";
					calculateError.push_back(line);
					i++;
				}
				else {
					i++;
				}
			}
			else if (temp[i] == '*' || temp[i] == '/') {
				if (temp[i + 1] == '+' || temp[i + 1] == '-' || temp[i + 1] == '*' || temp[i + 1] == '/') {
					cout << "双运算符不匹配";
					calculateError.push_back(line);
				}
				else if (!isalnum(temp[i + 1])) {
					cout << "运算符后无数字";
					calculateError.push_back(line);
					i++;
				}
				else {
					i++;
				}
			}
			else {
				i++;
			}
		}
		if (i == temp.length() - 1) {
			if (temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/') {
				cout << "运算符后无数字";
				calculateError.push_back(line);
			}
		}
	}
	fin.close();
	return 0;
}