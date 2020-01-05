The folder contains the source code of the tool BHIM. The program is written in C++ programming language. The project is compiled using CMake version 3.10, which is available in the virtual machine provided for TACAS'20 artifact evaluation. The libraries used in the tool are (i) boost and (ii) tinyxml, and they are bundled with the source code provided with the bhim package.

# Structure of The code

The "bhim" directory contains the following folders, 
1) Benchmark in Paper: This includes the benchmarks and scripts to reproduce the tables in the paper.
2) src: This folder contains the *.cpp files.
3) include: This folder contains the header files used in bhim.


The compilation can be done following the instructions below,

# BUILDING BHIM:

1. make sure the extracted folder contains an "include" folder with header files of the tool, an "src" folder which contains the source files of the tool, and a "CMakeLists.txt" file, which will be used by CMake to build the tool.
2. Make a directory named build  and change directory into it following the command:
    $ mkdir build
    $ cd build
3. Then use cmake to compile BHIM by using the following command:
    $ cmake .. 
4. This command will produce an executable binary named "bhim" under the build directory.


# RUNNING BHIM:

The benchmark examples published in the paper are stored in the directory called "Benchmarks in Paper." The benchmarks are of two types, Timed and unTimed. They are stored under two separate directories under "Benchmarks in Paper" directory named "Timed" and "unTimed" respectively. 

BHIM takes command line argument parameters as follows

$ cd PATH_TO_BHIM_BINARY
$ ./bhim -h HOLE_BOUND -x INPUT_FILE

The command line parameter HOLE_BOUND is the maximum hole bound allowed, and the INPUT_FILE is the path to the Multistack Pushdown Automata(timed or Untimed) for which we want to check emptiness. The input format is UPPAAL compatible, i.e., the Multistack Pushdown Automata is drawn in UPPAL and saved as a .xml file. Note that UPPAAL can not handle stack operations; hence we use the synchronization parameters in UPPAAL to denote stack operations. 

# GENERATING TABLES:
To quickly reproduce the results in the paper, we created a directory named "All" under the "Benchmarks in Paper" directory. Under this directory, you will find two folders for "timed" and "untimed" benchmarks. Each such directory contains a shell script named "GenerateTable.sh."
You can run the shell script as follows:
    $ sh GenerateTable.sh
this should produce the table for untimed and timed experiments, respectively.

P.S: Note that the directory should contain  "bhim" executable along with the benchmark examples.

# Configuration of the Host machine
CPU: Intel i7-4770K (8) @ 3.900GHz
RAM: 16GB

