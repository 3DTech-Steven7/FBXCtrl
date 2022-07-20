#include "PyFBXControl.h"
#include "MFBXControl.h"
#include <memory>


PyObject* PyFBXControl::py_CopyData(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char* kwlist[] = {"ImpPath", "ExpPath", "bInit", "bInvSave","FileFormat", nullptr};
    // NOLINT(clang-diagnostic-writable-strings)
    static char* kwlist2[] = {"ip", "ep", "i", "is", "ff", nullptr}; // NOLINT(clang-diagnostic-writable-strings)
    char* ImpPath;
    char* ExpPath;
    unsigned short cInit = 1;
    unsigned short cInvSave = 0;
    int iFileFormat = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss|hhi", kwlist, &ImpPath, &ExpPath, &cInit, &cInvSave, &iFileFormat))
    {
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss|hhi", kwlist2, &ImpPath, &ExpPath, &cInit, &cInvSave, &iFileFormat))
        {
            return nullptr;
        }
    }

    unique_ptr<MFBXRizomUVImp> ImpObject(new MFBXRizomUVImp(ImpPath));
    unique_ptr<MFBXRizomUVExp> ExopObject(new MFBXRizomUVExp(ExpPath));
    bool bResult;

    if ((bResult = ImpObject->InitFbxScene()))
    {
        const bool bInit = cInit ? true : false;
        if ((bResult = ExopObject->InitFbxScene(!bInit)))
        {
            const bool bInvSave = cInvSave ? true : false;
            ExopObject->CreateDocumentInfo(ImpObject.get());
            ExopObject->CreateGeometryInfo(ImpObject.get(), bInit);
            bResult = ExopObject->Save(iFileFormat);
            if (bInvSave)
            {
                ImpObject->Save(ExopObject.get(), iFileFormat);
            }
        }
    }
    ImpObject.release(); // NOLINT(bugprone-unused-return-value)
    ExopObject.release(); // NOLINT(bugprone-unused-return-value)

    return Py_BuildValue("O", bResult ? Py_True : Py_False);
}

PyObject* PyFBXControl::py_InitData(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char* kwlist[] = {"pPath", "FileFormat", nullptr};
    // NOLINT(clang-diagnostic-writable-strings)
    static char* kwlist2[] = {"p", "ff", nullptr}; // NOLINT(clang-diagnostic-writable-strings)
    char* Path;
    int iFileFormat = 0;
    
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|i", kwlist, &Path, &iFileFormat))
    {
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|i", kwlist2, &Path, &iFileFormat))
        {
            return nullptr;
        }
    }

    unique_ptr<MFBXRizomUVExp> ExopObject(new MFBXRizomUVExp(Path));
    bool bResult;

    if ((bResult = ExopObject->InitFbxScene(false)))
    {
        bResult = ExopObject->Save(iFileFormat);
    }
    ExopObject.release(); // NOLINT(bugprone-unused-return-value)

    return Py_BuildValue("O", bResult ? Py_True : Py_False);
}
