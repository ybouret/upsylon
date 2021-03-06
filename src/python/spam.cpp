#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyObject *SpamError = 0;


static inline PyObject *
spam_system(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    (void)self;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    if (sts != 0) {
        PyErr_SetString(SpamError, "System command failed");
        return NULL;
    }
    return PyLong_FromLong(sts);
}

static inline PyObject *
spam_version(PyObject *self, PyObject *args)
{
    (void)self;
    (void)args;

    return Py_BuildValue("s", "1.0");
}

static PyMethodDef SpamMethods[] = {
    {"system",  spam_system,  METH_VARARGS, "Execute a shell command."},
    {"version", spam_version, METH_VARARGS, "Return the current version." },
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef spamModule =
{
    PyModuleDef_HEAD_INIT,
    "spam",   /* name of module */
    NULL,     /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
               or -1 if the module keeps state in global variables. */
    SpamMethods,
    NULL,
    NULL,
    NULL,
    NULL
};


PyMODINIT_FUNC
PyInit_spam(void)
{
    PyObject *m = PyModule_Create(&spamModule);
    if (m == NULL)
        return NULL;

    SpamError = PyErr_NewException("spam.error", NULL, NULL);
    Py_XINCREF(SpamError);
    if (PyModule_AddObject(m, "error", SpamError) < 0) {
        Py_XDECREF(SpamError);
        Py_CLEAR(SpamError);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}
