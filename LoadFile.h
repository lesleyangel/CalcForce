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
//	//state = 0  ��ȡ�ɹ�
//	//state = -1 ��ȡʧ�ܣ��ļ��в�����
//	//state = -2 ��ȡʧ�ܣ��ļ������ļ���ȡʧ��
//	int readTrajInfoFromPath(const string& path);//�����ļ���
//	
//	InitInfo getInitByTime(const double& time);
//	InitInfo getInitByHight(const double& h);
//	static vector<vector<double>>* readSingleFile(string path, vector<int> usfulCol, bool ifprintstate = false);
//private:
//	map<double, InitInfo> trajInfoList;//�洢��ȡ������ first ʱ��; secend trajInfo
//};

class LoadInput
{
public:
	enum class Suffix { txt, ini, cst };
	LoadInput(const string& path);
	int readFile();
	template <typename T>
	//ע�⣡��
	//LoadInput::get(const string& label, T && defaultValue) 
	//����Ϊ����ģ�壬����ֵ�ɵڶ��������Զ��Ƶ��ó�������
	//��ؼǵ� int �� double �����֣���������ֵΪstringʱ�ڶ���������Ҫд��string("xxxx")
	T get(const string& label, T&& defaultValue);//��ȡ�Ĳ�����ǩ����������Ĭ��ȱʡֵ
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
	if (iter != m_InfoList.end())//�����ֵ����£�û���򲻸���ʹ��ԭʼֵ
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
	// TODO: �ڴ˴����� return ���
}

template <>
inline string LoadInput::string2Type(string& str)
{
	return str;
}