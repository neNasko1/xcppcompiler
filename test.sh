./compile.sh

for file in test-suite/input/*
do	
	NC='\033[0m'
	RED='\033[0;31m'
	GREEN='\033[0;32m'	
	
	filename="$(basename $file)"
	printf "${NC}$filename "

	if cmp --silent -- test-suite/current-output/"$filename" test-suite/output/"$filename"; then
  		printf "${GREEN}CORRECT \n"
	else
  		printf "${RED}WRONG \n"
	fi
	
	./run.sh "$file" test-suite/current-output/"$filename"
done
