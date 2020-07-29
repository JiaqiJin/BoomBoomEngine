#pragma once

namespace Kawaii
{
	class IBLAux
	{
	private:
		IBLAux() = default;
		~IBLAux() = default;
	public:
		static void convertToCubemap(int width, int height, unsigned int hdrTexIndex,
			unsigned int cuebmapTexIndex);
	
	};
}