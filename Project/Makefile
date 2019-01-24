TAGET:=bin/iothome
OBJ:=obj/uart.o obj/bejson.o obj/sql.o obj/function.o
OBJ+=main.c

F:=-Wall -g
CC:=gcc

$(TAGET):$(OBJ)
	$(CC) -o $@ -I./include $^ $(F)  -lpthread -ljson -lsqlite3
obj/%.o:src/%.c
	$(CC) -o $@ -I./include -c $^ $(F)

.PHONEY:clean
clean:
		rm -rf $(TAGET)
		rm -rf obj/*.o
		rm -rf *~ *.*~
		rm -rf include/*.*~
		rm -rf src/*.*~

