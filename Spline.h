#pragma once
//namespace SplineSpace
//{
class /*__declspec(dllexport)*/SplineFailure		//异常类
{
private:
	const char* Message;
public:
	SplineFailure(const char* msg);
	const char* GetMessage();
};

class /*__declspec(dllexport)*/SplineInterface		//接口类
{
public:
	//单点插值:
	//x->输入自变量(输入)
	//y->返回插值结果(输出)
	//如果x不是内插值点则插值失败
	virtual bool SinglePointInterp(const double& x, double& y) = 0;
	//多点插值:
	//x->输入自变量数组首指针,num->自变量个数的引用(输入)
	//y->返回插值结果的首指针(输出数组)
	virtual bool MultiPointInterp(const double* x, const int& num, double* y) = 0;
	//自动插值:
	//num->需要生成的插值个数的引用(输入)
	//x->存放自变量数组的首指针(输出数组)
	//y->对应拟合结果的首指针(输出数组)
	virtual bool AutoInterp(const int& num, double* x, double* y) = 0;

	virtual double getMaxX() = 0;
	virtual double getMinX() = 0;

	virtual ~SplineInterface() {};
};

enum class BoundaryCondition		//边界条件枚举
{
	GivenFirstOrder = 1		//给定一阶导数
	, GivenSecondOrder		//给定二阶导数
};

class /*__declspec(dllexport)*/Spline :public SplineInterface				//实现类
{
public:
	//构造函数:
	//x0->已知的自变量数组首指针,y0->已知的因变量数组首指针,num->已知数据的个数
	//bc->边界条件,默认给定二阶导数
	//leftBoundary,rightBoundary左右边界导数值的引用,默认都是0
	Spline(const double* x0, const double* y0, const int& num,
		BoundaryCondition bc = BoundaryCondition::GivenSecondOrder, const double& leftBoundary = 0, const double& rightBoundary = 0);

	bool SinglePointInterp(const double& x, double& y)throw(SplineFailure);		//虚函数实现
	bool MultiPointInterp(const double* x, const int& num, double* y)throw(SplineFailure);
	bool AutoInterp(const int& num, double* x, double* y)throw(SplineFailure);
	double getMaxX() { return MaxX; }
	double getMinX() { return MinX; }
	~Spline();

private:
	//方法
	void PartialDerivative1(void);		//求一阶导
	void PartialDerivative2(void);		//求二阶导
										//属性
	const double* GivenX;	//已知数据的自变量
	const double* GivenY;	//已知数据的因变量
	const int GivenNum;		//给定的已知数据点数
	const BoundaryCondition Bc;	//边界类型
	const double LeftB;
	const double RightB;

	double* PartialDerivative;	//保存对应的二阶或者一阶偏导数
	double MaxX;				//自变量的最大值
	double MinX;				//自变量的最小值

	const double* SplineX;	//插值自变量
	double* SplineY;		//插值因变量
	const int* SplineNum;	//插值个数
};
//}
//#endif
