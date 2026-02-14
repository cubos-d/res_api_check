#include <Python.h>
#include <exports.h>
#include <longobject.h>
#include <methodobject.h>
#include <modsupport.h>
#include <moduleobject.h>
#include <pyerrors.h>
#include <pytypedefs.h>
#include <refcount.h>
#include <stdio.h>
#include <tupleobject.h>
#include <unicodeobject.h>
#include "../include/metrics.h"

PyObject* get_cpuinfo(PyObject* self, PyObject *Py_UNUSED(ignored))
{
    //TODO take notes in the notebook. Here you learned a lot. 
    CpuData* cpu = fetch_cpu_data();
    if (!cpu) {
        PyErr_SetString(PyExc_RuntimeError, "Could not read /proc/stat");
        return NULL;
    }
    PyObject *dict_final = PyDict_New();
    PyObject* total_vec = PyTuple_New(10);
    PyObject* cpus_matrix = PyTuple_New(cpu->n_cores);
    for (int i = 0; i < cpu->n_cores; i++)
    {
        PyObject* actual_tuple = PyTuple_New(10);
        for (int j = 0; j < 10; j++)
        {
            PyObject* item_to_add = PyLong_FromLong(cpu->mat[i][j]);
            if (PyTuple_SetItem(actual_tuple, j, item_to_add) == -1) {
                char err_buffer[128];
                sprintf(err_buffer, "Couldn't allocate %d element in %dth cpu\n", j, i);
                PyErr_SetString(PyExc_IndexError, err_buffer);
            }
        }
        if (PyTuple_SetItem(cpus_matrix, i, actual_tuple) == -1){
            char err_buffer[128];
            sprintf(err_buffer, "Couldn't allocate array of elements in %dth cpu\n", i);
            PyErr_SetString(PyExc_IndexError, err_buffer);
        }
    } 

    for (int j = 0; j < 10; j++)
    {
        PyObject* item_to_add = PyLong_FromLong(cpu->total[j]);
        if (PyTuple_SetItem(total_vec, j, item_to_add) == -1) {
            char err_buffer[128];
            sprintf(err_buffer, "Couldn't allocate %dth of element in total usage array\n", j);
            PyErr_SetString(PyExc_IndexError, err_buffer);
        }
    }
    PyObject *total_key = PyUnicode_FromString("total_details");
    PyObject *mat_key = PyUnicode_FromString("cpu_details");
    if (PyDict_SetItem(dict_final, total_key, total_vec) == -1) {
        PyErr_SetString(PyExc_IndexError, "Couldn't allocate total item in final dict");
    }
    if (PyDict_SetItem(dict_final, mat_key, cpus_matrix) == -1) {
        PyErr_SetString(PyExc_IndexError, "Couldn't allocate matrix item in final dict");
    }
    Py_DECREF(total_key);
    Py_DECREF(mat_key);
    Py_DECREF(total_vec); 
    Py_DECREF(cpus_matrix);
    clean_cpu_data(cpu);
    return dict_final;
}

PyObject* get_meminfo(PyObject *self, PyObject *Py_UNUSED(ignored))
{
    int mp_capacity = 100;
    double p_available;
    MemHM* dict = fetch_mem_data("/proc/meminfo", mp_capacity);
    if (!dict) {
        PyErr_SetString(PyExc_RuntimeError, "Could not read /proc/meminfo");
        return NULL;
    }
    unsigned long mem_total = search_element(dict, "MemTotal");
    unsigned long mem_avail = search_element(dict, "MemAvailable");
    p_available = mem_avail/(mem_total + 0.0);
    PyObject *dict_final = PyDict_New();
    PyObject *first_key = PyUnicode_FromString("AvailableFraction");
    PyObject *first_value = PyFloat_FromDouble(p_available);
    if (PyDict_SetItem(dict_final, first_key, first_value) == -1) {
        char err_buffer[128];
        sprintf(err_buffer, "Couldn't Allocate key AvailableFraction with value %.2f in dict\n", p_available);
        PyErr_SetString(PyExc_KeyError, err_buffer);
    } 
    Py_DECREF(first_key);
    Py_DECREF(first_value);
    for (MemRow* act = dict->dic_head; act; act = act->dic_next)
    {
        PyObject *act_key = PyUnicode_FromString(act->data);
        PyObject *act_value = PyLong_FromLong(act->value);
        if (PyDict_SetItem(dict_final, act_key, act_value) == -1) {
            char err_buffer[128];
            sprintf(err_buffer, "Couldn't Allocate key %s with value %ld in dict\n",act->data, act->value);
            PyErr_SetString(PyExc_KeyError, err_buffer);
        } 
        Py_DECREF(act_key);
        Py_DECREF(act_value);
    }
    clean_mem_hm(dict);
    return dict_final;
}

static PyMethodDef methods[] = {
    {"get_memory_info", get_meminfo, METH_NOARGS, "This function returns relevant memory data in a dictionary."},
    {"get_cpu_info", get_cpuinfo, METH_NOARGS, "This functions returns relevant CPU data in a dictionary."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef sys_monitor = {
    PyModuleDef_HEAD_INIT,
    "monitor",
    "Python C module that returns the usage data of memory and CPU\n",
    -1,
    methods
};

PyMODINIT_FUNC PyInit_monitor()
{
    PyObject* module = PyModule_Create(&sys_monitor);
    return module;
}

/*
int main(int argc, char* argv[])
{
    unsigned long search_result;
    MemHM* dict = fetch_mem_data("/proc/meminfo", 128);
    //MemHM* dict = create_mem_hm(1);
    //insert_element(dict, "Hola", 321);
    delete_element(dict, "Balloon");
    insert_element(dict, "plapla", 512);
    search_result = search_element(dict, "MemAvailable");
    printf("Search result: %ld\n", search_result);
    print_mem_hm(dict);
    clean_mem_hm(dict);
    return 0;
}
*/