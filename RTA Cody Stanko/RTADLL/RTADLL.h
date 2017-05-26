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
		float rotation[4];
		float scale[4];
		float matrixT[16];
		float color[4];
		float norm[4];
		std::vector<float> weights;
	};

	struct Keyframe
	{
		double time;
		std::vector<Vertex> joints;
	};

	struct anim_clip
	{
		double duration;
		std::vector<Keyframe> frames;
	};
	//std::vector<Vertex> keyframes;
	std::vector<int> indicies;// = nullptr;
	std::vector<Vertex> verts;// = nullptr;
	std::vector<Vertex> stuff;// = nullptr;
	
	//int indexSize = 0;
	//int vertexSize = 0;
	
	std::vector<Vertex> bones;
	std::vector<Keyframe> keysHere;
	anim_clip animation_clip;

	class functions
	{
	private:
		
	public:
		static RTADLL_API int returnint();
		//static RTADLL_API void deletestuff();
		static RTADLL_API void initFBX();
	
		
		RTADLL_API std::vector<int> getIndex() { return indicies; }
		RTADLL_API std::vector<Vertex> getVertex() { return stuff; }
		//RTADLL_API int getIndexSize() { return indexSize; }
		//RTADLL_API int getVertexSize() { return vertexSize; }
		RTADLL_API std::vector<Vertex> getBones() { return bones; }
		RTADLL_API anim_clip getAnim() { return animation_clip; }

		//RTADLL_API void setIndex(int* _setthis) { indicies = _setthis; }
		//RTADLL_API void setVertex(Vertex* _set) { verts = _set; }
		//RTADLL_API void setIndexSize(int size) { indexSize = size; }
		//RTADLL_API void setVertexSize(int size) { vertexSize = size; }
	};
}