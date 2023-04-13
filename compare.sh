if [ $# -lt 1 ]; then
    echo "No input file given. To run: ./compare.sh filename"
    exit 1
fi

INPUT_FILE=$1
OUTPUT_FILE=encoded.huf

if test -f $OUTPUT_FILE; then
    echo "File: encoded.txt already exists in the working directory. compare.sh uses requires this file, so it must be deleted to run compare.sh"
    exit 1
fi

touch $OUTPUT_FILE > /dev/null

make > /dev/null

echo -n -e "\nThe size of the input file is: "

du -b -h $INPUT_FILE | cut -f1

echo -n -e "\nEncoding with 1-character sequence compression: \n The size of the encoded file is: "

./encode $INPUT_FILE $OUTPUT_FILE > /dev/null

du -b -h $OUTPUT_FILE | cut -f1

echo -n -e "\nEncoding with 2-character sequence compression:\n The size of the encoded file is: "

./encode2 $INPUT_FILE $OUTPUT_FILE > /dev/null

du -b -h $OUTPUT_FILE | cut -f1

echo ""

make clean > /dev/null

rm -f $OUTPUT_FILE > /dev/null
