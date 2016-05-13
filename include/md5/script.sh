gcc -c -lm md5main.c md5.c

ar rvs ../md5lib.a  md5.o md5main.o

rm md5.o md5main.o

#gcc -o md5 -lm new.c mylib.a
