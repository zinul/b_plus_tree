{
    values = {
        "/usr/bin/g++",
        {
            "-m64",
            "-Lbuild/linux/x86_64/release",
            "-s",
            "-L/usr/local/lib",
            "-L/usr/lib",
            "-lb_plus_tree"
        }
    },
    files = {
        "build/.objs/main/linux/x86_64/release/main.c.o",
        "build/linux/x86_64/release/libb_plus_tree.a"
    }
}