#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
//#include "TrajInfo.h"
using namespace std;

//class LoadFile
//{
//public:
//	LoadFile() { trajInfoList.clear(); }
//	//state = 0  读取成功
//	//state = -1 读取失败，文件夹不存在
//	//state = -2 读取失败，文件夹内文件读取失败
//	int readTrajInfoFromPath(const string& path);//输入文件夹
//	
//	InitInfo getInitByTime(const double& time);
//	InitInfo getInitByHight(const double& h);
//	static vector<vector<double>>* readSingleFile(string path, vector<int> usfulCol, bool ifprintstate = false);
//private:
//	map<double, InitInfo> trajInfoList;//存储读取的数据 first 时间; secend trajInfo
//};

class LoadInput
{
public:
	enum class Suffix { txt, ini, cst };
	LoadInput(const string& path);
	int readFile();
	template <typename T>
	//注意！！
	//LoadInput::get(const string& label, T && defaultValue) 
	//函数为函数模板，返回值由第二个参数自动推导得出！！！
	//务必记得 int 和 double 的区分！！！返回值为string时第二个参数需要写成string("xxxx")
	T get(const string& label, T&& defaultValue);//读取的参数标签名，参数的默认缺省值
	template <typename T>
	void update(const string& label, T& value);
	
private:
	template <typename T>
	T string2Type(string& str);
	int calcSuffix();
	map<string, string> m_InfoList;
	string m_path;
	Suffix m_type;
};

template<typename T>
inline T  LoadInput::get(const string& label, T && defaultValue)
{
	map<string, string>::iterator iter = m_InfoList.find(label);
	if (iter != m_InfoList.end())
	{
		
		return string2Type<T>(iter->second);
	}
	return defaultValue;
}

template<typename T>
inline void LoadInput::update(const string & label, T & value)
{
	map<string, string>::iterator iter = m_InfoList.find(label);
	if (iter != m_InfoList.end())//如果有值则更新，没有则不更新使用原始值
	{
		value = string2Type<T>(iter->second);
	}
}

template<typename T>
inline T LoadInput::string2Type(string & str)
{
	T temp;
	stringstream(str) >> temp;
	return temp;
	// TODO: 在此处插入 return 语句
}

template <>
inline string LoadInput::string2Type(string& str)
{
	return str;
}