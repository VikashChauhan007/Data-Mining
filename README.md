# Data-Mining
The Following Code has been written with the course-work of the  subject **CS568 - Data Mining** at **IIT Guwahati** in winter-2021.

Clique is a clustering algorithm used in unsupervised learning. Which helps to Cluster the high dimensional data in it's subspace.
We have thought of an Incremental Version of the CLIQUE algorithm. Which can increase the efficiency and reduce the running time for the slighly altered dataset.

The original source code was taken from PyClustering Library which has implimented many clustering algorithm in Python and CPP 14.

The Repository consists of the two folder.
The folder named as "Original CLIQUE" is the folder having the original clique algorithm written in CPP 14.
The folder named as "Incremental_Version_CLIQUE" is having the code which we are developing for the incremental version of the algorithm.


Tools Required: Codeblocks with CPP 14.

You can check if your cpp version is 14 or not by running **CPP_version_check.cpp** in the codeblocks.


> Steps to Run the Code:
1. Download the folders in desired folder.
2. Unzip the folder.
3. Open Codeblocks IDE.
4. Choose **Open an existing project** option. Select the CLIQUE project file by navigating to the code directory.
5. Click on **Settings** from the top pane. Under the dropdown choose **Compiler**.
6. Choose **Build Options** from the below pane. 
7. Check the option of **Explicitly add currently compiling file's directiry to compiler search dirs**
7.Build and Run the program.


> References:
1. R. Agrawal,  J. Gehrke,  D. Gunopulos, and P. Ragha-van, “Automatic Subspace Clustering of High DimensionalData for Data Mining Applications”.
2. Pyclustering Repository - (https://pyclustering.github.io/)
