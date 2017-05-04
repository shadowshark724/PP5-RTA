#pragma once

#ifdef RTADLL_EXPORTS  
#define RTADLL_API __declspec(dllexport)   
#else  
#define RTADLL_API __declspec(dllimport)   
#endif


namespace RTA
{
	class functions
	{
	public:
		static RTADLL_API int returnint();
		static RTADLL_API void initFBX();
	};
}