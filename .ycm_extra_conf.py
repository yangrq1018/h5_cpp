def Settings(**kwargs):
    return {
        "flags": [
            "-x", "c++",
            "-std=c++17",
            "-Wextra",
            "-Werror",
            "-IHighFive/include",
            "-I/usr/include/hdf5/serial",
            "-L/usr/lib/x86_64-linux-gnu/hdf5/serial/",
            "-lhdf5",
        ]
    }
