#include <iostream>
#include "src/Filesystem.h"
#include "src/Scripts.h"

int main() {
    l6::Filesystem fs("/home/coltenus/vsc");
    fs.Print();
    std::cout << fs.GetByName("test1.py")->GetFullPath() << "\n\n";
    fs.Open("./Python", true);
    fs.Print();
    std::cout << fs.GetByPath("./test1/")->GetFullPath() << "\n\n";
    fs.Open("/home/coltenus/vsc/.vscode");
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
    std::cout << fs.GetRoot()->GetFullPath() << "\n\n";
    l6::Scripts* sc = new l6::Scripts;
    sc->Refresh();
    sc->PrintNames();
    sc->ChangeRows(0, "scripts/text.txt", l6::NewFile);
    sc->ChangeRows(0, "scripts/text.txt", l6::Terminal);
    {
        std::string bufStr;
        sc->ChangeRows(0, "scripts/text.txt", l6::ReturnData, &bufStr);
        printf("\n\n%s\n", bufStr.c_str());
    }
    delete sc;
    return 0;
}

