<?
// This file is a binding generation demo, using SWIG:
// http://www.swig.org/
// You may generate bindings for many languages, such as
// Perl, Python, Tcl/Tk, Java, (Lua is on the road ...)

// To run this script, use PHP command line interface (CLI), it's provided by
// Raydium building scripts (ex: php/sapi/cli/php gen_bindings.php)

// Warning, this is an experimental and mostly unsupported feature, only
// usable with Python, for now.
// Feedback's welcome.
// See output in raydium/swig directory and use compile wrapper using provided
// instruction, with a previously "./configured" Raydium engine.

$nobindings="//!NOBINDINGS";

//////////// functions

// This function will generate "i" file for SWIG.
// this file is needed for every language.
function generate_interface_file()
{
global $nobindings;

if(chdir("raydium/headers")===false)
    die("not in Raydium's root dir\n");

if(!file_exists("../swig") && mkdir("../swig")===false)
    die("cannot create raydium/swig subdirectory\n");

$fpi=fopen("../swig/raydium.i","wt");
if($fpi===false)
    die("cannot create interface file");
fwrite($fpi,"%module raydium\n%{\n#include \"../index.c\"\n%}\n");
//fwrite($fpi,"%module raydium\n%{\n#define NO_PHP_SUPPORT\n#include \"../index.c\"\n%}\n");
// type mappings (may not do it like this, but eh ...)
fwrite($fpi,"%typedef unsigned int GLuint;\n");
fwrite($fpi,"%typedef unsigned int ALuint;\n");
fwrite($fpi,"%typedef unsigned int GLsizei;\n");
fwrite($fpi,"%typedef int GLint;\n");
fwrite($fpi,"%typedef int ALint;\n");
fwrite($fpi,"%typedef float GLfloat;\n");
fwrite($fpi,"%typedef float ALfloat;\n");
fwrite($fpi,"%typedef float dReal;\n");
fwrite($fpi,"%module raydium\n%{\n#include \"../common.h\"\n%}\n\n%include \"../common.h\"\n");


$h=opendir(".");
while(false !== ($file = readdir($h)))
    if(substr($file,-2,2)==".h")
    {
    $fp=fopen($file,"rt");
    $l=fgets($fp);
    fclose($fp);
    if(substr($l,0,strlen($nobindings))==$nobindings)
	{
	echo "ignoring $file (asked by header)\n";
	continue;
	}

    $hd="../headers/$file";
    fwrite($fpi,"%module raydium\n%{\n#include \"$hd\"\n%}\n\n%include \"$hd\"\n");
    }

closedir($h);


chdir("..");
$h=opendir(".");
while(false !== ($file = readdir($h)))
    if(substr($file,-2,2)==".h" && $file!="common.h")
    {
    $fp=fopen($file,"rt");
    $l=fgets($fp);
    fclose($fp);
    if(substr($l,0,strlen($nobindings))==$nobindings)
	{
	echo "ignoring $file (asked by header)\n";
	continue;
	}

    $hd="../$file";
    fwrite($fpi,"%module raydium\n%{\n#include \"$hd\"\n%}\n\n%include \"$hd\"\n");
    }

closedir($h);


fclose($fpi);
chdir("..");
echo "\n";
}



// Will use interface file to generate Perl5 wrappers, using SWIG
function swig_wrappers_perl()
{
if(chdir("raydium/swig")===false)
    die("not in Raydium's root dir, cannot find swig dir\n");

passthru("swig -perl5 raydium.i",&$ret);
echo "\n";
if($ret!=0)
    die("");


chdir("../..");
echo "Compile example (see ocomp.sh for up to date gcc args) for Perl5:\n";
echo "gcc -g -Wall -shared raydium/swig/raydium_wrap.c -o raydium/swig/raydium.so -Iode/include/ -Iphp/ -Iphp/include -Iphp/main/ -Iphp/Zend -Iphp/TSRM -I/usr/lib/perl5/5.8.3/i386-linux-thread-multi/CORE/ -D_REENTRANT -D_GNU_SOURCE -DTHREADS_HAVE_PIDS -fno-strict-aliasing -I/usr/local/include -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -I/usr/include/gdbm  -L/usr/X11R6/lib/ -lGL -lglut -lGLU -lm -lopenal -Iode/include/ ode/lib/libode.a -lvorbis -lvorbisfile -logg -Iphp/ -Iphp/include -Iphp/main/ -Iphp/Zend -Iphp/TSRM php/libs/libphp4.a -lresolv -lcrypt -lz";
echo "\n";
}

// Will use interface file to generate Python wrappers, using SWIG
function swig_wrappers_python()
{

$argvmap=<<<EOHC

%include exception.i

// Type mapping for grabbing a FILE * from Python
%typemap(python,in) FILE * {
  if (!PyFile_Check(\$input)) {
        PyErr_SetString(PyExc_TypeError, "Need a file!");
	return NULL;
	}
    \$1 = PyFile_AsFile(\$input);
}

// Grab a Python function object as a Python object.
%typemap(python,in) PyObject *pyfunc {
  if (!PyCallable_Check(\$input)) {
        PyErr_SetString(PyExc_TypeError, "Need a callable object!");
	return NULL;
	}
    \$1 = \$input;
}

%typemap(in) (int argc, char **argv) {
  int i;
  if (!PyList_Check(\$input)) {
    SWIG_exception(SWIG_ValueError, "Expecting a list");
  }
  $1 = PyList_Size(\$input);
  if (\$1 == 0) {
    SWIG_exception(SWIG_ValueError, "List must contain at least 1 element");
  }
  \$2 = (char **) malloc((\$1+1)*sizeof(char *));
  for (i = 0; i < \$1; i++) {
    PyObject *s = PyList_GetItem(\$input,i);
    if (!PyString_Check(s)) {
      free(\$2);
      SWIG_exception(SWIG_ValueError, "List items must be strings");
    }
    \$2[i] = PyString_AsString(s);
  }
  \$2[i] = 0;
}
EOHC;

if(chdir("raydium/swig")===false)
    die("not in Raydium's root dir, cannot find swig dir\n");

$fp=fopen("raydium.i","rt");
$contents = '';
while (!feof($fp)) 
    {
    $contents .= fread($fp, 8192);
    }
fclose($fp);

$fp=fopen("raydium.i","wt");
fwrite($fp,$argvmap."\n".$contents);
fclose($fp);

$contents=<<<EOHD
%{
PyObject *MainLoopPython;

static void MainLoopPythonCallback(void)
{
PyObject *result;
PyObject *arglist;

arglist = Py_BuildValue("()");

result=PyEval_CallObject(MainLoopPython,arglist);
Py_DECREF(arglist);
if (result == Py_None) 
    {
    return;
    }

if (result) 
    {
    Py_DECREF(result);
    }
return;
}

%}


%module raydium
%{
void SetMainLoopPython(PyObject *pyfunc)
{
MainLoopPython = pyfunc;
Py_INCREF(MainLoopPython);
raydium_callback(MainLoopPythonCallback);
}

%}

void SetMainLoopPython(PyObject *pyfunc);

void exit (int status);

EOHD;

$fp=fopen("raydium.i","at");
fwrite($fp,$contents);
fclose($fp);

passthru("swig -python raydium.i",&$ret);
echo "\n";
if($ret!=0)
    die("");


chdir("../..");
echo "Compile example (see ocomp.sh for up to date gcc args) for Python:\n";
echo "gcc -g -Wall -shared raydium/swig/raydium_wrap.c -o raydium/swig/_raydium.so -Iode/include/ -L/usr/X11R6/lib/ -lGL -lglut -lGLU -lm -ljpeg -lopenal -Iode/include/ ode/lib/libode.a -lvorbis -lvorbisfile -logg -I/usr/include/python2.3/  -Iphp/ -Iphp/include -Iphp/main/ -Iphp/Zend -Iphp/TSRM php/libs/libphp4.a -lresolv -lcrypt -lz";
echo "\n";
}


///////////// "main"
generate_interface_file();

//swig_wrappers_perl();
swig_wrappers_python();



