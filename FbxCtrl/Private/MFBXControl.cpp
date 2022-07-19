#include "MFBXControl.h"
#include "Common.h"
#include <format>
#include <string>


MFBXControl::MFBXControl(FbxString InFBXPath)
{
    pFBXPath = InFBXPath;
    lSdkManager = nullptr;
    lScene = nullptr;
}

MFBXControl::MFBXControl(char* InFBXPath)
{
    pFBXPath = InFBXPath;
    lSdkManager = nullptr;
    lScene = nullptr;
}

MFBXControl::~MFBXControl()
{
    lSdkManager->Destroy();
    lScene->Destroy();
    delete pFBXPath;

    lSdkManager = nullptr;
    lScene = nullptr;
    pFBXPath = nullptr;
}

bool MFBXControl::InitFbxScene(bool bLoad)
{
    bool bResult = true;
    lSdkManager = nullptr;
    lScene = nullptr;
    InitializeSdkObjects(lSdkManager, lScene);
    if (bLoad)
    {
        bResult = LoadScene(lSdkManager, lScene, pFBXPath.Buffer());
    }
    return bResult;
}

FbxString MFBXRizomUVImp::GetLongName(const FbxProperty PropertyNode, const FbxString* CurrentName) const
{
    FbxString ChildName = *CurrentName;
    if (*CurrentName != TopNodeName)
    {
        FbxProperty ChildPropertyNode = PropertyNode;
        FbxString ParentName;
        for (FbxUInt8 i = 0; i < 255; i++)
        {
            ParentName.Clear();
            ChildPropertyNode = ChildPropertyNode.GetParent();
            ParentName = ChildPropertyNode.GetName();
            string name = format("{0}|{1}", ParentName, ChildName);
            ChildName = FbxString(name.c_str());
            if (ParentName == TopNodeName)
            {
                break;
            }
        }
    }
    return ChildName;
}

PropertyDict MFBXRizomUVImp::GetAllChildProperty(FbxProperty PropertyNode) const
{
    PropertyDict ChildDict;
    if (PropertyNode.GetName() == TopNodeName)
    {
        ChildDict[TopNodeName] = PropertyNode;
    }

    FbxString PropertyName;
    FbxProperty PropertyChild;

    for (FbxUInt16 i = 0; i < 9999; i++)
    {
        PropertyName.Clear();
        if (i == 0)
        {
            PropertyChild = PropertyNode.GetFirstDescendent();
        }
        else
        {
            PropertyChild = PropertyNode.GetNextDescendent(PropertyChild);
        }

        PropertyName = PropertyChild.GetName();

        if (PropertyName.IsEmpty())
        {
            break;
        }

        PropertyName = GetLongName(PropertyChild, &PropertyName);
        ChildDict[PropertyName] = PropertyChild;
    }
    return ChildDict;
}

PropertyDict MFBXRizomUVImp::GetDocumentProperties() const
{
    const FbxDocument* documentNode = lScene->GetRootDocument();
    FbxProperty&& documentroot = documentNode->FindProperty(TopNodeName);
    PropertyDict child_dict = GetAllChildProperty(documentroot);
    return child_dict;
}

Geometrydict MFBXRizomUVImp::GetGeometryProperties() const
{
    Geometrydict geometry_dict;
    for (int i = 0; i < lScene->GetGeometryCount(); i++)
    {
        FbxGeometry* geometry = lScene->GetGeometry(i);
        FbxProperty geometry_property = geometry->FindProperty(TopNodeName);
        PropertyDict child_dict = GetAllChildProperty(geometry_property);

        LayerDict layer_dict;
        for (int n = 0; n < geometry->GetLayerCount(); n++)
        {
            FbxLayer* layer = geometry->GetLayer(n);
            layer_dict[n] = layer; // NOLINT(clang-diagnostic-implicit-int-conversion)
        }

        geometry_dict[i] = SGeometryInfo(geometry, layer_dict, child_dict);
    }
    return geometry_dict;
}

void MFBXRizomUVExp::FromDictCopyProperty(FbxObject* InNode, PropertyDict& PropertyInfo) const
{
    PropertyDict::iterator it;
    for (it = PropertyInfo.begin(); it != PropertyInfo.end(); ++it)
    {
        const FbxString Name = it->first;
        const FbxProperty ExpProperty = it->second;

        FbxDataType DataType = ExpProperty.GetPropertyDataType();
        FbxProperty ImpProperty = FbxProperty::Create(InNode, DataType, Name);
        ImpProperty.CopyValue(ExpProperty);
    }
}

void MFBXRizomUVExp::CreateDocumentInfo(const MFBXRizomUVImp* ImpNode) const
{
    FbxDocument* documentNode = lScene->GetRootDocument();
    PropertyDict PropertyInfo = ImpNode->GetDocumentProperties();
    FromDictCopyProperty(documentNode, PropertyInfo);
}

void MFBXRizomUVExp::CreateGeometryInfo(const MFBXRizomUVImp* ImpNode, const bool bInit) const
{
    Geometrydict geometry_info_dict = ImpNode->GetGeometryProperties();
    int geometry_count;
    if (bInit)
    {
        geometry_count = geometry_info_dict.size();
        // NOLINT(bugprone-narrowing-conversions, clang-diagnostic-shorten-64-to-32, cppcoreguidelines-narrowing-conversions)
    }
    else
    {
        geometry_count = lScene->GetGeometryCount();
    }

    for (int i = 0; i < geometry_count; i++) // NOLINT(clang-diagnostic-sign-compare)
    {
        SGeometryInfo info_dict = geometry_info_dict[i];
        // NOLINT(clang-diagnostic-implicit-int-conversion, clang-diagnostic-sign-compare)

        if (bInit)
        {
            CreateInitMesh(info_dict.Geometry->GetName());
        }

        FbxGeometry* geometry_exp = lScene->GetGeometry(i);

        FromDictCopyProperty(geometry_exp, info_dict.PropertyInfo);
        SetLayerUserData(geometry_exp, info_dict.LayerInfo);
    }
}

void MFBXRizomUVExp::SetLayerUserData(FbxGeometry* & geometry_exp, LayerDict& InLayerDict)
{
    // ReSharper disable once CppJoinDeclarationAndAssignment
    LayerDict::iterator it;
    for (it = InLayerDict.begin(); it != InLayerDict.end(); ++it)
    {
        if (FbxLayerElementUserData* user_data = it->second->GetUserData())
        {
            FbxLayer* layer = geometry_exp->GetLayer(it->first);
            if (layer == nullptr)
            {
                geometry_exp->CreateLayer();
                layer = geometry_exp->GetLayer(it->first);
            }
            layer->SetUserData(user_data);
        }
    }
}

bool MFBXRizomUVExp::Save(const int fileFormat) const
{
    return SaveScene(lSdkManager, lScene, pFBXPath, fileFormat);
}

void MFBXRizomUVExp::CreateInitMesh(const char* pName) const
{
    FbxMesh* lMesh = FbxMesh::Create(lScene, pName);

    // Create control points.
    lMesh->InitControlPoints(1);

    // create a FbxNode
    FbxNode* lNode = FbxNode::Create(lScene, pName);

    // set the node attribute
    lNode->SetNodeAttribute(lMesh);

    // return the FbxNode
    lScene->GetRootNode()->AddChild(lNode);
}
