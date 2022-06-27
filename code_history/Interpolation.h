/*
��ֵ����V1.2.3
Nά��ֵ���������ֱ�׼�����ļ���ʽ�Ĳ�ֵ��֧����Ԫ�������ʽ��������������֧����ά�ȱ�ͷ����Ϊ1�������֧�������ֵ���Զ�ʶ��˳������
������һά��ֵ�Ͷ�ά��ֵΪͨ�ø�ʽ��Ч����Nά��ֵ����,������vector��֧�������ֵ���Զ�ʶ��˳������
��������ά����ά��ֵ�ڲ�ֵ�㳬������ʱ�ᱨ����Ч�ʱ�Nά��ֵ�ͣ���֧�������ֵ��

���ڣ�2019.12.02
*/
#pragma once
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
using namespace std;
#define DLLEXPORT __declspec(dllexport)
//  ��ֵ����������


class DLLEXPORT Interpolation
{
public:
	Interpolation(void);
	~Interpolation(void);

public:
	//��ʼ�����ݣ����ַ�ʽ 1����txt��2���ֶ�����
	bool InterpTableRead(string FileName);                  // ��ȡ�ϸ�ʽ��ֵ��׼������
	bool InterpTableReadFile(string FileName);			    //	��ȡ��ֵ��txt���¸�ʽ
	bool InterpTableReadData(string strdata);               //  ��ȡ�ַ������¸�ʽ
	
	bool SetHeader(vector<vector<double>> header,vector<string> headername);//�ֶ����ñ�ͷ
	bool SetData(vector<int> cubeorder, vector<vector<double>> cubedata);   //�ֶ���������
	
	//��ֵ������
	double InterpolationN(const vector<double>& x, bool out = false);//	��ֵ����,out=1������壬=0���������

	//��ȡ����
	vector<vector<double>> GetData(const vector<int>& cubeorder);//��ȡָ��λ�õ����ݾ���
	int GetDim();											        //	��ȡά��
	const vector<int>& GetHeaderLength();							//	��ȡ��ͷ����
	const vector<vector<double>>& GetHeader();						//	��ȡ��ͷ
	const vector<string>& GetHeaderName();							//	��ȡ��ͷ����
	bool WriteTxt(string txtpath);//����д�ɱ�׼��ʽ
	void CreateZeroInterp();//��һ��Ϊ0�Ĳ�ֵ��
	int Check();  // ���ݼ��
public:	 //  for serializing
	bool IfOut;						//	IfOut=1������壬=0���������
	bool Ifturn;					//	���Ϊture��������Ĳ�ֵ���������
	int	m_Dim;			            //	ά��
	vector<int> m_HeaderLength;		//	��ά�б���
	vector<vector<double>> m_Header;//	��ά�б�
	vector<double> m_data;			//	����
	vector<string> m_HeaderName;    //  ��ͷ����
private:
	bool CheckTable();	//	����ͷ�Ƿ��������У����data�������ͷ�Ƿ�ƥ��
	double Interp1(double x1, double x2, double y1, double y2, double x);//	һά��ֵ����
	double InterpN(int start, const vector<double>& x, int Dim, const vector<vector<int>>& XNum, const vector<vector<double>>& XVal);//	nά��ֵ����
	void WriteCube(int n,fstream &ff); //дһ�����������
	
//��ʱ���õģ������һ���������ݸ�ʽ
private:
	void ReadAero(const char *FileName,int n);	//���µ������ļ�,nΪ��ȡ������ֵ���ݵ�����-1,�����Ԫ�������������ʽ
	void InterpTableRead2(const char *FileName,int n);	//֧�����������ReadAreo,nΪ��ȡ������ֵ���ݵ�����-1
	int FindPosition(const vector<double>& head); //InterpTableRead2�ã�Ѱ�Ҳ嵽m_AllData�еĵط�
	vector<double> Shorten(const vector<double>& a); //���̱�ͷ��	
public:
	vector<vector<double>> m_AllData;	//InterpTableRead2�����������ݴ�������
};

//////////////////////////////////////////////////////////////////////////
//һά��ֵ
double Interpolation1(int n_Xs, double *Xs,double *Data, double X );
extern "C" DLLEXPORT double Interpolation1(const vector<double> &Xs,const vector<double>& Data, double X );
//��ά��ֵ
double Interpolation2(int n_Xs, double *Xs, int n_Ys, double *Ys, double *Data, double X, double Y);
extern "C" DLLEXPORT double Interpolation2(const vector<double> &Xs, const vector<double> &Ys, const vector<double> &Data, double X, double Y);
//��ά��ֵ
double Interpolation3(int n_Xs, double *Xs, int n_Ys, double *Ys, int n_Zs, double *Zs, double *Data, double X, double Y, double Z );
extern "C" DLLEXPORT double Interpolation3(const vector<double> &Xs, const vector<double> &Ys, const vector<double> &Zs, const vector<double> &Data, double X, double Y, double Z );
//��ά��ֵ                 
double Interpolation4(int n_X1, double *X1s, int n_X2, double *X2s, int n_X3, double *X3s, int n_X4s, double *X4s, double *Data, double X1, double X2, double X3, double X4);
extern "C" DLLEXPORT double Interpolation4(const vector<double> &X1s, const vector<double> &X2s, const vector<double> &X3s, const vector<double> &X4s, const vector<double> &Data, double X1, double X2, double X3, double X4);


/*
������־
V1.2~V1.2.1��20190325
�޸���double���͵ĵ���0�ж�
�Ľ��˱�ͷ����Ϊ1ʱNά��ֵ�Ĵ���ʽ

V1.2.1~V1.2.2��20190326
�Ľ���InterpTableRead��ȡЧ�ʣ�������data���ȴδ�С�ж�

V1.2.2~V1.2.3��20191202
�����˰Ѷ������������ɱ�׼����txt�ĺ���
*/