#include "PyFBXControl.h"
#include "MFBXControl.h"
#include <memory>


PyObject* PyFBXControl::py_CopyData(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char* kwlist[] = {"ImpPath", "ExpPath", "bInit", "FileFormat", nullptr};
    // NOLINT(clang-diagnostic-writable-strings)
    static char* kwlist2[] = {"ip", "ep", "i", "ff", nullptr}; // NOLINT(clang-diagnostic-writable-strings)
    char* ImpPath;
    char* ExpPath;
    unsigned short cInit = 1;
    int iFileFormat = 0;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss|hi", kwlist, &ImpPath, &ExpPath, &cInit, &iFileFormat))
    {
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss|hi", kwlist2, &ImpPath, &ExpPath, &cInit, &iFileFormat))
        {
            return nullptr;
        }
    }

    if (cInit < 0 || cInit > 1)
    {
        return nullptr;
    }

    unique_ptr<MFBXRizomUVImp> ImpObject(new MFBXRizomUVImp(ImpPath));
    unique_ptr<MFBXRizomUVExp> ExopObject(new MFBXRizomUVExp(ExpPath));
    bool bResult;

    if ((bResult = ImpObject->InitFbxScene()))
    {
        const bool bInit = cInit ? true : false;
        if ((bResult = ExopObject->InitFbxScene(!bInit)))
        {
            ExopObject->CreateDocumentInfo(ImpObject.get());
            ExopObject->CreateGeometryInfo(ImpObject.get(), bInit);
            bResult = ExopObject->Save(iFileFormat);
        }
    }
    ImpObject.release(); // NOLINT(bugprone-unused-return-value)
    ExopObject.release(); // NOLINT(bugprone-unused-return-value)

    return Py_BuildValue("O", bResult ? Py_True : Py_False);
}
