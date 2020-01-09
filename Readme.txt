The folder contains the source code of the tool BHIM. The program is written in C++ programming language. 
The project is compiled using CMake version 3.10, which is available in the virtual machine provided 
for TACAS'20 artifact evaluation. The libraries used in the tool are (i) boost and (ii) tinyxml, 
and they are bundled with the source code provided with the bhim package.

# Structure of The code

The "bhim" directory contains the following folders, 
1) Benchmark in Paper: This includes the benchmarks and scripts to reproduce the tables in the paper.
2) src: This folder contains the *.cpp files.
3) include: This folder contains the header files used in bhim.


The compilation can be done following the instructions below,

# BUILDING BHIM:

1. make sure the extracted folder contains an "include" folder with header files of the tool, 
an "src" folder which contains the source files of the tool, and a "CMakeLists.txt" file,
which will be used by CMake to build the tool.
2. Make a directory named build  and change directory into it following the command:
    $ mkdir build
    $ cd build
3. Then use cmake to compile BHIM by using the following command:
    $ cmake .. 
4. Then execute make to compile 
    $ make
5. This command will produce an executable binary named "bhim" under the build directory.


# RUNNING BHIM:

The benchmark examples published in the paper are stored in the directory called "Benchmarks in Paper". 
The benchmarks are of two types, Timed and unTimed. They are stored under two separate directories under 
"Benchmarks in Paper" directory named "Timed" and "unTimed" respectively. 

BHIM can be executed as follows:

$ cd PATH_TO_BHIM_BINARY
$ ./bhim -h HOLE_BOUND -x INPUT_FILE

The command line parameter "-h HOLE_BOUND" is the maximum hole bound allowed, 
and the "-x INPUT_FILE" is the path to the Multistack Pushdown Automata (timed or un-timed) for which 
we want to check emptiness/reachability. Both of the parameters are required. The input format is UPPAAL
compatible, i.e., the Multistack Pushdown Automata is drawn in UPPAAL and saved as a ".xml" file. Note that,
UPPAAL can not handle stack operations; hence we use the synchronization parameters of UPPAAL to denote
stack operations. 

To make easy execution each benchmark files, we added a script called "run.sh" to each benchmark directory. 
The parameters are already given in the scripts and just running it will give you the respective row of the
tables presented in the paper.

For example, if you want to run the "Bluetooth" benchmark, you need to first go to "Timed" or "unTimed" directory
under the "Benchmarks of Paper" directory. Then go to "Bluetooth" directory and run the scripts. The direction is given
as follows,
    $ cd ${EXTRACTED_DIR}/bhim/Benchmarks in Paper/Timed
    $ cd Bluetooth
    $ sh run.sh

    
P.S Make sure you have built bhim following the instructions above and the executable "bhim" is there in the "build" directory.


# GENERATING TABLES:
To quickly reproduce the results in the paper, we created a shell script under "timed" and "untimed" directories named "GenerateTable.sh". The shell script assumes the existence of "build" directory and "bhim" executable inside it. So before running the scripts make sure that the "build" directory exists and "bhim" executable built following the "BUILD BHIM" section.

You can run the shell script from the directory (e.g. ${EXTRACTED_DIR}/bhim/Benchmarks in Paper/Timed or ${EXTRACTED_DIR}/bhim/Benchmarks in Paper/unTimed) as follows:
    $ sh GenerateTable.sh
this should produce the table for untimed and timed experiments, respectively.

# Parameterized Examples

We also run some experiments to check scalability of "BHIM"
These experiments are part of the Appendix of the submission. We used the examples 
"Lcrit (timed)" and "Lbh" to run "BHIM" varying different parameters like "holes",
"clocks", "Maximum constraint" and "state-transitions". The benchmarks are in the
directory named "Appendix" which contains folders for each parameters and benchmarks.
And each folder contains file/files and a script "run.sh". "run.sh" will run the examples with varying parameters and will generate the tables. 

P.S: Some examples can take time to run so it will take some time to generate the whole 
table.

# Configuration of the Host machine
CPU: Intel i7-4770K (8) @ 3.900GHz
RAM: 16GB

