
import os
import platform
import sys

CPP_STANDARD = "-std=c++17"
# Can't include "./src/*.c" here because clang++ complains.
# Should probably create a glad compiled C output to link to.
# For now, just changing "glad.c" to "glad.cpp"
SOURCE = "./src/*.cpp"
EXECUTABLE_NAME = "lab"
DEFAULT_INCLUDES = "-I ./include/"

CLANG_COMPILER = "clang++"
GNU_COMPILER = "g++"

compiler = ""
includes = ""
frameworks = ""
options = ""
executable = ""
libraries = ""

debug = False

if len(sys.argv) >= 2 and sys.argv[1] == "--debug":
    debug = True

if platform.system() == "Linux":
    compiler = CLANG_COMPILER
    includes = DEFAULT_INCLUDES
    libraries = "-l SDL2 -l dl"
    executable = EXECUTABLE_NAME
elif platform.system() == "Darwin":
    compiler = CLANG_COMPILER
    includes = DEFAULT_INCLUDES + " -I /Library/Frameworks/SDL2.framework/Headers"
    frameworks = "-F /Library/Frameworks -framework SDL2"
    executable = EXECUTABLE_NAME
elif platform.system() == "Windows":
    compiler = GNU_COMPILER
    includes = DEFAULT_INCLUDES
    libraries = "-l mingw32 -l SDL2main -l SDL2"
    options = "-static-libgcc -static-libstdc++ -mwindows"
    executable = EXECUTABLE_NAME + ".exe"

compileString = compiler + " " + CPP_STANDARD + ((" " + options) if options else "") + (
    " -g -D DEV" if debug else "") + " -o " + executable + " " + includes + " " + SOURCE + ((" " + frameworks) if frameworks else "") + ((" " + libraries) if libraries else "")

print("============v (Command running on terminal) v===========================")
print("Compilng on: " + platform.system())
print(compileString)
print("========================================================================")

os.system(compileString)
