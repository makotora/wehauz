OBJS 	= main.o cdr_info.o info_list.o superfasthash.o call_stats.o hash_table.o utilities.o heap.o input.o
SOURCE	= main.cpp cdr_info.cpp info_list.cpp superfasthash.cpp call_stats.cpp hash_table.cpp utilities.cpp heap.cpp input.cpp
HEADER  = cdr_info.h superfasthash.h call_stats.h hash_table.h utilities.h heap.h input.h
OUT  	= werhauz
CC	= g++
CFLAGS= -c -Wall $(DEBUG)
LFLAGS= -Wall $(DEBUG)

all: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(OUT)

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

cdr_info.o: cdr_info.cpp
	$(CC) $(CFLAGS) cdr_info.cpp

info_list.o: info_list.cpp
	$(CC) $(CFLAGS) info_list.cpp

superfasthash.o: superfasthash.cpp
	$(CC) $(CFLAGS) superfasthash.cpp

hash_table.o: hash_table.cpp
	$(CC) $(CFLAGS) hash_table.cpp

utilities.o: utilities.cpp
	$(CC) $(CFLAGS) utilities.cpp

heap.o: heap.cpp
	$(CC) $(CFLAGS) heap.cpp

input.o: input.cpp
	$(CC) $(CFLAGS) input.cpp


clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)
