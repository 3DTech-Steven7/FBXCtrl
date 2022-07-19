#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "utils.h"
#include "PyFBXControl.h"

static PyMethodDef SpamMethods[] = {
    // ReSharper disable once CppCStyleCast
    {"CopyData", (PyCFunction)PyFBXControl::py_CopyData, METH_VARARGS | METH_KEYWORDS, CopyData},
    // NOLINT(clang-diagnostic-cast-function-type)
    {nullptr, nullptr, 0, nullptr}
};

#ifndef PYTHON_ABI_VERSION // NOLINT(clang-diagnostic-undef)
PyMODINIT_FUNC PYINIT_FUNCTION(void)
{
    (void)Py_InitModule(MODULENAME, SpamMethods, "My maya extension module.");
}


#else

static PyModuleDef spammodule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = MODULENAME,
    .m_doc = "Example module that creates an extension type.",
    .m_size = -1,
    .m_methods = SpamMethods
};

PyMODINIT_FUNC PYINIT_FUNCTION(void)
{
    return PyModule_Create(&spammodule);
};
#endif
