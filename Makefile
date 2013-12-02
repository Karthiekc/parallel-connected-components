all: par1 par2 par3

par1: Par_Rand_CC1.cpp
	cilk++ -O3  -o Par_Rand_CC1 Par_Rand_CC1.cpp -lcilkutil

par2: Par_Rand_CC2.cpp
	cilk++ -O3  -o Par_Rand_CC2 Par_Rand_CC2.cpp -lcilkutil

par3: Par_Rand_CC3.cpp
	cilk++ -O3  -o Par_Rand_CC3 Par_Rand_CC3.cpp -lcilkutil
