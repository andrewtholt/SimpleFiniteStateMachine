BINS=fsm

fsm:	fsm.c fsm.h
	$(CC) -g fsm.c -o fsm

clean:
	rm -f *.o $(BINS)
