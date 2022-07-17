#pragma once

#include <fbxsdk.h>
#include <map>


using namespace std;

using PropertyDict = map<FbxString, FbxProperty*>;
using LayerDict = map<FbxUInt8, FbxLayer*>;

struct SGeometryInfo
{
    FbxGeometry Geometry;
    LayerDict LayerInfo;
    PropertyDict PropertyInfo;
};

using Geometrydict = map<FbxUInt16, SGeometryInfo*>;


class MFBXControl
{
public:
    MFBXControl(FbxString InFBXPath);
    ~MFBXControl();

    bool InitFbxScene();

public:
    FbxManager* lSdkManager;
    FbxScene* lScene;
    FbxString pFBXPath;

    inline static const char* TopNodeName = "RizomUV";
};


class MFBXRizomUVImp : public MFBXControl
{
    using Super = MFBXControl;
public:
    // MFBXRizomUVImp(FbxString InFBXPath);
    // ~MFBXRizomUVImp();

    FbxString GetLongName(const FbxProperty* PropertyNode, const FbxString* CurrentName) const;

    PropertyDict GetAllChildProperty(FbxProperty* PropertyNode) const;

    [[nodiscard("Ignoring the return value will result in memory leaks.")]]
    PropertyDict GetDocumentProperties() const;

    // Geometrydict GetGeometryProperties();
};

class MFBXRizomUVExp : public MFBXControl
{
    using Super = MFBXControl;
public:
    void FromDictCopyProperty(FbxObject* InNode, PropertyDict& PropertyInfo) const;

    void CreateDocumentInfo(MFBXRizomUVImp ImpNode) const;

    //void CreateGeometryInfo(FbxNode* InNode);

    void Save() const;
};

