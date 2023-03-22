#include <iostream>
#include "src/Filesystem.h"

int main() {
    l6::Filesystem fs("/home/coltenus/vcs");
    fs.Print();
    fs.Open("./Python", true);
    fs.Print();
    fs.Open("/home/coltenus/vcs/.vscode");
    fs.Print();
    fs.GoUpper();
    fs.CreateFolder("test");
    fs.Open("./test", true);
    fs.CreateFolder("test2");
    fs.GoUpper();
    fs.Print();
    fs.Rename("test", "test1");
    fs.CreateFolder("test", true);
    fs.Copy("test1", fs.GetCurrentPath(), "test3");
    fs.Print();
    fs.Replace("test", fs.GetCurrentPath() + "/test1");
    fs.Print();
    fs.Remove("test1");
    fs.Remove("test3");
    fs.Print();
    return 0;
}

