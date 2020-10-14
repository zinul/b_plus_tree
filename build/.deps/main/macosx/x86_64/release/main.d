{
    files = {
        "build/.objs/main/macosx/x86_64/release/main.c.o",
        "build/macosx/x86_64/release/libb_plus_tree.a"
    },
    values = {
        "/usr/bin/xcrun -sdk macosx clang++",
        {
            "-Lbuild/macosx/x86_64/release",
            "-Wl,-x",
            "-lb_plus_tree",
            "-arch",
            "x86_64",
            "-mmacosx-version-min=10.15",
            "-isysroot",
            "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk",
            "-stdlib=libc++",
            "-lz"
        }
    }
}