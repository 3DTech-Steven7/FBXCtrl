#include "utils.h"
#include <Python.h>
#include "PyFBXControl.h"



static PyMethodDef SpamMethods[] = {

    //{"getScale", static_cast<PyCFunction>(MFBXControl::py_get_scale), METH_VARARGS | METH_KEYWORDS, get_scale},
    {nullptr, nullptr, 0, nullptr}

};



#if PYTHON_ABI_VERSION == 2
PyMODINIT_FUNC initFbxCtrl(void)
{
	(void)Py_InitModule(MODULE_NAME, SpamMethods, "My maya extension module.");
};

#elif PYTHON_ABI_VERSION == 3
static struct PyModuleDef spammodule = {
    PyModuleDef_HEAD_INIT,
    STRING(MODULE_NAME), /* name of module */
    nullptr, /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module,
				 or -1 if the module keeps state in global variables. */
    SpamMethods
};

PyMODINIT_FUNC PYINIT_FUNCTION(void)
{
    // PyObject *m;
    // if (PyType_Ready(&spammodule) < 0)
    //     return NULL;
    //
    // m = PyModule_Create(&FBXCtrlmodule);
    // if (m == NULL)
    //     return NULL;
    //
    // Py_INCREF(&spammodule);
    // if (PyModule_AddObject(m, STRING(FBXCtrlObject), (PyObject *) &spammodule) < 0) {
    //     Py_DECREF(&spammodule);
    //     Py_DECREF(m);
    //     return NULL;
    // }
    //
    // return m;
    return PyModule_Create(&spammodule);
};
#endif
