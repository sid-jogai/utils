set -xe

mkdir -p testdata

curl 'https://openbible.com/textfiles/kjv.txt' > bible.txt
cat bible.txt bible.txt bible.txt bible.txt bible.txt bible.txt bible.txt bible.txt > bible10.txt
cat bible10.txt bible10.txt bible10.txt bible10.txt bible10.txt bible10.txt bible10.txt bible10.txt > bible100.txt
cat bible100.txt bible100.txt bible100.txt bible100.txt bible100.txt bible100.txt bible100.txt bible100.txt > bible1000.txt
mv bible.txt bible10.txt bible100.txt bible1000.txt testdata/

curl 'https://raw.githubusercontent.com/azadkuh/sqlite-amalgamation/refs/heads/master/sqlite3.c' > testdata/sqlite3.txt

yes "hello" | head -n1000 > testdata/repeats_small.txt
yes "world" | head -n1000 >> testdata/repeats_small.txt

yes "one"   | head -n1000000 > testdata/repeats_medium.txt
yes "two"   | head -n1000000 >> testdata/repeats_medium.txt
yes "three" | head -n1000000 >> testdata/repeats_medium.txt
yes "four"  | head -n1000000 >> testdata/repeats_medium.txt
yes "five"  | head -n1000000 >> testdata/repeats_medium.txt

yes "one"   | head -n100000000 > testdata/repeats_large.txt
yes "two"   | head -n100000000 >> testdata/repeats_large.txt
yes "three" | head -n100000000 >> testdata/repeats_large.txt
yes "four"  | head -n100000000 >> testdata/repeats_large.txt
yes "five"  | head -n100000000 >> testdata/repeats_large.txt
yes "six"   | head -n100000000 >> testdata/repeats_large.txt
yes "seven" | head -n100000000 >> testdata/repeats_large.txt
yes "eight" | head -n100000000 >> testdata/repeats_large.txt
yes "nine"  | head -n100000000 >> testdata/repeats_large.txt
yes "ten"   | head -n100000000 >> testdata/repeats_large.txt
