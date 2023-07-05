mkdir a
echo > a/b
echo > b
find . b
find . b | xargs echo a