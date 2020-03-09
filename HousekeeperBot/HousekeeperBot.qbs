import qbs

CppApplication {
    cpp.cxxLanguageVersion: "c++17"
    consoleApplication: true
    files: [
        "imgsource.cpp",
        "imgsource.h",
        "main.cpp",
    ]

    cpp.includePaths: [
        "../cpp-httplib",
        "../timercpp",
        "/usr/local/include/opencv4/"
    ]

    cpp.dynamicLibraries: [
        "TgBot",
        "boost_system",
        "ssl",
        "crypto",
        "curl",
        "pthread",
        "opencv_core",
        "opencv_video",
        "opencv_videoio",
        "opencv_imgcodecs",
        "opencv_imgproc",
    ]

    /*-lTgBot -lboost_system -lpthread -lssl -lcrypto -lcurl HAVE_CURL*/
    cpp.defines: [
        "HAVE_CURL"
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "bin"
    }
}
