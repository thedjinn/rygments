/* vim:set expandtab: */

#include <ruby.h>

/* Python.h redefines macros on Darwin, we don't want any warnings so we 
 * undefine them here first. */
#ifdef _DARWIN_C_SOURCE
    #undef _XOPEN_SOURCE
    #undef SIZEOF_TIME_T
    #undef SIZEOF_LONG
    #undef WORDS_BIGENDIAN
#endif

#include <Python.h>

#define log(x) (printf(">> %s\n",(x)));

struct wrapper_struct {
    PyObject *module;
    PyObject *rygmentize;
};

static VALUE wrapper_class = Qnil;

/* This function places a string at a certain index in the given tuple */
static int put_in_tuple(PyObject *tuple, int index, const char *string) {
    if (string == NULL) {
        return 1;
    }
    
    PyObject *pystr = PyString_FromString(string);
    if (pystr == NULL) {
        return 1;
    }
    
    return PyTuple_SetItem(tuple, index, pystr);
}

/* Initializes the python interpreter and loads the rygments Python 
 * code */
static int initialize(struct wrapper_struct *s) {
    PyObject *module_name;

    /* Patch the Python search path to include the rygments module path */
    setenv("PYTHONPATH", ".", 1);
    
    /* Initialize Python */
    Py_InitializeEx(0);
    
    /* Load the rygments module */
    module_name = PyString_FromString("rygments");
    s->module = PyImport_Import(module_name);
    Py_DECREF(module_name);

    /* Check if module was loaded successfully */
    if (s->module == NULL) {
        PyErr_Print();
        fprintf(stderr, "Failed to load the rygments python module\n");
        return 1;
    }
    
    /* Get a reference to the rygmentize function */
    s->rygmentize = PyObject_GetAttrString(s->module, "rygmentize");

    /* Check if we have a function reference */
    if (!s->rygmentize || !PyCallable_Check(s->rygmentize)) {
        Py_XDECREF(s->rygmentize);
        Py_DECREF(s->module);

        if (PyErr_Occurred()) {
            PyErr_Print();
        }

        fprintf(stderr, "Cannot find the rygmentize function in the python module\n");
        return 1;
    }

    return 0;
}

/* Perform syntax highlighting on a single string */
static VALUE wrapper_highlight_string(VALUE self, VALUE code, VALUE lexer, VALUE formatter) {
    struct wrapper_struct *s;
    PyObject *pArgs, *pValue;
    int res;

    /* Get the wrapper structure */
    Data_Get_Struct(self, struct wrapper_struct, s);

    /* Collect arguments in a tuple */
    pArgs = PyTuple_New(3);
    res = 0;
    res += put_in_tuple(pArgs, 0, RSTRING_PTR(code));
    res += put_in_tuple(pArgs, 1, RSTRING_PTR(lexer));
    res += put_in_tuple(pArgs, 2, RSTRING_PTR(formatter));
    
    /* Check if we have bad arguments */
    if (res) {
        Py_DECREF(pArgs);
        fprintf(stderr,"Bad arguments\n");
        return 1;
    }

    /* Call the function */
    pValue = PyObject_CallObject(s->rygmentize, pArgs);
    Py_DECREF(pArgs);

    /* Check if we have a valid result */
    if (pValue == NULL) {
        fprintf(stderr,"Call failed\n");
        return 1;
    }

    /* Convert the pygmentized result to a Ruby string */
    VALUE result = rb_str_new2(PyString_AsString(pValue));
    Py_DECREF(pValue);

    return result;
}

/* Perform syntax highlighting on a file */
static VALUE wrapper_highlight_file(VALUE self, VALUE filename, VALUE lexer, VALUE formatter) {
    FILE *f;
    if ((f=fopen(RSTRING_PTR(filename),"r"))==NULL) {
        printf("error: could not open file\n");
        exit(EXIT_FAILURE);
    }
    
    fseek(f,0,SEEK_END);
    long fsize=ftell(f);
    rewind(f);
    
    char *buf=ALLOC_N(char, fsize);
    if ((fread(buf,1,fsize,f))!=(size_t)fsize) {
        printf("error: could not read from file \"\"\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }
    fclose(f);

    VALUE code = rb_str_new(buf, fsize);
    free(buf);

    return wrapper_highlight_string(self, code, lexer, formatter);
}

/* Free the wrapper structure and deinitialize the Python interpreter */
static void wrapper_free(void *p) {
    struct wrapper_struct *s = (struct wrapper_struct *)p;
    
    Py_DECREF(s->rygmentize);
    Py_DECREF(s->module);

    if (PyErr_Occurred()) {
        PyErr_Print();
    }
    
    Py_Finalize();

    free(p);
}

/* Create a new instance of the wrapper class */
static VALUE wrapper_new(VALUE klass) {
    struct wrapper_struct *ptr = ALLOC(struct wrapper_struct);
    initialize(ptr);

    VALUE tdata = Data_Wrap_Struct(klass, 0, wrapper_free, ptr);
    return tdata;
}

/* Ruby extension initializer */
void Init_rygments(void) {
    wrapper_class = rb_define_class("Wrapper", rb_cObject);
    rb_define_singleton_method(wrapper_class, "new", wrapper_new, 0);
    rb_define_method(wrapper_class, "highlight_string", wrapper_highlight_string, 3);
    rb_define_method(wrapper_class, "highlight_file", wrapper_highlight_file, 3);
}
