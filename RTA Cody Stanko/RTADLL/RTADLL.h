#pragma once

#ifdef RTADLL_EXPORTS  
#define RTADLL_API __declspec(dllexport)   
#else  
#define RTADLL_API __declspec(dllimport)   
#endif

#include <vector>

namespace RTA
{
	struct Vertex
	{
		float xyzw[4];
	};

	int * indicies = nullptr;
	Vertex * verts = nullptr;
	Vertex * stuff = nullptr;
	int indexSize = 0;
	int vertexSize = 0;
	
	std::vector<Vertex> bones;

	class functions
	{
	private:
		
	public:
	

		static RTADLL_API int returnint();
		//static RTADLL_API void deletestuff();
		static RTADLL_API void initFBX();
	
		RTADLL_API int* getIndex() { return indicies; }
		RTADLL_API Vertex* getVertex() { return stuff; }
		RTADLL_API int getIndexSize() { return indexSize; }
		RTADLL_API int getVertexSize() { return vertexSize; }
		RTADLL_API std::vector<Vertex> getBones() { return bones; }

		RTADLL_API void setIndex(int* _setthis) { indicies = _setthis; }
		RTADLL_API void setVertex(Vertex* _set) { verts = _set; }
		RTADLL_API void setIndexSize(int size) { indexSize = size; }
		RTADLL_API void setVertexSize(int size) { vertexSize = size; }
	};
}