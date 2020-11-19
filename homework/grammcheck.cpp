//题目：C++源程序语法检查器

//本程序实现了以下基本语法检查功能：
//①	花括号配对情况；
//②	分号缺失情况；
//③	运算符符合运算规则情况；
//④	引号（单、双）配对情况；
//⑤	块注释匹配情况（/**/）
//⑥	圆括号匹配情况；

//额外实现功能：
//①	尖括号配对情况；
//②	冒号缺失情况；
//③	行注释（//）规则情况；

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

#define NumberSign 0
#define Leftangle 1
#define Rightangle 2

using namespace std;

const char* SrcFile = "tobecheck.cpp";
const char* TrgFile = "checkres.txt";

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
	vector<int>	colonError;	//冒号错误<[错误行数]>
	vector<vector<int> >	angleError;		//尖括号错误<[错误行数],[错误类型]>
	static bool ispareth(char i) { return i == ' ' || i == '\t' || i == '}'; }

public:
	CGramCheck();
	int GetTextline(const char* Sourcefile);            //获取文件总行数
	int CommentCheck(const char* Sourcefile, const char* Tempfile);   //对注释内容进行检查，并找出引号错误 
	int ParentheseCheck(const char* Tempfile);	//检查括号（圆、花）错误，返回错误总数
	int SemicolonCheck(const char* Tempfile);	//分号检查
	int CalculateCheck(const char* Tempfile);		//计算检查
	int ColonCheck(const char* Tempfile);		//冒号检查
	int AnglebracketCheck(const char* Tempfile);	//尖括号检查
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
	MyCheck.ColonCheck(TrgFile);
	MyCheck.AnglebracketCheck(TrgFile);
	MyCheck.OutputResult(TrgFile);
	return 0;
}

//初始化构造函数
CGramCheck::CGramCheck()
{
	textline = 0;
	errSum = 0;
}

//获取文件总行数
int CGramCheck::GetTextline(const char* Sourcefile)
{
	string temp;
	ifstream fin;
	fin.open(Sourcefile);
	if (!fin) {
		cout << "Can't open file " << Sourcefile << "!" << endl;
		return ERR;
	}//打开文件
	while (getline(fin, temp)) { textline++; }	//获取行数
	fin.close();
	return 0;
}

//检查引号，注释情况，返回错误总数
int CGramCheck::CommentCheck(const char* Sourcefile, const char* Tempfile) {
	string temp;
	bool doublequotes = false;		//双引号标志位
	bool singlequotes = false;		//单引号标志位
	bool comflag = false;				//注释标志位
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
	}//打开文件并判断

	while (getline(fin, temp)) {
		++line;		//行数
		int i(0);
		if (!temp.length()) {
			fout << endl;
			continue;
		}
		while (i < temp.length() - 1) {
			if (temp[i] == '"') {	//判断双引号情况
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
			else if (temp[i] == '\'') {	//判断单引号情况
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
			else if (temp[i] == '/' && temp[i + 1] == '*') {	//判断块前注释情况
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
			}	//判断括号是否在注释内
			else if (temp[i] == '*' && temp[i + 1] == '/') {//判断块后注释情况
				if ((!doublequotes) && (!singlequotes) && !comflag) {
					cout << "缺少/*" << endl; errSum++;
					vector<int>tp;
					tp.push_back(line);
					tp.push_back(LeftComment);
					commentError.push_back(tp);		//对错误进行压栈操作
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
		if (i == temp.length() - 1) {		//判断标志位将错误与行数压入栈中
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
	fout.close();		//关闭文件
	return 0;
}

//检查括号（圆、花）错误，返回错误总数
int CGramCheck::ParentheseCheck(const char* Tempfile)
{
	string temp;
	int line(0);
	vector<vector<int> >parentheseFlag(0);	//符号栈，用于括号匹配
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
				parentheseFlag.push_back(temp);	//符号压栈
			}
			else if (i == '{') {
				vector<int>temp(0);
				temp.push_back(line);
				temp.push_back('{');
				parentheseFlag.push_back(temp);//符号压栈
			}
			else if (i == ')') {
				if (!parentheseFlag.size()) {
					vector<int>tmp(0);
					tmp.push_back(line);
					tmp.push_back(Rightbracket);	//判断圆括号是否匹配，并进行出栈操作
					bracketError.push_back(tmp);		//记录错误
					cout << "缺失左圆括号：line " << line << endl; errSum++;	//控制台输出错误类型，并记录错误总数
				}
				else {
					if (parentheseFlag.back()[1] != '(') {
						vector<int>tmp(0);
						tmp.push_back(line);
						tmp.push_back(Rightbracket);
						bracketError.push_back(tmp);
						cout << "缺失左圆括号：line " << line << endl; errSum++;//控制台输出错误类型，并记录错误总数
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
					cout << "缺失左花括号：line " << line << endl; errSum++;//控制台输出错误类型，并记录错误总数
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
	return 0;
}

//分号检查
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
		temp.erase(remove_if(temp.begin(), temp.end(), ispareth), temp.end());	//对字符串进行处理
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
					if(temp.find("namespace") == -1
						&& temp.find("int") == -1
						&& temp.find("float") == -1
						&& temp.find("double") == -1
						&& temp.find("short") == -1
						&& temp.find("long") == -1
						&& temp.find("bool") == -1
						&& temp.find("char") == -1
						&& temp.find("return") == -1
						&& temp.find("cout") == -1
						&& temp.find("cin") == -1
						){ }		//关键字查找
					else {
						semicolonError.push_back(line);
						cout << "缺少分号:line" << line << endl; errSum++;
					}
			}
		}
	}
	fin.close();
	return 0;
}

//计算检查
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
		bool numflag = false;		//运算成员标志位
		int p = temp.find("=");	//寻找赋值“=”号
		if (p != -1 && p != temp.length() - 1 && temp[p + 1] != '=') {	//判断是否为“==”比较符
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

//冒号检查
int CGramCheck::ColonCheck(const char* Tempfile)
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
		if(temp.find("private")!=-1|| temp.find("public")!=-1||temp.find("protected")!=-1){
			if (temp.find(":") == -1) {
				cout << "缺少冒号！" << line<<endl; errSum++;
				colonError.push_back(line);
			}
		}
	}
	fin.close();
	return 0;
}

//尖括号检查
int CGramCheck::AnglebracketCheck(const char* Tempfile)
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
		if (temp.find("include") != -1) {
			if (temp.find("#") == -1) {
				cout << "缺少＃号！" << line << endl; errSum++;
				vector<int>tmp(0);
				tmp.push_back(line);
				tmp.push_back(NumberSign);
				angleError.push_back(tmp);
			}
			if (temp.find("<") == -1) {
				cout << "缺少左尖括号！" << line << endl; errSum++;
				vector<int>tmp(0);
				tmp.push_back(line);
				tmp.push_back(Leftangle);
				angleError.push_back(tmp);
			}
			if (temp.find(">") == -1) {
				cout << "缺少右尖括号！" << line << endl; errSum++;
				vector<int>tmp(0);
				tmp.push_back(line);
				tmp.push_back(Rightangle);
				angleError.push_back(tmp);
			}
		}
	}
	fin.close();
	return 0;
}

//输出错误结果
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

	fout << "冒号错误为：" << endl;
	for (auto i : colonError) {
		fout << "缺少冒号：\tline：" << i << endl;
	}
	fout << endl;

	fout << "尖括号错误为：" << endl;
	for (auto i : angleError) {
		if (i[1] == NumberSign)fout << "缺少＃号：\tline：" << i[0] << endl;
		if (i[1] == Rightangle)fout << "缺少右尖符号：\tline：" << i[0] << endl;
		if (i[1] == Leftangle)fout << "缺少左尖符号：\tline：" << i[0] << endl;
	}
	fout << endl;

	fout.close();
	return 0;
}