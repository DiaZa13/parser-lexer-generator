
#include "Mermaid.h"
#include <fstream>
#include <>


Mermaid::Mermaid(){
    ofstream file("../graph.html", ios::app);
    if (file){
        file<<"";
    }
};