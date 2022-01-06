main_dir = ./main/
hashtable_dir = ./hashtable/
holdall_dir = ./holdall/
linked_list_dir = ./linked_list/
context_dir = ./context/
opt_dir = ./opt/
lang_dir = ./lang/

CC = gcc
CFLAGS = -std=c18 \
  -Wall -Wconversion -Werror -Wextra -Wfatal-errors -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(main_dir) \
  -I$(hashtable_dir) \
  -I$(holdall_dir) \
  -I$(linked_list_dir) \
  -I$(context_dir) \
  -I$(opt_dir) \
  -I$(lang_dir)

LDFLAGS =

vpath %.c $(main_dir)
vpath %.h $(main_dir)

vpath %.c $(hashtable_dir)
vpath %.h $(hashtable_dir)

vpath %.c $(holdall_dir)
vpath %.h $(holdall_dir)

vpath %.c $(linked_list_dir)
vpath %.h $(linked_list_dir)

vpath %.c $(context_dir)
vpath %.h $(context_dir)

vpath %.c $(opt_dir)
vpath %.h $(opt_dir)

objects = main.o hashtable.o holdall.o linked_list.o context.o opt.o 
executable = ws

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) $(LDFLAGS) -o $(executable)

main.o: main.c hashtable.h holdall.h linked_list.h context.h opt.h
hashtable.o: hashtable.c hashtable.h
holdall.o: holdall.c holdall.h
linked_list.o: linked_list.c linked_list.h
context.o: context.c context.h
opt.o: opt.c opt.h