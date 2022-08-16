#pragma once
//namespace SplineSpace
//{
class /*__declspec(dllexport)*/SplineFailure		//�쳣��
{
private:
	const char* Message;
public:
	SplineFailure(const char* msg);
	const char* GetMessage();
};

class /*__declspec(dllexport)*/SplineInterface		//�ӿ���
{
public:
	//�����ֵ:
	//x->�����Ա���(����)
	//y->���ز�ֵ���(���)
	//���x�����ڲ�ֵ�����ֵʧ��
	virtual bool SinglePointInterp(const double& x, double& y) = 0;
	//����ֵ:
	//x->�����Ա���������ָ��,num->�Ա�������������(����)
	//y->���ز�ֵ�������ָ��(�������)
	virtual bool MultiPointInterp(const double* x, const int& num, double* y) = 0;
	//�Զ���ֵ:
	//num->��Ҫ���ɵĲ�ֵ����������(����)
	//x->����Ա����������ָ��(�������)
	//y->��Ӧ��Ͻ������ָ��(�������)
	virtual bool AutoInterp(const int& num, double* x, double* y) = 0;

	virtual double getMaxX() = 0;
	virtual double getMinX() = 0;

	virtual ~SplineInterface() {};
};

enum class BoundaryCondition		//�߽�����ö��
{
	GivenFirstOrder = 1		//����һ�׵���
	, GivenSecondOrder		//�������׵���
};

class /*__declspec(dllexport)*/Spline :public SplineInterface				//ʵ����
{
public:
	//���캯��:
	//x0->��֪���Ա���������ָ��,y0->��֪�������������ָ��,num->��֪���ݵĸ���
	//bc->�߽�����,Ĭ�ϸ������׵���
	//leftBoundary,rightBoundary���ұ߽絼��ֵ������,Ĭ�϶���0
	Spline(const double* x0, const double* y0, const int& num,
		BoundaryCondition bc = BoundaryCondition::GivenSecondOrder, const double& leftBoundary = 0, const double& rightBoundary = 0);

	bool SinglePointInterp(const double& x, double& y)throw(SplineFailure);		//�麯��ʵ��
	bool MultiPointInterp(const double* x, const int& num, double* y)throw(SplineFailure);
	bool AutoInterp(const int& num, double* x, double* y)throw(SplineFailure);
	double getMaxX() { return MaxX; }
	double getMinX() { return MinX; }
	~Spline();

private:
	//����
	void PartialDerivative1(void);		//��һ�׵�
	void PartialDerivative2(void);		//����׵�
										//����
	const double* GivenX;	//��֪���ݵ��Ա���
	const double* GivenY;	//��֪���ݵ������
	const int GivenNum;		//��������֪���ݵ���
	const BoundaryCondition Bc;	//�߽�����
	const double LeftB;
	const double RightB;

	double* PartialDerivative;	//�����Ӧ�Ķ��׻���һ��ƫ����
	double MaxX;				//�Ա��������ֵ
	double MinX;				//�Ա�������Сֵ

	const double* SplineX;	//��ֵ�Ա���
	double* SplineY;		//��ֵ�����
	const int* SplineNum;	//��ֵ����
};
//}
//#endif
