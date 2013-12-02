Parallel-connected-components
=============================

This project implements a randomized parallel algorithm to find the number of connected components in a massive graph. It is based on Intel's Cilk++ framework. This project covers three different randomized parallel algorithms.


Background:

These algorithms are entirely based on lecutures by Professor Rezaul at Stony Brook University. http://www.cs.sunysb.edu/~rezaul/Spring-2013/CSE638/CSE638-lectures-10-11.pdf.

First algorithm (implemented in Par_Rand_CC1.cpp) is covered in the above lecture. The other two algorithms (for Par_Rand_CC2.cpp and Par_Rand_CC3.cpp) can be found in the file Algorithms.pdf. 


Running the code:

Run the make file. Then run the code as:

./Par_Rand_CC1 < testGraph.txt


Details of tests:

The algorithms were tested on publicly available graph datasets in Lonestar supercomputer (with allocation of 12 physical cores). Details of tests below:

Graph:      Vertices:   Edges:  CC_1_Running_Time: CC_2_Running_Time: CC_3_Running_Time:

as-skitter  1.7M        11M     2.96s               2.14s             20.78s
ca-AstroPh  18.7K       396K    0.16s               0.86s             0.72s
com-lj      4M          34M     12.28s              7.78s             74.78s
com-orkut   3M          117M    43.31s              26.83s            222.97s
com-amazon  334K        925K    0.41s               0.18s             1.38s
