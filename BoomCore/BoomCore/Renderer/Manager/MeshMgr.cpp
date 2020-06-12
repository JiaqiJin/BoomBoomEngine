#include "MeshMgr.h"

namespace Kawaii
{
	template<> MeshMgr::ptr Singleton<MeshMgr>::_instance = nullptr;

	MeshMgr::ptr MeshMgr::getSingleton()
	{
		if (_instance == nullptr)
			return _instance = std::make_shared<MeshMgr>();
		return _instance;
	}
}