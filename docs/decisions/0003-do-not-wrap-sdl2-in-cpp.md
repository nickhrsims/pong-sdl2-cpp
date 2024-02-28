# Decision 0003 - Do not wrap SDL2 in C++

Do not, as part of the project, write a C++ wrapper around SDL2; neither for
style reasons nor for "paradigmatic" reasons. C++ is a multi-paradigm language,
and the project is certainly small enough to tolerate paradigm mixing where
appropriate without loss of clarity.
