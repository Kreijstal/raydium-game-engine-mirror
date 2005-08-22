<?
// This file is a binding generation demo, using SWIG:
// http://www.swig.org/
// You may generate bindings for many languages, such as
// Perl, Python, Tcl/Tk, Java, (Lua is on the road ...)

// Use PHP command line interface (CLI), it's provided by
// Raydium building scripts.

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
fwrite($fpi,"%module raydium\n%{\n#define NO_PHP_SUPPORT\n#include \"../index.c\"\n%}\n");
// type mappings (may not do it like this, but eh ...)
fwrite($fpi,"%typedef unsigned int GLuint;\n");
fwrite($fpi,"%typedef unsigned int ALuint;\n");
fwrite($fpi,"%typedef unsigned int GLsizei;\n");
fwrite($fpi,"%typedef int GLint;\n");
fwrite($fpi,"%typedef int ALint;\n");
fwrite($fpi,"%typedef float GLfloat;\n");
fwrite($fpi,"%typedef float ALfloat;\n");


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

    $if=substr($file,0,-2).".i";
    $hd="../headers/$file";
//    $fp=fopen("../swig/$if","wt");
//    if($fp===false)
//	die("cannot create interface file (for $file)\n");
    fwrite($fpi,"%module raydium\n%{\n#include \"$hd\"\n%}\n\n%include \"$hd\"\n");
//    fclose($fp);
    }
closedir($h);
fclose($fpi);
chdir("../..");
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

passthru("swig -python raydium.i",&$ret);
echo "\n";
if($ret!=0)
    die("");


chdir("../..");
echo "Compile example (see ocomp.sh for up to date gcc args) for Python:\n";
echo "gcc -g -Wall -shared raydium/swig/raydium_wrap.c -o raydium/swig/_raydium.so -Iode/include/ -L/usr/X11R6/lib/ -lGL -lglut -lGLU -lm -lopenal -Iode/include/ ode/lib/libode.a -lvorbis -lvorbisfile -logg -I /usr/include/python2.3/";
echo "\n";
}


///////////// "main"
generate_interface_file();

//swig_wrappers_perl();
swig_wrappers_python();



