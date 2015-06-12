#include <stdio.h>
#include <stdlib.h>
#include <python2.7/Python.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2){
        return -1;
    }

    char * string;  
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;  

    Py_Initialize();       
    if ( !Py_IsInitialized() )           
        return -1; 

    /*chdir("/mnt/hgfs/D/code/linux-virtual/myproj/python/");*/
    /*PyRun_SimpleString("execfile('pytest.py')");*/
    /*return 0;*/
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    pModule = PyImport_ImportModule("semail");
    /*pName = PyString_FromString("pytest");  */
    /*pModule = PyImport_Import(pName);  */
    if ( !pModule )   
    {  
        printf("can't find test1.py\n");  
        getchar();  
        return -1;  
    }  
    pDict = PyModule_GetDict(pModule);  
    if ( !pDict )           
    {  
        return -1;  
    }  

    pFunc = PyDict_GetItemString(pDict, "main");  
    if ( !pFunc || !PyCallable_Check(pFunc) )           
    {  
        printf("can't find function [add]\n");  
        getchar();  
        return -1;  
    }  

    pArgs = PyTuple_New(1);

    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", argv[1]));   
    /*PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",4));*/

    pRetVal = PyObject_CallObject(pFunc, pArgs);  
    printf("function return value : %d\r\n", PyInt_AsLong(pRetVal));  

    /*Py_DECREF(pName);  */
    Py_DECREF(pArgs);  
    Py_DECREF(pModule);  
    Py_DECREF(pRetVal);

    Py_Finalize();

    return 0;
}
