The folder contains the source code of the tool BHIM. The program is written in C++ programming language. The project is compiled using CMake version 3.10, which is available in  the virtual machine. The libraries used in the tool are (i) boost and (ii) tinyxml and they are bundled with the source code provided with the distribution. 

# Structure of The code





The compilation can be done following the instructions below,

# BUILDING BHIM:

1. make sure the extracted folder contains a include folder which contains the header files of the tool, a src folder which contains the source files of the tool and a CMakeLists.txt file. Which will be used to by  CMake to build the tool.
2. Make a directory named build  and change directory into it following the command:
	$ mkdir build
	$ cd build
3. Then use cmake to compile BHIM by using the following command:
	$ cmake .. 
4. This command will produce a executable binary named "bhim" under the build directory.


# RUNNING BHIM:

The benchmark examples published in the paper is stored in the directory called "Benchmarks in Paper". The benchmarks are of two types, Timed and unTimed. They are stored under two separate directories under "Benchmarks in Paper" directory named "Timed" and "unTimed" respectively. 

BHIM takes command line argument parameters as follows

$ cd PATH_TO_BHIM_BINARY
$ ./bhim -h HOLE_BOUND -x INPUT_FILE

the command line parameter HOLE_BOUND is the maximum hole bound allowed and the INPUT_FILE is the path to the Multistack Pushdown Automata(timed or Untimed) for which we want to check emptiness. The input format is UPPAAL compatible, i.e. the Multistack Pushdown Automata is drawn in UPPAL and saved as an .xml file. Note that, UPPAAL can not handle stack operations, hence we use the synchronization parameters in UPPAAL to denote stack operations. 

# Appendix Examples 

# Configuration of Host machine

