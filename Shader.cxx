#include "Shader.h"

using namespace std;

Shader::Shader(string filename)
{
    ifstream file(filename, ios::in | ios::binary);
    loadFile(file);
}

Shader::Shader(ifstream &file)
{
    loadFile(file);
}

void Shader::loadFile(ifstream &file)
{
    shaderString.assign((istreambuf_iterator<char>(file)),
                        istreambuf_iterator<char>());
}
