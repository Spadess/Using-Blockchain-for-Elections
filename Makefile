PROGRAMS = exec1 exec2 exec3 exec4 exec5 exec6 exec7

.PHONY:	all clean

all: $(PROGRAMS)

exec1: main1.o partie1.o
	gcc -Wall -o exec1 -g main1.o partie1.o -lm

exec2: main2.o partie2.o partie1.o
	gcc -Wall -o exec2 -g main2.o partie2.o partie1.o -lm

exec3: main3.o partie3.o partie2.o partie1.o
	gcc -Wall -o exec3 -g main3.o partie3.o partie2.o partie1.o -lm

exec4: main4.o partie4.o partie3.o partie2.o partie1.o
	gcc -Wall -o exec4 -g main4.o partie4.o partie3.o partie2.o partie1.o -lm

exec5: main5.o partie5.o partie4.o partie3.o partie2.o partie1.o
	gcc -Wall -o exec5 -g main5.o partie5.o partie4.o partie3.o partie2.o partie1.o -lssl -lcrypto -lm

exec6: main6.o partie6.o partie5.o partie4.o partie3.o partie2.o partie1.o
	gcc -Wall -o exec6 -g main6.o partie6.o partie5.o partie4.o partie3.o partie2.o partie1.o -lssl -lcrypto -lm

exec7: main7.o partie7.o partie6.o partie5.o partie4.o partie3.o partie2.o partie1.o
	gcc -Wall -o exec7 -g main7.o partie7.o partie6.o partie5.o partie4.o partie3.o partie2.o partie1.o -lssl -lcrypto -lm


main1.o: main1.c partie1.h
	gcc -Wall -c -g main1.c -lm

main2.o: main2.c partie2.h
	gcc -Wall -c -g main2.c -lm

main3.o: main3.c partie3.h
	gcc -Wall -c -g main3.c -lm

main4.o: main4.c partie4.h
	gcc -Wall -c -g main4.c -lm

main5.o: main5.c partie5.h
	gcc -Wall -c -g main5.c -lssl -lcrypto -lm

main6.o: main6.c partie6.h
	gcc -Wall -c -g main6.c -lssl -lcrypto -lm

main7.o: main7.c partie7.h
	gcc -Wall -c -g main7.c -lssl -lcrypto -lm


partie1.o: partie1.c partie1.h
	gcc -Wall -c -g partie1.c -lm

partie2.o: partie2.c partie2.h
	gcc -Wall -c -g partie2.c -lm

partie3.o: partie3.c partie3.h
	gcc -Wall -c -g partie3.c -lm

partie4.o: partie4.c partie4.h
	gcc -Wall -c -g partie4.c -lm

partie5.o: partie5.c partie5.h
	gcc -Wall -c -g partie5.c -lssl -lcrypto -lm

partie6.o: partie6.c partie6.h
	gcc -Wall -c -g partie6.c -lssl -lcrypto -lm

partie7.o: partie7.c partie7.h
	gcc -Wall -c -g partie7.c -lssl -lcrypto -lm

clean:
	rm -f *.o *~ $(PROGRAMS) modpow.txt candidates.txt keys.txt declarations.txt computePOW.txt Blockchain/*
