import qbs

CppApplication {
    cpp.cxxLanguageVersion: "c++17"
    consoleApplication: true
    files: [
        "imgdiff.cpp",
        "imgdiff.h",
        "imgsource.cpp",
        "imgsource.h",
        "main.cpp",
    ]

    cpp.includePaths: [
        "../cpp-httplib",
        "../timercpp",
        "/usr/local/include/opencv4/"
    ]

    cpp.libraryPaths: [
        "/usr/local/lib/"
    ]

    cpp.dynamicLibraries: [
        "TgBot",
        "ssl",
        "curl",
        "crypto",
        "pthread",
        "opencv_core",
        "opencv_quality",
        "opencv_imgproc",
        "opencv_imgcodecs",
        "boost_system",
    ]

    cpp.defines: [
        "HAVE_CURL"
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
