#!/bin/bash

LFLAGS='-t 4'
for cpp_file in `ls *.cpp` ; do
    name=`echo $cpp_file | sed 's/\.cpp$//'`
    lgrind $LFLAGS -i $cpp_file > tex/$name.tex
done

