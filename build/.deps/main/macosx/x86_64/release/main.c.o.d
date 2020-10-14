{
    depfiles_gcc = "build/.objs/main/macosx/x86_64/release/main.c.o: main.c b_plus_tree.h\
",
    files = {
        "main.c"
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