CMD="clang++ main.cpp -o out -lraylib -Wextra -Wall -v"

for arg in "$@"; do
    CMD+=" -D $arg"
done

echo "$CMD"
$CMD
if [ $? -eq 0 ]; then
    echo -e "\e[32m    Build succeeded\e[0m"
    ./out
else
    echo -e "\e[31m!!! Build FAILED !!!\e[0m"
fi
