#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "utils.h"
#include "PyFBXControl.h"

static PyMethodDef SpamMethods[] = {
    {"CopyData",  (PyCFunction)PyFBXControl::py_CopyData, METH_VARARGS | METH_KEYWORDS, CopyData},
    {NULL, NULL, 0, NULL}
};

#if PYTHON_ABI_VERSION == 2
PyMODINIT_FUNC initFbxCtrl(void)
{
	(void)Py_InitModule(MODULE_NAME, SpamMethods, "My maya extension module.");
};

#elif PYTHON_ABI_VERSION == 3

static PyModuleDef spammodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = MODULENAME,
    .m_doc = "Example module that creates an extension type.",
    .m_size = -1,
    .m_methods = SpamMethods
};

PyMODINIT_FUNC PyInit_FbxCtrl_Py39(void)
{
    return PyModule_Create(&spammodule);
};
#endif
