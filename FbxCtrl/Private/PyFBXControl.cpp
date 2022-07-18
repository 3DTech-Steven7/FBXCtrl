#include "PyFBXControl.h"
#include "MFBXControl.h"
#include <memory>


PyObject* PyFBXControl::py_CopyData(PyObject* self, PyObject* args, PyObject* kwargs)
{
    static char* kwlist[] = { "ImpPath", "ExpPath", NULL };
    static char* kwlist2[] = { "ip", "ep", NULL };
    char* ImpPath;
    char* ExpPath;
    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist, &ImpPath, &ExpPath))
    {
        if (!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist2, &ImpPath, &ExpPath))
        {
            return NULL;
        }
    }
    //printf("ImpPath: %s\n", ImpPath);
    //printf("ExpPath: %s\n", ExpPath);
    
    unique_ptr<MFBXRizomUVImp> ImpObject(new MFBXRizomUVImp(ImpPath));
    unique_ptr<MFBXRizomUVExp> ExopObject(new MFBXRizomUVExp(ExpPath));
    bool bResult;

    
    if ((bResult = ImpObject->InitFbxScene()))
    {
        if ((bResult = ExopObject->InitFbxScene()))
        {
            ExopObject->CreateDocumentInfo(ImpObject.get());
            bResult = ExopObject->Save();
        } 
    }
    ImpObject.release();  // NOLINT(bugprone-unused-return-value)
    ExopObject.release();  // NOLINT(bugprone-unused-return-value)

    return Py_BuildValue("O", bResult ? Py_True : Py_False);
}