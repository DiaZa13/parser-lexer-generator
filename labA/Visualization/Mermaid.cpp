
#include "Mermaid.h"
#include <fstream>
#include <iostream>
#include <windows.h>

Mermaid::Mermaid(std::string flow, std::string graph){
    std::ofstream file("../graph.html", std::ios::out);

    if (file){
        file<<"<!DOCTYPE html>\n"
              "<html lang=\"en\"><head><meta charset=\"utf-8\" /></head>\n"
              "  <body>\n"
              "    <pre class=\"mermaid\">\n";
        file<<"\t " + flow +"\n" + "\t\t" + graph;
        file<<"    </pre>\n"
              "    <script src=\"https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js\"></script>"
              "    <script> mermaid.initialize({startOnLoad:true});</script>"
              "  </body>\n"
              "</html>";
    }else
        throw std::invalid_argument("AddPositiveIntegers arguments must be positive");

    file.close();
}

void Mermaid::showGraph() {
        ShellExecute(NULL, "open", "../graph.html",NULL, NULL, SW_SHOWNORMAL);
}