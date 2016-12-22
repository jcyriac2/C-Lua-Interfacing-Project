In this project, I worked collaboratively with Prof Alexander Schwing to help him interface a library(Region.h) that he has written from his research. His library is written in C but he would  also like to take advantage of the scientific library "Torch" which is written for Lua. For that reason, I helped in interfacing the C library so that its functions can be called from Lua.

The files:

1. LInterface.cpp
  
  Contains the interfacing code that links the 2 languages together. It defines functions that can be called in Lua and links them to the corresponding functions in the C Library. It also handles passing parameters between the 2 languages through a stack
  
2. TestRMP.cpp
  
  An example C++ code that calls the functions in "Region.h" which is written in C. 

3. Test.lua
  
  This code does the same thing as the TestRMP.cpp file but it is written in Lua. It is used to test the Interfacing code.
  
Note: The "Region.h" file and other related files required to compile the code has not been put here as they are protected until they are published by the Professor. 
