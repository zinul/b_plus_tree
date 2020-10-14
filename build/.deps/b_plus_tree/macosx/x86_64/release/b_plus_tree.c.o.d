{
    depfiles_gcc = "build/.objs/b_plus_tree/macosx/x86_64/release/b_plus_tree.c.o:   b_plus_tree.c b_plus_tree.h\
",
    files = {
        "b_plus_tree.c"
    },
    values = {
        "/usr/bin/xcrun -sdk macosx clang",
        {
            "-fvisibility=hidden",
            "-O3",
            "-g",
            "-Qunused-arguments",
            "-arch",
            "x86_64",
            "-mmacosx-version-min=10.15",
            "-isysroot",
            "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk"
        }
    }
}