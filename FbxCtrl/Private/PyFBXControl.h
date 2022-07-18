#pragma once
#include <Python.h>


class PyFBXControl
{
public:
    static PyObject* py_CopyData(PyObject* self, PyObject* args, PyObject* kwargs);
};