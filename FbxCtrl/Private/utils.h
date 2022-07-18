﻿#pragma once

#define STR1(a) #a
#define STRING(R) STR1(R)

#ifdef Py39
#define MODULE_NAME FbxCtrl_Py39
#define PYINIT_FUNCTION PyInit_FbxCtrl_Py39
#elif Maya_Py39
#define MODULE_NAME FbxCtrl_Maya_Py39
#define PYINIT_FUNCTION PyInit_FbxCtrl_Maya_Py39
#endif

#define CustomType(a) STRING(MODULE_NAME.##a)
#define MODULENAME STRING(MODULE_NAME)

static const char CopyData[] = "This function's not parameter";
