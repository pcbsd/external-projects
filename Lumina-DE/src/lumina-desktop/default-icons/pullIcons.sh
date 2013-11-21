#!/bin/sh

for i in `find ${1} | grep png | grep -v _`
do
cp ${i} .
done
