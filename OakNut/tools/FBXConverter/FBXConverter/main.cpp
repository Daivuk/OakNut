#include <fbxsdk.h>
#include <iostream>
#include <NvTriStrip.h>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <cinttypes>

#define VERSION 2

struct sVertex
{
    float x, y, z;
    float nx, ny, nz;
    float tx, ty, tz;
    float bx, by, bz;
    float u, v;
    float r, g, b;

    bool operator==(const sVertex& in_other) const
    {
        return (
            x == in_other.x && y == in_other.y && z == in_other.z &&
            nx == in_other.nx && ny == in_other.ny && nz == in_other.nz &&
            u == in_other.u && v == in_other.v &&
            r == in_other.r && g == in_other.g && b == in_other.b);
    }
};

struct sVertexNoColor
{
    float x, y, z;
    float nx, ny, nz;
    float tx, ty, tz;
    float bx, by, bz;
    float u, v;

    bool operator==(const sVertex& in_other) const
    {
        return (
            x == in_other.x && y == in_other.y && z == in_other.z &&
            nx == in_other.nx && ny == in_other.ny && nz == in_other.nz &&
            u == in_other.u && v == in_other.v);
    }
};

struct sVertexNoBump
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
    float r, g, b;

    bool operator==(const sVertex& in_other) const
    {
        return (
            x == in_other.x && y == in_other.y && z == in_other.z &&
            nx == in_other.nx && ny == in_other.ny && nz == in_other.nz &&
            u == in_other.u && v == in_other.v &&
            r == in_other.r && g == in_other.g && b == in_other.b);
    }
};

struct sVertexNoColorNoBump
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;

    bool operator==(const sVertex& in_other) const
    {
        return (
            x == in_other.x && y == in_other.y && z == in_other.z &&
            nx == in_other.nx && ny == in_other.ny && nz == in_other.nz &&
            u == in_other.u && v == in_other.v);
    }
};

/* Tab character ("\t") counter */
int numTabs = 0; 

/**
 * Print the required number of tabs.
 */
void PrintTabs() 
{
    for(int i = 0; i < numTabs; i++)
        printf("\t");
}

/**
 * Return a string-based representation based on the attribute type.
 */
FbxString GetAttributeTypeName(FbxNodeAttribute::EType type) 
{ 
    switch(type) { 
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

/**
 * Print an attribute.
 */
void PrintAttribute(FbxNodeAttribute* pAttribute) 
{
    if(!pAttribute) return;
 
    FbxString typeName = GetAttributeTypeName(pAttribute->GetAttributeType());
    FbxString attrName = pAttribute->GetName();
    PrintTabs();
    // Note: to retrieve the character array of a FbxString, use its Buffer() method.
    printf("<attribute type='%s' name='%s'/>\n", typeName.Buffer(), attrName.Buffer());
}


/**
 * Print a node, its attributes, and all its children recursively.
 */
void PrintNode(FbxNode* pNode) 
{
    PrintTabs();
    const char* nodeName = pNode->GetName();
    FbxDouble3 translation = pNode->LclTranslation.Get(); 
    FbxDouble3 rotation = pNode->LclRotation.Get(); 
    FbxDouble3 scaling = pNode->LclScaling.Get();

    // Print the contents of the node.
    printf("<node name='%s' translation='(%f, %f, %f)' rotation='(%f, %f, %f)' scaling='(%f, %f, %f)'>\n", 
        nodeName, 
        translation[0], translation[1], translation[2],
        rotation[0], rotation[1], rotation[2],
        scaling[0], scaling[1], scaling[2]
        );
    numTabs++;

    // Print the node's attributes.
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	{
        PrintAttribute(pNode->GetNodeAttributeByIndex(i));
	}

    // Recursively print the children.
    for (int j = 0; j < pNode->GetChildCount(); j++)
	{
        PrintNode(pNode->GetChild(j));
	}

    numTabs--;
    PrintTabs();
    printf("</node>\n");
}

FbxMesh* findMesh(FbxNode* pNode)
{
    for (int i = 0; i < pNode->GetNodeAttributeCount(); i++)
	{
		FbxNodeAttribute* pAttribute = pNode->GetNodeAttributeByIndex(i);
		FbxNodeAttribute::EType type = pAttribute->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh)
		{
			// Convert to triangle
			FbxGeometryConverter lConverter(pNode->GetFbxManager());
			return (FbxMesh*)lConverter.Triangulate(pAttribute, false);
		}
	}
    // Check children
    for (int i = 0; i < pNode->GetChildCount(); ++i)
    {
        auto pMesh = findMesh(pNode->GetChild(i));
        if (pMesh) return pMesh;
    }
	return NULL;
}

void computeTangentBasis(const float *P0, const float *P1, const float *P2,
                         const float *UV0, const float *UV1, const float *UV2,
                         const float *normal, float *tangent, float *binormal)
{
    //using Eric Lengyel's approach with a few modifications
    //from Mathematics for 3D Game Programmming and Computer Graphics
    // want to be able to trasform a vector in Object Space to Tangent Space
    // such that the x-axis cooresponds to the 's' direction and the
    // y-axis corresponds to the 't' direction, and the z-axis corresponds
    // to <0,0,1>, straight up out of the texture map

    //let P = v1 - v0
    float P[3] = {P1[0] - P0[0], P1[1] - P0[1], P1[2] - P0[2]};
    //let Q = v2 - v0
    float Q[3] = {P2[0] - P0[0], P2[1] - P0[1], P2[2] - P0[2]};
    float s1 = UV1[0] - UV0[0];
    float t1 = UV1[1] - UV0[1];
    float s2 = UV2[0] - UV0[0];
    float t2 = UV2[1] - UV0[1];


    //we need to solve the equation
    // P = s1*T + t1*B
    // Q = s2*T + t2*B
    // for T and B


    //this is a linear system with six unknowns and six equatinos, for TxTyTz BxByBz
    //[px,py,pz] = [s1,t1] * [Tx,Ty,Tz]
    // qx,qy,qz     s2,t2     Bx,By,Bz

    //multiplying both sides by the inverse of the s,t matrix gives
    //[Tx,Ty,Tz] = 1/(s1t2-s2t1) *  [t2,-t1] * [px,py,pz]
    // Bx,By,Bz                      -s2,s1	    qx,qy,qz  

    //solve this for the unormalized T and B to get from tangent to object space

    float tmp = 0.0f;
    if (fabsf(s1*t2 - s2*t1) <= 0.0001f)
    {
        tmp = 1.0f;
    }
    else
    {
        tmp = 1.0f / (s1*t2 - s2*t1);
    }

    tangent[0] = (t2*P[0] - t1*Q[0]);
    tangent[1] = (t2*P[1] - t1*Q[1]);
    tangent[2] = (t2*P[2] - t1*Q[2]);

    tangent[0] = tangent[0] * tmp;
    tangent[1] = tangent[1] * tmp;
    tangent[2] = tangent[2] * tmp;

    binormal[0] = (s1*Q[0] - s2*P[0]);
    binormal[1] = (s1*Q[1] - s2*P[1]);
    binormal[2] = (s1*Q[2] - s2*P[2]);

    binormal[0] = binormal[0] * tmp;
    binormal[1] = binormal[1] * tmp;
    binormal[2] = binormal[2] * tmp;
}

#define STRIDE 11
#define POSITION_OFFSET 0
#define NORMAL_OFFSET 3
#define TEXCOORD_OFFSET 6
#define COLOR_OFFSET 8

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cout << "Missing parameters";
#ifdef _DEBUG
	system("pause");
#endif
		return 1;
	}

	std::string src = argv[1];
	std::string dst = argv[2];

	FILE* pFic = 0;
	if (fopen_s(&pFic, dst.c_str(), "wb"))
	{
		std::cout << "Error opening dst for writting: " << dst;
#ifdef _DEBUG
	system("pause");
#endif
		return 1;
	}

	bool useYUp = false;
    bool bLH = false;
    bool bColor = false;
    bool bBump = false;
	int argi = 3;
	float scale = 1;
    while (argi < argc)
    {
        std::string p = argv[argi++];
        std::string v = argv[argi++];

        if (p == "-up")
        {
            if (v == "y" || v == "Y")
            {
                useYUp = true;
            }
            else if (v == "-y" || v == "-Y")
            {
                useYUp = true;
                bLH = true;
            }
        }
        else if (p == "-scale")
        {
            std::stringstream ss(v);
            if (!(ss >> scale))
            {
                printf("Error parsing -scale.\n");
                return 1;
            }
        }
        else if (p == "-color")
        {
            bColor = v == "on";
        }
        else if (p == "-bump")
        {
            bBump = v == "on";
        }
    }

    unsigned long version = VERSION;
	fwrite(&version, 4, 1, pFic);

#define FLAG_HAS_COLOR 0x1
#define FLAG_HAS_BINORMALS 0x2

    uint32_t flags = 0;
    if (bColor) flags |= FLAG_HAS_COLOR;
    if (bBump) flags |= FLAG_HAS_BINORMALS;
    fwrite(&flags, 4, 1, pFic);

	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();

	// Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);
	
    // Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(src.c_str(), -1, lSdkManager->GetIOSettings())) 
	{ 
        printf("Call to FbxImporter::Initialize() failed.\n"); 
        printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString()); 
#ifdef _DEBUG
	system("pause");
#endif
		return 1;
	}

	// Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file is imported, so get rid of the importer.
    lImporter->Destroy();

	// Print the nodes of the scene and their attributes recursively.
    // Note that we are not printing the root node because it should
    // not contain any attributes.
    FbxNode* lRootNode = lScene->GetRootNode();
	FbxMesh* pMesh = 0;
    if (lRootNode) 
	{
        for(int i = 0; i < lRootNode->GetChildCount(); i++)
		{
       //     PrintNode(lRootNode->GetChild(i));
			pMesh = findMesh(lRootNode->GetChild(i));
			if (pMesh) break;
		}
    }
	else
	{
        printf("No Root node?\n"); 
#ifdef _DEBUG
	system("pause");
#endif
		return 1;
	}

	if (!pMesh)
	{
        printf("No Mesh found\n"); 
#ifdef _DEBUG
	system("pause");
#endif
		return 1;
	}

	std::vector<sVertex> unpacked_vertices;
	std::vector<sVertex> packed_vertices;
	std::vector<unsigned short> indices;

	// Get indices + normal/texcoords
	int current = 0;
	FbxStringList UVSetNameList;
	pMesh->GetUVSetNames(UVSetNameList);
	FbxVector2	fbxTexCoord;
	FbxVector4	fbxNormal;
	FbxVector4	fbxPosition;
	FbxColor	fbxColor;
	bool unmapped;
	int index;
	int colorIndex;
	FbxGeometryElementVertexColor* pVertexColorElement = pMesh->GetElementVertexColor(0);
	if (pVertexColorElement)
	{
		if (pVertexColorElement->GetMappingMode() != FbxLayerElement::eByPolygonVertex)
		{
			pVertexColorElement->RemapIndexTo(FbxLayerElement::eByPolygonVertex);
		}
	}
	for (int Poly = 0; Poly < pMesh->GetPolygonCount(); Poly++)
	{
		// Get number of verts in this poly
		int NumVertices = pMesh->GetPolygonSize(Poly);

		// Loop for each vert in poly
		for (int Vertex = 0; Vertex < NumVertices; ++Vertex)
		{
			index = pMesh->GetPolygonVertex(Poly, Vertex);
			fbxPosition = pMesh->GetControlPointAt(index);
			pMesh->GetPolygonVertexUV(Poly, Vertex, UVSetNameList.GetStringAt(0), fbxTexCoord, unmapped);
			pMesh->GetPolygonVertexNormal(Poly, Vertex, fbxNormal);
			if (pVertexColorElement)
			{
				colorIndex = pVertexColorElement->mIndexArray->GetAt(current);
				fbxColor = pVertexColorElement->mDirectArray->GetAt(colorIndex);
			}
			else
			{
				fbxColor.Set(1, 1, 1);
			}

			sVertex unpacked_vertex;

			// Store Position
			if (useYUp)
			{
				unpacked_vertex.x = (float)fbxPosition[0];
				unpacked_vertex.y = (float)fbxPosition[2];
                unpacked_vertex.z = bLH ? (float)fbxPosition[1] : -(float)fbxPosition[1];
			}
			else
			{
				unpacked_vertex.x = (float)fbxPosition[0];
				unpacked_vertex.y = (float)fbxPosition[1];
				unpacked_vertex.z = (float)fbxPosition[2];
			}

			// Store Normal
			fbxNormal.Normalize();
			if (useYUp)
			{
				unpacked_vertex.nx = (float)fbxNormal[0];
				unpacked_vertex.ny = (float)fbxNormal[2];
                unpacked_vertex.nz = bLH ? (float)fbxNormal[1] : -(float)fbxNormal[1];
            }
			else
			{
				unpacked_vertex.nx = (float)fbxNormal[0];
				unpacked_vertex.ny = (float)fbxNormal[1];
				unpacked_vertex.nz = (float)fbxNormal[2];
			}

			// Store TexCoord
			unpacked_vertex.u = (float)fbxTexCoord[0];
			unpacked_vertex.v = 1 - (float)fbxTexCoord[1];

			// Store Color
			unpacked_vertex.r = (float)fbxColor.mRed;
			unpacked_vertex.g = (float)fbxColor.mGreen;
			unpacked_vertex.b = (float)fbxColor.mBlue;
			
			indices.push_back((unsigned short)unpacked_vertices.size());
			unpacked_vertices.push_back(unpacked_vertex);

			++current;
		}
	}

    // Destroy the SDK manager and all the other objects it was handling.
    lSdkManager->Destroy();

	// Now optimize by sharing vertices
	unsigned leni = unpacked_vertices.size();
	unsigned lenj;
	unsigned int i, j;
	for (i = 0; i < leni; ++i)
	{
		const sVertex& unpacked_vertex = unpacked_vertices[i];

		lenj = packed_vertices.size();
		for (j = 0; j < lenj; ++j)
		{
			const sVertex& packed_vertex = packed_vertices[j];
			if (packed_vertex == unpacked_vertex)
			{
				indices[i] = (unsigned short)j;
				break;
			}
		}

		if (j != lenj) continue;
		indices[i] = (unsigned short)packed_vertices.size();
		packed_vertices.push_back(unpacked_vertex);
	}
	unpacked_vertices.clear();

	// Finally scale the resulting model
	leni = packed_vertices.size();
	for (i = 0; i < leni; ++i)
	{
		sVertex& packed_vertex = packed_vertices[i];
		packed_vertex.x *= scale;
		packed_vertex.y *= scale;
		packed_vertex.z *= scale;
	}

	// Now optimize vertex ordering to optimize the video card cache
	//SetCacheSize(CACHESIZE_GEFORCE3);
	//SetListsOnly(true);
	//PrimitiveGroup primitiveGroup;
	//primitiveGroup.type = PT_LIST;
	//primitiveGroup.numIndices = (unsigned int)indices.size();
	//primitiveGroup.indices = new unsigned short[indices.size()];
	//memcpy(primitiveGroup.indices, &indices[0], sizeof(unsigned short) * indices.size());
	//PrimitiveGroup* remappedPrimitiveGroup;
	//RemapIndices(&primitiveGroup, 1, (unsigned short)packed_vertices.size(), &remappedPrimitiveGroup);
	
	// Remap

    // Reorder indices
    if (bLH)
    {
        //for (decltype(indices.size()) ind = 0; ind < indices.size(); ind += 3)
        //{
        //    std::swap(indices[ind + 0], indices[ind + 1]);
        //}
    }

    // Compute tagent basis
    if (bBump)
    {
        for (decltype(indices.size()) ind = 0; ind < indices.size(); ind += 3)
        {
            computeTangentBasis(
                &packed_vertices[indices[ind + 0]].x,
                &packed_vertices[indices[ind + 1]].x,
                &packed_vertices[indices[ind + 2]].x,
                &packed_vertices[indices[ind + 0]].u,
                &packed_vertices[indices[ind + 1]].u,
                &packed_vertices[indices[ind + 2]].u,
                &packed_vertices[indices[ind + 0]].nx,
                &packed_vertices[indices[ind + 0]].tx,
                &packed_vertices[indices[ind + 0]].bx);
            computeTangentBasis(
                &packed_vertices[indices[ind + 1]].x,
                &packed_vertices[indices[ind + 2]].x,
                &packed_vertices[indices[ind + 0]].x,
                &packed_vertices[indices[ind + 1]].u,
                &packed_vertices[indices[ind + 2]].u,
                &packed_vertices[indices[ind + 0]].u,
                &packed_vertices[indices[ind + 1]].nx,
                &packed_vertices[indices[ind + 1]].tx,
                &packed_vertices[indices[ind + 1]].bx);
            computeTangentBasis(
                &packed_vertices[indices[ind + 2]].x,
                &packed_vertices[indices[ind + 0]].x,
                &packed_vertices[indices[ind + 1]].x,
                &packed_vertices[indices[ind + 2]].u,
                &packed_vertices[indices[ind + 0]].u,
                &packed_vertices[indices[ind + 1]].u,
                &packed_vertices[indices[ind + 2]].nx,
                &packed_vertices[indices[ind + 2]].tx,
                &packed_vertices[indices[ind + 2]].bx);
        }
    }

    if (bLH)
    {
        for (auto& vert : packed_vertices)
        {
            vert.x = -vert.x;
            vert.nx = -vert.nx;
            vert.tx = -vert.tx;

            //vert.bx = -vert.bx;
            vert.by = -vert.by;
            vert.bz = -vert.bz;
        }
    }

    unsigned long nbVertices = (unsigned long)packed_vertices.size();
    unsigned long nbIndices = (unsigned long)indices.size();
    fwrite(&nbVertices, 4, 1, pFic);
    fwrite(&nbIndices, 4, 1, pFic);
    if (bColor)
    {
        if (bBump)
        {
            fwrite(&packed_vertices[0], 4, nbVertices * (sizeof(sVertex) / sizeof(float)), pFic);
        }
        else
        {
            std::vector<sVertexNoBump> packed_verticesFinal;
            for (auto& vert : packed_vertices)
            {
                sVertexNoBump newVert{
                    vert.x, vert.y, vert.z,
                    vert.nx, vert.ny, vert.nz,
                    vert.u, vert.v,
                    vert.r, vert.g, vert.b
                };
                packed_verticesFinal.push_back(newVert);
            }
            fwrite(&packed_verticesFinal[0], 4, nbVertices * (sizeof(sVertexNoBump) / sizeof(float)), pFic);
        }
    }
    else
    {
        if (bBump)
        {
            std::vector<sVertexNoColor> packed_verticesFinal;
            for (auto& vert : packed_vertices)
            {
                sVertexNoColor newVert{
                    vert.x, vert.y, vert.z,
                    vert.nx, vert.ny, vert.nz,
                    vert.tx, vert.ty, vert.tz,
                    vert.bx, vert.by, vert.bz,
                    vert.u, vert.v
                };
                packed_verticesFinal.push_back(newVert);
            }
            fwrite(&packed_verticesFinal[0], 4, nbVertices * (sizeof(sVertexNoColor) / sizeof(float)), pFic);
        }
        else
        {
            std::vector<sVertexNoColorNoBump> packed_verticesFinal;
            for (auto& vert : packed_vertices)
            {
                sVertexNoColorNoBump newVert{
                    vert.x, vert.y, vert.z,
                    vert.nx, vert.ny, vert.nz,
                    vert.u, vert.v
                };
                packed_verticesFinal.push_back(newVert);
            }
            fwrite(&packed_verticesFinal[0], 4, nbVertices * (sizeof(sVertexNoColorNoBump) / sizeof(float)), pFic);
        }
    }
    fwrite(&indices[0], 2, nbIndices, pFic);
	fclose(pFic);
	printf("Vert count: %i\n", (int)nbVertices); 

#ifdef _DEBUG
	system("pause");
#endif
	return 0;
}
