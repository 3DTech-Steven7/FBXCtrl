#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"
#include "utils.h"


// typedef struct {
//     PyObject_HEAD
//     /* Type-specific fields go here. */
//     PyObject *PropertyDict; /* first name */
// } FBXCtrlObject;


// class PyFBXControl
// {
// public:
//     static PyObject* py_get_scale(PyObject* self);
// };

// static void FBXCtrl_dealloc(FBXCtrlObject *self)
// {
//     Py_XDECREF(self->PropertyDict);
//     Py_TYPE(self)->tp_free((PyObject *) self);
// }
//
// static PyObject * FBXCtrl_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
// {
//     FBXCtrlObject *self;
//     self = (FBXCtrlObject *) type->tp_alloc(type, 0);
//     if (self != NULL) {
//         self->PropertyDict = PyUnicode_FromString("");
//         if (self->PropertyDict == NULL) {
//             Py_DECREF(self);
//             return NULL;
//         }
//     }
//     return (PyObject *) self;
// }
//
// static int FBXCtrl_init(FBXCtrlObject *self, PyObject *args, PyObject *kwds)
// {
//     static char *kwlist[] = {"PropertyDict", NULL};
//     PyObject *PropertyDict = NULL, *tmp;
//
//     if (!PyArg_ParseTupleAndKeywords(args, kwds, "|OOi", kwlist,
//                                      &PropertyDict))
//                                          return -1;
//
//     if (PropertyDict) {
//         tmp = self->PropertyDict;
//         Py_INCREF(PropertyDict);
//         self->PropertyDict = PropertyDict;
//         Py_XDECREF(tmp);
//     }
//     return 0;
// }
//
// static PyMemberDef FBXCtrl_members[] = {
//     {"PropertyDict", T_OBJECT_EX, offsetof(FBXCtrlObject, PropertyDict), 0,
//      "PropertyDict name"},
//     {NULL}  /* Sentinel */
// };
//
// static PyObject * FBXCtrl_name(FBXCtrlObject *self, PyObject *Py_UNUSED(ignored))
// {
//     if (self->PropertyDict == NULL) {
//         PyErr_SetString(PyExc_AttributeError, "PropertyDict");
//         return NULL;
//     }
//     return PyUnicode_FromFormat("%S", self->PropertyDict);
// }
//
// static PyMethodDef FBXCtrl_methods[] = {
//     {"name", (PyCFunction) FBXCtrl_name, METH_NOARGS,
//      "Return the name, combining the first and last name"
//     },
//     {NULL}  /* Sentinel */
// };
//
// static PyTypeObject spammodule = {
//     PyVarObject_HEAD_INIT(NULL, 0)
//     .tp_name = CustomType(FBXCtrl),
//     .tp_doc = "FBXCtrlImp objects",
//     .tp_basicsize = sizeof(FBXCtrlObject),
//     .tp_itemsize = 0,
//     .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
//     .tp_new = FBXCtrl_new,
//     .tp_init = (initproc) FBXCtrl_init,
//     .tp_dealloc = (destructor) FBXCtrl_dealloc,
//     .tp_members = FBXCtrl_members,
//     .tp_methods = FBXCtrl_methods,
// };
//
// static PyModuleDef FBXCtrlmodule = {
//     PyModuleDef_HEAD_INIT,
//     .m_name = STRING(FBXCtrl),
//     .m_doc = "Example module that creates an extension type.",
//     .m_size = -1,
// };
