# DataSetup.sh <inputpath> <outputpath>
# check if <outputpath> exists
if [ ! -d $2 ]; then
    mkdir $2
fi
# append '/' to <outputpath> if it doesn't have one
if [ "${2: -1}" != "/" ]; then
    outputpath=$2"/"
else
    outputpath=$2
fi
./preprocess $1 $outputpath
