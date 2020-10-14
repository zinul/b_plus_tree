{
    depfiles_gcc = "build/.objs/b_plus_tree/linux/x86_64/release/b_plus_tree.c.o:  b_plus_tree.c b_plus_tree.h\
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
        "b_plus_tree.c"
    }
}