{
    depfiles_gcc = "build/.objs/main/linux/x86_64/release/main.c.o: main.c b_plus_tree.h\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-fvisibility=hidden",
            "-O3",
            "-I/usr/local/include",
            "-I/usr/include",
            "-g"
        }
    },
    files = {
        "main.c"
    }
}