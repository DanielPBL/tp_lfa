#!/bin/bash

rm *.dot *.pdf &>/dev/null

cd ..

make &>/dev/null

cd tests

../build/gerador $1 $2

for i in $(ls | grep '.dot$'); do
	dot -Tpdf -o $(echo $i | sed -e 's/.dot//').pdf $i
done
