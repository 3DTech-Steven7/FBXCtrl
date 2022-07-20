#pragma once

#include <fbxsdk.h>
#include <map>


using namespace std; // NOLINT(clang-diagnostic-header-hygiene)

using PropertyDict = map<FbxString, FbxProperty>;
using LayerDict = map<FbxUInt8, FbxLayer*>;


class MFBXRizomUVExp;
struct SGeometryInfo
{
    FbxGeometry* Geometry = nullptr;
    LayerDict LayerInfo;
    PropertyDict PropertyInfo;
};

using Geometrydict = map<FbxUInt16, SGeometryInfo>;


class MFBXControl
{
public:
    MFBXControl(FbxString InFBXPath);
    MFBXControl(char* InFBXPath);
    ~MFBXControl();

    bool InitFbxScene(bool bLoad = true);
    bool Save(int fileFormat) const;

public:
    FbxManager* lSdkManager;
    FbxScene* lScene;
    FbxString pFBXPath;

    inline static const char* TopNodeName = "RizomUV";
};


class MFBXRizomUVImp : public MFBXControl
{
public:
    // ~MFBXRizomUVImp();

    FbxString GetLongName(FbxProperty PropertyNode, const FbxString* CurrentName) const;

    PropertyDict GetAllChildProperty(FbxProperty PropertyNode) const;

    [[nodiscard("Ignoring the return value will result in memory leaks.")]]
    PropertyDict GetDocumentProperties() const;

    Geometrydict GetGeometryProperties() const;
    bool Save(MFBXRizomUVExp* ExpNode, int fileFormat) const;
};

class MFBXRizomUVExp : public MFBXControl
{
public:
    void FromDictCopyProperty(FbxObject* InNode, PropertyDict& PropertyInfo) const;

    void CreateDocumentInfo(const MFBXRizomUVImp* ImpNode) const;

    void CreateGeometryInfo(const MFBXRizomUVImp* ImpNode, bool bInit) const;

    static void SetLayerUserData(FbxGeometry* & geometry_exp, LayerDict& InLayerDict);
    
    void CreateInitMesh(const char* pName) const;
};
