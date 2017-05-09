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
			printf("-");
	}

	//void functions::deletestuff()
	//{
	//	if (indicies != nullptr)
	//	{
	//		delete[] indicies;
	//		indicies = nullptr;
	//	}
	//	if (verts != nullptr)
	//	{
	//		delete[] verts;
	//		verts = nullptr;
	//	}
	//}

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

	void loadData(FbxNode * _Node)
	{
		FbxMesh * meshData = _Node->GetMesh();

		// verts from here
		//meshData->GetControlPoints()
		int polys = meshData->GetPolygonCount();
		indicies = new int[polys * 3];

		//printf("Poly Count = %i", polys);
		//printf("Current index:");
		int indCount = 0;
		for (int j = 0; j < polys; j++)
		{
			//index here
			//indicies.push_back(meshData->)

			for (int k = 0; k < 3; k++)
			{
				indicies[(j * 3) + k] = meshData->GetPolygonVertex(j, k);
			//	printf("%d ,", indicies[j + k]);
				indCount++;
			}
		}
		indexSize = indCount;

		printf(" count of index = %d", indCount);

		int count = meshData->GetControlPointsCount();
		printf("Control points = %i", count);

		verts = new Vertex[count];
		for (int i = 0; i < count; i++)
		{
			fbxsdk::FbxVector4 holdthings;
			holdthings = meshData->GetControlPointAt(i);
			verts[i].xyzw[0] = holdthings.mData[0];
			verts[i].xyzw[1] = holdthings.mData[1];
			verts[i].xyzw[2] = holdthings.mData[2];
			verts[i].xyzw[3] = holdthings.mData[3];
		//	printf("\npos - (%f, %f, %f, %f)", verts[i].xyzw[0], verts[i].xyzw[1], verts[i].xyzw[2], verts[i].xyzw[3]);
			//verts[i]
		}

		stuff = new Vertex[polys * 3];

		for (int j = 0; j < polys * 3; j++)
		{
			stuff[j] = verts[indicies[j]];
		}
		// norms uvs here
		vertexSize = indCount;

	}

	FbxDouble fixthis(FbxDouble _fix)
	{
		if (_fix < -FBXSDK_TOLERANCE || _fix > FBXSDK_TOLERANCE)
		{
			FbxDouble temp = 0;
			return temp;
		}
		else
			return _fix;
	}

	void LimbLoad(FbxNode * _Node, Vertex curr)
	{
		Vertex parent;
		for (int i = 0; i < _Node->GetChildCount(); i++)
		{
			FbxDouble3 trans = _Node->GetChild(i)->LclTranslation.Get();
			//trans [0] = fixthis(trans[0]);
			parent.xyzw[0] = trans[0] + curr.xyzw[0];
			parent.xyzw[1] = trans[1] + curr.xyzw[1];
			parent.xyzw[2] = trans[2] + curr.xyzw[2];
			parent.xyzw[3] = 1.0f;
			bones.push_back(curr);
			bones.push_back(parent);
			printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f", _Node->GetName(), curr.xyzw[0], curr.xyzw[1], curr.xyzw[2]);
			printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f",_Node->GetChild(i)->GetName() , parent.xyzw[0], parent.xyzw[1], parent.xyzw[2]);
			LimbLoad(_Node->GetChild(i), parent);			
		}
	}

	void PrintNode(FbxNode * _Node)
	{
		PrintTabs();
		const char * nodeName = _Node->GetName();
		const char * nodeAtt = _Node->GetNodeAttribute()->GetTypeName();
		FbxString testAtt = GetAttributeTypeName((*_Node).GetNodeAttribute()->GetAttributeType());
		FbxDouble3 translation = _Node->LclTranslation.Get();
		FbxDouble3 rotation = _Node->LclRotation.Get();
		FbxDouble3 scaling = _Node->LclScaling.Get();
		if (testAtt == "mesh")
		{
			printf("<node name = '%s'  type = '%s' translation = '(%0.2f, %0.2f, %0.2f)' rotation = '(%0.2f, %0.2f, %0.2f)' scaling = '(%0.2f, %0.2f, %0.2f)'>\n",
				nodeName,
				nodeAtt,
				translation[0], translation[1], translation[2],
				rotation[0], rotation[1], rotation[2],
				scaling[0], scaling[1], scaling[2]);
			loadData(_Node);
		}
		if (testAtt == "skeleton")
		{
			printf("<node name = '%s'  type = '%s' translation = '(%0.2f, %0.2f, %0.2f)' rotation = '(%0.2f, %0.2f, %0.2f)' scaling = '(%0.2f, %0.2f, %0.2f)'>\n",
				nodeName,
				nodeAtt,

				translation[0], translation[1], translation[2],
				rotation[0], rotation[1], rotation[2],
				scaling[0], scaling[1], scaling[2]);
			/*if (groot == nullptr)
			{
				groot = _Node;
			}*/
			if (bones.size() == 0)
			{
				Vertex hold;
				FbxDouble3 trans = _Node->LclTranslation.Get();
				//trans[0] = fixthis(trans[0]);
				hold.xyzw[0] = trans[0];
				hold.xyzw[1] = trans[1];
				hold.xyzw[2] = trans[2];
				hold.xyzw[3] = 1.0f;
				//bones.push_back(hold);
				for (int j = 0; j < _Node->GetChildCount(); j++)
				{
					printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f", _Node->GetName(),hold.xyzw[0], hold.xyzw[1], hold.xyzw[2]);
					bones.push_back(hold);
					Vertex temp;
					nodeName = _Node->GetChild(j)->GetName();
					trans = _Node->GetChild(j)->LclTranslation.Get();
					//trans[0] = fixthis(trans[0]);
					temp.xyzw[0] = trans[0] + hold.xyzw[0];
					temp.xyzw[1] = trans[1] + hold.xyzw[1];
					temp.xyzw[2] = trans[2] + hold.xyzw[2];
					temp.xyzw[3] = 1.0f;
					printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f", nodeName,temp.xyzw[0], temp.xyzw[1], temp.xyzw[2]);
					bones.push_back(temp);
					LimbLoad(_Node->GetChild(j), temp);
					//bones.push_back(hold);
				}
			}
		}
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
	//	printf("</node>\n");
	}

	void functions::initFBX()
	{
		const char* fileName = "Teddy_Idle.fbx";

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
		//for (int i = 0; i < bones.size(); i++)
		//printf("Debug here1 - %0.4f, %0.4f, %0.4f\n", bones[i].xyzw[0], bones[i].xyzw[1], bones[i].xyzw[2]);
		//printf("Debug here1 size = %d", bones.size());
		sdkManager->Destroy();

	}
}
