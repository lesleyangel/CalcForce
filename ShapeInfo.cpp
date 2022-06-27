#include "ShapeInfo.h"

MassPoint::MassPoint(const MassPointInitInfo& info)
{
	switch (info.type)
	{
	case MassType::Device:
		IsOnCabin = false;
		*this = MassPoint(info.mass, info.site, IsOnCabin, info.name);
		break;
	case MassType::Engine:
		IsOnCabin = true;
		*this = MassPoint(info.mass, info.site, IsOnCabin, info.name);
		break;
	default:
		break;
	}
}

MassPoint::MassPoint(double mass, Point pt, bool IsOnCabin, string name)
{
	this->mass = mass;
	this->pt = pt;
	this->xc = pt.getX();
	this->name = name;
	cabinID = -1;
	bulkheadID0 = bulkheadID1 = 0;
	this->IsOnCabin = IsOnCabin;
}

CabinInfo::CabinInfo(const CabinInitInfo& info)
{
	*this = CabinInfo(info.x1, info.MID, info.T, info.extramass, info.siteNum, info.bhlist);
	this->t_max = info.t_max;
	this->t_min = info.t_min;
}


CabinInfo::CabinInfo(double x1, int MID, double T, double extramass, int siteNum, vector<int> PBARL_list)
{
	this->x1 = x1;
	this->SiteNum = siteNum;
	this->bulkheadNum = PBARL_list.size();
	this->PBARL_list = PBARL_list;
	this->extramass = extramass;
	this->MID = MID;
	this->T = T;
	mass = x0 = xc = Area = 0;
	SetTnotMass = true;
}