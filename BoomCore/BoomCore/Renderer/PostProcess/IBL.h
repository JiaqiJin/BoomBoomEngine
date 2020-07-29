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
	
		static void convoluteDiffuseIntegral(int width, int height, unsigned int cubemapTexIndex,
			unsigned int irradianceTexIndex);
	};
}