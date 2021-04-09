# Data-Mining
The Following Code has been written with the course-work of the  subject **CS568 - Data Mining** at **IIT Guwahati** in winter-2021.

Clique is a clustering algorithm used in unsupervised learning. Which helps to Custer the high dimension data in subspace.
We have thought of an Incremental Version of the CLIQUE algorithm which can be used to increase the efficiency and reduce the time for the slighly altered dataset.

The original code source was taken from PyClustering Library which has implimented many clustering algorithm in Python and CPP 14.

The Repository consists of the two folder.
The folder named as Original CLIQUE is the folder having the original clique algorithm written in CPP 14.
The folder named as Incremental_Version_CLIQUE is having the code which we are developing for the incremental version of the algorithm.


Tools Required: Codeblocks with CPP 14.

You can check if you cpp version is 14 or by running **CPP_version_check.cpp** in the codeblocks.

Steps to Run the Code
>>>In Codeblocks:
1. Download the folder by clicking on Code button in github repository.
2. Unzip the open the folder and copy the folder path.
3. Now in codeblocks IDE, Go to Settings Tab, then click on compiler
4. Choose Search directories tab 
5. In that add the folder path under compiler section:
6. For example if folder name is ABC,
      C:\Users\...\ABC
7.Build and Run the program.
>>>It will initially run on Target data file.

B.References:
1. R. Agrawal,  J. Gehrke,  D. Gunopulos, and P. Ragha-van, “Automatic Subspace Clustering of High DimensionalData for Data Mining Applications”.
2. Pyclustering Repository - (https://pyclustering.github.io/)
