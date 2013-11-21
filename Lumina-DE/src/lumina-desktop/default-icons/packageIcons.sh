#!/bin/sh
#Quick script for packaging up all the icons that are going to be 
#installed onto the local system, and create a file listing all the 
#contents

ICONLIST="FILELIST"
#clear the list file
echo "" > ${ICONLIST}
#fill the list file
for i in `find . | grep png`
do
echo `basename ${i}` >> ${ICONLIST}
done
#Now package all the icons into a tar.gz file

#Now remove the icons
