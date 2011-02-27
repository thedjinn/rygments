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

static VALUE rygments_class = Qnil;
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

/*
 * call-seq:
 *   wrapper.highlight_string(string, lexer, formatter) -> string
 *
 * This method performs syntax highlighting on a string. Do not call this
 * function directly. Instead, use the Rygments.highlight functions.
 *
 * The return value is always a string. An exception is raised whenever
 * an error occurs.
 */
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

/*
 * call-seq:
 *   wrapper.highlight_file(filename, lexer, formatter) -> string
 *
 * This method performs syntax highlighting on a file. Do not call this
 * function directly. Instead, use the Rygments.highlight functions.
 *
 * The return value is always a string. An exception is raised whenever
 * an error occurs.
 */
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

/* 
 * call-seq:
 *   Wrapper.new(helper_path) -> Wrapper instance
 *
 * Creates a new instance of the Python wrapper. Do not use this
 * function directly. Instead, use the Rygments.highlight functions
 * which will take care of handling the wrapper.
 *
 * Note: The Wrapper should be treated as a singleton. It is not designed
 * to handle multiple instances. Furthermore, on Darwin initializing,
 * finalizing and again initializing the Python interpreter results in
 * segmentation faults.
 */
static VALUE wrapper_new(VALUE klass, VALUE path) {
    /* Patch the Python search path to include the rygments module path */
    setenv("PYTHONPATH", RSTRING_PTR(path), 1);
    
    struct wrapper_struct *ptr = ALLOC(struct wrapper_struct);
    initialize(ptr);

    VALUE tdata = Data_Wrap_Struct(klass, 0, wrapper_free, ptr);
    return tdata;
}

/* 
 * This Ruby C extension defines a Rygments::Wrapper class.
 *
 * The Rygments::Wrapper class is a wrapper around the Python interpreter.
 * This means that Pygments can be invoked multiple times without having
 * to fork or reload anything, thus reducing the overhead to a minimum.
 *
 * As a Rygments user you do not have to deal with the wrapper system
 * directly. Instead, you should use the static functions that are exposed 
 * in the Rygments module.
 */
void Init_wrapper(void) {
    rygments_class = rb_define_module("Rygments");
    wrapper_class = rb_define_class_under(rygments_class, "Wrapper", rb_cObject);
    rb_define_singleton_method(wrapper_class, "new", wrapper_new, 1);
    rb_define_method(wrapper_class, "highlight_string", wrapper_highlight_string, 3);
    rb_define_method(wrapper_class, "highlight_file", wrapper_highlight_file, 3);
}
