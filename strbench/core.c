#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <locale.h>


static PyObject *BlacklistedError;


struct range {
    Py_UCS4 start;
    Py_UCS4 end;
};

struct sanitizer {
    Py_ssize_t blacklist_len; 
    Py_ssize_t blacklist_index; 
    struct range *blacklist;
};

static int
core_blacklist_check_char(struct sanitizer *instance, const Py_UCS4 c) {
    struct range *list = instance->blacklist;
    for (int i = 0; i < instance->blacklist_len; i++) {
        if ((c >= list[i].start) && (c <= list[i].end)) {
            return -1;
        }
    }
    return 0;
}

static PyObject *
core_blacklist_check(PyObject *self, PyObject *args) {
    PyObject *handle;
    PyObject *input;

    /* Parse arguments. */
    if (!PyArg_ParseTuple(args, "OU", &handle, &input)) {
        return NULL;
    }
   
    /* Unbox */
    struct sanitizer *instance = (struct sanitizer*)
        PyCapsule_GetPointer(handle, NULL);
    
    if (PyUnicode_READY(input)) {
        PyErr_SetString(PyExc_MemoryError, "Cannot allocate memory");
        return NULL;
    }

    //PyObject_Print(input, stdout, 0);
    
    Py_ssize_t len = PyUnicode_GET_LENGTH(input);
    int kind = PyUnicode_KIND(input);
    void *data = PyUnicode_DATA(input);

    for (int c = 0; c < len; c++) {
        Py_UCS4 ch = PyUnicode_READ(kind, data, c);
        if (core_blacklist_check_char(instance, ch)) {
            PyErr_SetString(BlacklistedError, "Blacklisted.");
            return NULL;
        }
    }

    /* Return nothing */
    Py_RETURN_NONE;
}


static PyObject *
core_blacklist_append(PyObject *self, PyObject *args) {
    PyObject *handle;
    uint32_t start, end;

    /* Parse arguments. */
    if (!PyArg_ParseTuple(args, "OII", &handle, &start, &end)) {
        return NULL;
    }
   
    /* Unbox */
    struct sanitizer *instance = (struct sanitizer*)
        PyCapsule_GetPointer(handle, NULL);
    
    instance->blacklist[instance->blacklist_index++] = 
        (struct range){start, end};

    /* Return nothing */
    Py_RETURN_NONE;
}


static PyObject *
core_create(PyObject *self, PyObject *args) {
    struct sanitizer *instance = malloc(sizeof(struct sanitizer));
    if (instance == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Cannot allocate memory");
        return NULL;
    }
    
    instance->blacklist_index = 0;
    /* Parse arguments. */
    if (!PyArg_ParseTuple(args, "n", &instance->blacklist_len)) {
        return NULL;
    }
    
    /* Allocate blacklist */
    instance->blacklist = calloc(
        instance->blacklist_len,
        sizeof(struct range)
    );
    if (instance->blacklist == NULL) {
        PyErr_SetString(PyExc_MemoryError, "Cannot allocate memory");
        return NULL;
    }
    
    /* Encapsulate the pinter for the future. */
    PyObject *handle = PyCapsule_New((void *)instance, NULL, NULL);

    /* Return */
    return Py_BuildValue("O", handle);
}


static PyObject *
core_dispose(PyObject *self, PyObject *args) {
    PyObject *handle;
    
    /* Parse arguments. */
    if (!PyArg_ParseTuple(args, "O", &handle)) {
        return NULL;
    }
   
    /* Unbox */
    struct sanitizer *instance = (struct sanitizer*)
        PyCapsule_GetPointer(handle, NULL);
    
    /* Deallocate */
    free(instance->blacklist);
    free(instance);
    
    /* Return nothing */
    Py_RETURN_NONE;
}


static PyMethodDef SanitizerMethods[] = {
    {
        "create",  
        core_create, 
        METH_VARARGS,
        "Creates a sanitizer instance."
    },
    {
        "dispose",  
        core_dispose, 
        METH_VARARGS,
        "Dispose a sanitizer instance."
    },
    {
        "blacklist_append",  
        core_blacklist_append, 
        METH_VARARGS,
        "Append a range into blacklist."
    },
    {
        "blacklist_check",  
        core_blacklist_check, 
        METH_VARARGS,
        "Check an input for blacklist."
    },
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


static char core_doc[] = 
    "Heplful utilitise for sanitize and check user input.";

static struct PyModuleDef coremodule= {
    PyModuleDef_HEAD_INIT,
    "strbench.core",   
    core_doc, 
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
   SanitizerMethods
};

PyMODINIT_FUNC
PyInit_core(void) {
    PyObject *m;

    m =  PyModule_Create(&coremodule);
    if (m == NULL) {
        return NULL;
    }

    BlacklistedError = 
        PyErr_NewException("strbench.core.BlacklistedError", NULL, NULL);
    Py_XINCREF(BlacklistedError);
    if (PyModule_AddObject(m, "BlacklistedError", BlacklistedError) < 0) {
        Py_XDECREF(BlacklistedError);
        Py_CLEAR(BlacklistedError);
        Py_DECREF(m);
        return NULL;
    }
    
    return m;
}
