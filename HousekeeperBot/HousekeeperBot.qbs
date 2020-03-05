import qbs

CppApplication {
    cpp.cxxLanguageVersion: "c++17"
    consoleApplication: true
    files: "main.cpp"

    cpp.dynamicLibraries: [
        "TgBot",
        "boost_system",
        "ssl",
        "crypto",
        "curl",
        "pthread"
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
