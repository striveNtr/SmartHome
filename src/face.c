#if 0
1、包含Python.h头文件，以便使用Python API。
2、使用void Py_Initialize()初始化Python解释器，
3、使用PyObject *PyImport_ImportModule(const char *name)和PyObject *PyObject_GetAttrString(PyObject *o, const 
char *attr_name)获取sys.path对象，并利用int PyList_Append(PyObject *list, PyObject *item)将当前路径.添加到sys.path
中，以便加载当前的Python模块(Python文件即python模块)。
4、使用PyObject *PyImport_ImportModule(const char *name)函数导入Python模块，并检查是否有错误。
5、使用PyObject *PyObject_GetAttrString(PyObject *o, const char *attr_name)函数获取Python函数对象，并检查是否可调用。
6、使用PyObject *Py_BuildValue(const char *format, ...)函数将C类型的数据结构转换成Python对象，作为Python函数的参数,没有
参数不需要调用
7、使用PyObject *PyObject_CallObject(PyObject *callable, PyObject *args)函数调用Python函数，并获取返回值。
8、使用int PyArg_Parse(PyObject *args, const char *format, ...)函数将返回值转换为C类型，并检查是否有错误,没有返回值时不
需要调用。
9、使用void Py_DECREF(PyObject *o)函数释放所有引用的Python对象。
10、结束时调用void Py_Finalize()函数关闭Python解释器。
相关的函数参数说明参考网站（网站左上角输入函数名即可开始搜索）：
https://docs.python.org/zh-cn/3/c-api/import.html
#endif

#include <Python.h>
#include "face.h"
void face_init(void)
{
    Py_Initialize();
    PyObject *sys = PyImport_ImportModule("sys");
    PyObject *path = PyObject_GetAttrString(sys, "path");
    PyList_Append(path, PyUnicode_FromString("."));
}

void face_fianl(void)
{
    Py_Finalize();
}
double face_category(void)
{
    double result = 0.0;
    system(WGET_CMD);
    if (access(FACE_FILE, F_OK) == 0)
    {
        return result;
    }
    PyObject *pmodule = PyImport_ImportModule("face"); // face.py
    if (!pmodule)
    {
        PyErr_Print();
        printf("Error:failed to load face.py\n");
        goto FAILED_MODULE;
    }
    PyObject *pfun = PyObject_GetAttrString(pmodule, "alibaba_face"); // alibaba_face函数
    if (!pfun)
    {
        PyErr_Print();
        printf("Error:failed to load face.py\n");
        goto FAILED_FUN;
    }
    PyObject *pvalue = PyObject_CallObject(pfun, NULL); // 调用alibaba_face函数
    if (!pvalue)
    {
        PyErr_Print();
        printf("Error:failed to load face.py\n");
        goto FAILED_VALUE;
    }

    if (!PyArg_Parse(pvalue, "d", &result)) // 解析获取调用alibaba_face函数返回值 转换成c语言格式
    {
        PyErr_Print();
        printf("Error:failed to load face.py\n");
        goto FAILED_VALUE;
    }
    printf("result:%0.2lf\n", result);

FAILED_VALUE:
    Py_DECREF(pvalue);
FAILED_FUN:
    Py_DECREF(pfun);
FAILED_MODULE:
    Py_DECREF(pmodule);
    return result;
}
