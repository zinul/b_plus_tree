{
    files = {
        "main.c"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-O3",
            "-I/usr/local/include",
            "-I/usr/include"
        }
    },
    depfiles_gcc = "build/.objs/main/linux/x86_64/release/main.c.o: main.c b_plus_tree.h\
"
}