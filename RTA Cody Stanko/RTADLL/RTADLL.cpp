// RTADLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RTADLL.h"

namespace RTA
{
	using namespace fbxsdk;

	int numTabs = 0;
	int functions::returnint()
	{
		return FBXSDK_INT_MIN;
	}

	void PrintTabs() {
		for (int i = 0; i < numTabs; i++)
			printf("\t");
	}
	
	FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) {
		switch (type) {
		case FbxNodeAttribute::eUnknown: return "unidentified";
		case FbxNodeAttribute::eNull: return "null";
		case FbxNodeAttribute::eMarker: return "marker";
		case FbxNodeAttribute::eSkeleton: return "skeleton";
		case FbxNodeAttribute::eMesh: return "mesh";
		case FbxNodeAttribute::eNurbs: return "nurbs";
		case FbxNodeAttribute::ePatch: return "patch";
		case FbxNodeAttribute::eCamera: return "camera";
		case FbxNodeAttribute::eCameraStereo: return "stereo";
		case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
		case FbxNodeAttribute::eLight: return "light";
		case FbxNodeAttribute::eOpticalReference: return "optical reference";
		case FbxNodeAttribute::eOpticalMarker: return "marker";
		case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
		case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
		case FbxNodeAttribute::eBoundary: return "boundary";
		case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
		case FbxNodeAttribute::eShape: return "shape";
		case FbxNodeAttribute::eLODGroup: return "lodgroup";
		case FbxNodeAttribute::eSubDiv: return "subdiv";
		default: return "unknown";
		}
	}

	void PrintAttribute(FbxNodeAttribute * _Attribute)
	{
		if (!_Attribute) return;

		FbxString typeName = GetAttributeTypeName(_Attribute->GetAttributeType());
		FbxString attrName = _Attribute->GetName();
		PrintTabs();
	}

	void PrintNode(FbxNode * _Node)
	{
		PrintTabs();
		const char * nodeName = _Node->GetName();
		FbxDouble3 translation = _Node->LclTranslation.Get();
		FbxDouble3 rotation = _Node->LclRotation.Get();
		FbxDouble3 scaling = _Node->LclScaling.Get();

		printf("<node name = '%s' translation = '(%f, %f, %f)' rotation = '(%f, %f, %f)' scaling = '(%f, %f, %f)'>\n",
			nodeName,
			translation[0], translation[1], translation[2],
			rotation[0], rotation[1], rotation[2],
			scaling[0], scaling[1], scaling[2]);
		numTabs++;

		for (int i = 0; i < _Node->GetNodeAttributeCount(); i++)
		{
			PrintAttribute(_Node->GetNodeAttributeByIndex(i));
		}
		
		for (int j = 0; j < _Node->GetChildCount(); j++)
		{
			PrintNode(_Node->GetChild(j));
		}

		numTabs--;
		PrintTabs();
		printf("</node>\n");
	}

	void functions::initFBX()
	{
		const char* fileName = "Box_Idle.fbx";

		FbxManager * sdkManager = FbxManager::Create();
			
		FbxImporter * importer = FbxImporter::Create(sdkManager,"");

		if (!importer->Initialize(fileName, -1, sdkManager->GetIOSettings()))
		{
			printf("Call to FbxImporter::Initialize() failed.\n");
			printf("Error returned: %s\n\n", importer->GetStatus().GetErrorString());
			exit(-1);
		}

		FbxScene * scene = FbxScene::Create(sdkManager, "theScene");

		importer->Import(scene);

		importer->Destroy();

		FbxNode * rootNode = scene->GetRootNode();

		if (rootNode)
		{
			for (int i = 0; i < rootNode->GetChildCount(); i++)
			{
				PrintNode(rootNode->GetChild(i));
			}
		}
		
		sdkManager->Destroy();

	}
}
