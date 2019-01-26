
# coding=utf-8

from conans import ConanFile, CMake, tools


class OpenlibraryConan(ConanFile):
    name = "OpenLibrary"
    version = "2.3"
    license = "LGPLv3"
    author = "Michał Walenciak Kicer86@gmail.com"
    url = "https://github.com/Kicer86/openlibrary"
    description = "Open Library is a set of various toolkits."
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=True"
    generators = "cmake"

    def source(self):
        self.run("git clone https://github.com/Kicer86/openlibrary.git")
        self.run("cd openlibrary && git checkout master")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
        tools.replace_in_file("openlibrary/CMakeLists.txt", "project(openlibrary)",
                              '''project(openlibrary)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()''')

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="openlibrary")
        cmake.build()
        cmake.install()

    def package(self):
        self.copy("*.h", dst="include", src="openlibrary")
        self.copy("*openlibrary.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["openlibrary"]
