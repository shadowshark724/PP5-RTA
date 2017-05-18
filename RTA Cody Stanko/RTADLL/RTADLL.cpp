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
		

		//printf("Poly Count = %i", polys);
		//printf("Current index:");
		int indCount = 0;
		for (int j = 0; j < polys; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				indicies.push_back(meshData->GetPolygonVertex(j, k));
			//	printf("%d ,", indicies[j + k]);
				indCount++;
			}
		}
		

		printf(" count of index = %d", indCount);

		int count = meshData->GetControlPointsCount();
		printf("Control points = %i", count);

		//verts = new Vertex[count];
		for (int i = 0; i < count; i++)
		{
			fbxsdk::FbxVector4 holdthings;
			Vertex temp;
			holdthings = meshData->GetControlPointAt(i);

			temp.xyzw[0] = holdthings.mData[0];
			temp.xyzw[1] = holdthings.mData[1];
			temp.xyzw[2] = holdthings.mData[2];
			temp.xyzw[3] = holdthings.mData[3];
			verts.push_back(temp);
		//	printf("\npos - (%f, %f, %f, %f)", verts[i].xyzw[0], verts[i].xyzw[1], verts[i].xyzw[2], verts[i].xyzw[3]);
			//verts[i]
		}

		//stuff = new Vertex[polys * 3];

		for (int j = 0; j < polys * 3; j++)
		{
			stuff.push_back(verts[indicies[j]]);
		}
		// norms uvs here
		//vertexSize = indCount;

	}

	void LimbLoad(FbxNode * _Node, Vertex curr, Keyframe* key, FbxTime timeDur)
	{
		Vertex parent;
		for (int i = 0; i < _Node->GetChildCount(); i++)
		{
			FbxDouble3 trans = _Node->GetChild(i)->LclTranslation.Get();
			FbxDouble4 glob = _Node->GetChild(i)->EvaluateGlobalTransform(timeDur).GetT();
			FbxDouble4 rot = _Node->GetChild(i)->EvaluateGlobalTransform(timeDur).GetR();
			FbxDouble4 scale = _Node->GetChild(i)->EvaluateGlobalTransform(timeDur).GetS();
			//parent.xyzw[0] = trans[0] + curr.xyzw[0];
			//parent.xyzw[1] = trans[1] + curr.xyzw[1];
			//parent.xyzw[2] = trans[2] + curr.xyzw[2];
			//parent.xyzw[3] = 1.0f;
			parent.xyzw[0] = glob[0];
			parent.xyzw[1] = glob[1];
			parent.xyzw[2] = glob[2];
			parent.xyzw[3] = glob[3];

			parent.rotation[0] = rot[0];
			parent.rotation[1] = rot[1];
			parent.rotation[2] = rot[2];
			parent.rotation[3] = rot[3];

			parent.scale[0] = scale[0];
			parent.scale[1] = scale[1];
			parent.scale[2] = scale[2];
			parent.scale[3] = scale[3];

			key->joints.push_back(curr);
			key->joints.push_back(parent);
			//printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f", _Node->GetName(), curr.xyzw[0], curr.xyzw[1], curr.xyzw[2]);
			//printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f",_Node->GetChild(i)->GetName() , parent.xyzw[0], parent.xyzw[1], parent.xyzw[2]);
			LimbLoad(_Node->GetChild(i), parent, key,timeDur);			
		}
	}

	void PrintNode(FbxNode * _Node)
	{
		//int something = _Node->GetScene;
		//printf("poses = %d", something);

		PrintTabs();
		const char * nodeName = _Node->GetName();
		const char * nodeAtt = _Node->GetNodeAttribute()->GetTypeName();
		FbxString testAtt = GetAttributeTypeName((*_Node).GetNodeAttribute()->GetAttributeType());
		FbxDouble3 translation = _Node->LclTranslation.Get();
		FbxDouble3 rotation = _Node->LclRotation.Get();
		FbxDouble3 scaling = _Node->LclScaling.Get();

		FbxDouble4 global1 = _Node->EvaluateGlobalTransform().GetT();
		FbxDouble4 global2 = _Node->EvaluateGlobalTransform().GetR();
		FbxDouble4 global3 = _Node->EvaluateGlobalTransform().GetQ();
		FbxDouble4 global4 = _Node->EvaluateGlobalTransform()[3];

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

#pragma region loadani

			
			if (animation_clip.frames.size() == 0)
			{
				FbxAnimStack * stack = _Node->GetScene()->GetCurrentAnimationStack();
				FbxTimeSpan timeSpan = stack->GetLocalTimeSpan();
				FbxTime timeDur = timeSpan.GetDuration();
				animation_clip.duration = timeDur.GetSecondDouble();
				FbxLongLong frameCount = timeDur.GetFrameCount(FbxTime::EMode::eFrames24);
				for (FbxLongLong i = 1; i < frameCount + 1; i += 1)
				{
					Keyframe tempframe;
					timeDur.SetFrame(i, FbxTime::EMode::eFrames24);
					//FbxTime frametime = transCurve->KeyGetCount();
					tempframe.time = timeDur.GetSecondDouble();

					Vertex hold;
					FbxDouble3 trans = _Node->LclTranslation.Get();
					FbxDouble4 glob = _Node->EvaluateGlobalTransform(timeDur).GetT();
					FbxDouble4 rot = _Node->EvaluateGlobalTransform(timeDur).GetR();
					FbxDouble4 scale = _Node->EvaluateGlobalTransform(timeDur).GetS();

					/*hold.xyzw[0] = trans[0];
					hold.xyzw[1] = trans[1];
					hold.xyzw[2] = trans[2];
					hold.xyzw[3] = 1.0f;*/
					hold.xyzw[0] = glob[0];
					hold.xyzw[1] = glob[1];
					hold.xyzw[2] = glob[2];
					hold.xyzw[3] = 1.0f;//glob[3];

					hold.rotation[0] = rot[0];
					hold.rotation[1] = rot[1];
					hold.rotation[2] = rot[2];
					hold.rotation[3] = rot[3];

					hold.scale[0] = scale[0];
					hold.scale[1] = scale[1];
					hold.scale[2] = scale[2];
					hold.scale[3] = scale[3];
					//bones.push_back(hold);
					for (int j = 0; j < _Node->GetChildCount(); j++)
					{
						printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f", _Node->GetName(), hold.xyzw[0], hold.xyzw[1], hold.xyzw[2]);
						tempframe.joints.push_back(hold);
						Vertex temp;
						nodeName = _Node->GetChild(j)->GetName();
						trans = _Node->GetChild(j)->LclTranslation.Get();
						glob = _Node->GetChild(j)->EvaluateGlobalTransform(timeDur).GetT();
						rot = _Node->GetChild(j)->EvaluateGlobalTransform(timeDur).GetR();
						scale = _Node->GetChild(j)->EvaluateGlobalTransform(timeDur).GetS();

						/*temp.xyzw[0] = trans[0] + hold.xyzw[0];
						temp.xyzw[1] = trans[1] + hold.xyzw[1];
						temp.xyzw[2] = trans[2] + hold.xyzw[2];
						temp.xyzw[3] = 1.0f;*/

						temp.xyzw[0] = glob[0];
						temp.xyzw[1] = glob[1];
						temp.xyzw[2] = glob[2];
						temp.xyzw[3] = 1.0f;// glob[3];

						temp.rotation[0] = rot[0];
						temp.rotation[1] = rot[1];
						temp.rotation[2] = rot[2];
						temp.rotation[3] = rot[3];

						temp.scale[0] = scale[0];
						temp.scale[1] = scale[1];
						temp.scale[2] = scale[2];
						temp.scale[3] = scale[3];

						//printf("\nDebug here2 %s = %0.4f, %0.4f, %0.4f", nodeName,temp.xyzw[0], temp.xyzw[1], temp.xyzw[2]);
						tempframe.joints.push_back(temp);
						LimbLoad(_Node->GetChild(j), temp, &tempframe, timeDur);
						//bones.push_back(hold);
					}



					animation_clip.frames.push_back(tempframe);
				}
			}
#pragma endregion

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
		const char* fileName = "Teddy_Run.fbx";

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
