main:b_plus_tree.c main.c Makefile
	make clean
	clang -g -c b_plus_tree.c -o objs/b_plus_tree.o
	clang -g -c main.c -o objs/main.o
	clang -g objs/main.o objs/b_plus_tree.o -o objs/main
	./objs/main
clean:
	rm -f objs/*.o
	rm -f objs/main
run:
	./objs/main
debug:
	lldb objs/main