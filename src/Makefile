all	: stem.o
	gcc -g -o ../bin/patricia -lm ../src/driver.c ../include/md5lib.a ../obj/stem.o

stem.o	:
	
	gcc -g -c ../include/stemming.c -o ../obj/stem.o
	


md5Lib.a	:
	gcc -g -c -lm ../include/md5/md5main.c md5.c
	ar rvs md5Lib.a  md5.o md5main.o

index	:
	../bin/patricia -i ../src/idx.conf 


clean	:
	@ rm ../obj/stem.o ../bin/patricia
	#@ rm ../bin/debug
	#@ rm ../bin/hashvalues

clean2	:
	@ rm ../bin/debug
	@ rm ../bin/hashvalues

rmtree	:
	rm ../bin/tree

rmpfiles	:
	rm ../bin/ifiles/*

reload	:
	@rm -rf ../bin/ifiles/000* rm -rf ../bin/ifiles/001* rm -rf ../bin/ifiles/010* rm -rf ../bin/ifiles/100* rm -rf ../bin/ifiles/00* rm -rf ../bin/ifiles/11* rm -rf ../bin/ifiles/0* rm -rf ../bin/ifiles/1*
	rm ../bin/tree ../bin/docdic
	
	
