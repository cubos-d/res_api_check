#include <Python.h>
#include <pytypedefs.h>
#include "../include/metrics.h"

int main(int argc, char* argv[])
{
    MemHM* dict = fetch_mem_data("/proc/meminfo", 128);
    //MemHM* dict = create_mem_hm(1);
    //insert_element(dict, "Hola", 321);
    //insert_element(dict, "PLAPLA", 123);
    print_mem_hm(dict);
    clean_mem_hm(dict);
    return 0;
}

PyObject* get_meminfo(PyObject *self)
{
    // Aquí puede pasar directo al paso 3.6
    int mp_capacity = 100;
    MemHM* dict = fetch_mem_data("/proc/meminfo", 128);
    //Vea examples.txt ahí está la cosa ya casi hecha para hacer esta función
    return NULL;
}