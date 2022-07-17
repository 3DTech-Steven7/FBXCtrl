#include "MFBXControl.h"
#include "Common.h"
#include <format>
#include <string>


MFBXControl::MFBXControl(FbxString InFBXPath)
{
    pFBXPath = InFBXPath;
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

bool MFBXControl::InitFbxScene()
{
    lSdkManager = nullptr;
    lScene = nullptr;
    InitializeSdkObjects(lSdkManager, lScene);
    bool bResult = LoadScene(lSdkManager, lScene, pFBXPath.Buffer());
    return bResult;
}

// MFBXRizomUVImp::MFBXRizomUVImp(FbxString InFBXPath):Super(InFBXPath)
// {
//     InitFbxScene();
// }
//
// MFBXRizomUVImp::~MFBXRizomUVImp()
// {
//     Super::~Super();
// }

FbxString MFBXRizomUVImp::GetLongName(const FbxProperty* PropertyNode, const FbxString* CurrentName) const
{
    FbxString ChildName = *CurrentName;
    if (*CurrentName != TopNodeName)
    {
        FbxProperty ChildPropertyNode = *PropertyNode;

        for (FbxUInt8 i = 0; i < 255; i++)
        {
            ChildPropertyNode = ChildPropertyNode.GetParent();
            const FbxString ParentName = ChildPropertyNode.GetName();
            string name = format("{0}|{1}", ParentName, ChildName);
            ChildName = FbxString(name.c_str());

            if (ChildName == TopNodeName)
            {
                break;
            }
        }
    }
    return ChildName;
}

PropertyDict MFBXRizomUVImp::GetAllChildProperty(FbxProperty* PropertyNode) const
{
    PropertyDict ChildDict;
    if (PropertyNode->GetName() == TopNodeName)
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
            PropertyChild = PropertyNode->GetFirstDescendent();
        }
        else
        {
            PropertyChild = PropertyNode->GetNextDescendent(PropertyChild);
        }

        PropertyName = PropertyChild.GetName();

        if (PropertyName.IsEmpty())
        {
            break;
        }

        PropertyName = GetLongName(&PropertyChild, &PropertyName);
        ChildDict[PropertyName] = &PropertyChild;
    }
    return ChildDict;
}

PropertyDict MFBXRizomUVImp::GetDocumentProperties() const
{
    const FbxDocument* documentNode = lScene->GetRootDocument();
    FbxProperty&& documentroot = documentNode->FindProperty(TopNodeName);
    PropertyDict child_dict = GetAllChildProperty(&documentroot);
    return child_dict;
}

void MFBXRizomUVExp::FromDictCopyProperty(FbxObject* InNode, PropertyDict& PropertyInfo) const
{
    PropertyDict::iterator it;
    for (it = PropertyInfo.begin(); it != PropertyInfo.end(); ++it)
    {
        const FbxString Name = it->first;
        const FbxProperty* ExpProperty = it->second;

        FbxDataType DataType = ExpProperty->GetPropertyDataType();
        FbxProperty ImpProperty = FbxProperty::Create(InNode, DataType, Name);
        ImpProperty.CopyValue(*ExpProperty);
    }
}

void MFBXRizomUVExp::CreateDocumentInfo(MFBXRizomUVImp ImpNode) const
{
    FbxDocument* documentNode = lScene->GetRootDocument();
    PropertyDict PropertyInfo = ImpNode.GetDocumentProperties();
    FromDictCopyProperty(documentNode, PropertyInfo);
}

void MFBXRizomUVExp::Save() const
{
    SaveScene(lSdkManager, lScene, pFBXPath);
}