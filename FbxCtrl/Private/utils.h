#pragma once

#define STR1(a) #a
#define STRING(R) STR1(R)
#define PYIt(a) PyInit_##a

#ifdef Py39
#define MODULE_NAME FbxCtrl_Py39
#elif Maya_Py39
#define MODULE_NAME FbxCtrl_Maya_Py39
#endif

#define CustomType(a) STRING(MODULE_NAME.##a)
#define PYINIT_FUNCTION PYIt(MODULE_NAME)


static const char get_scale[] = "This function's not parameter";
