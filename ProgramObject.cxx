#include "ProgramObject.h"

#include <GL/gl.h>
#include <GL/glext.h>

using namespace std;

ProgramObject::ProgramObject(vector<Shader> &shaders)
{
    //programHandle = glCreateProgramObjectARB();
    for(vector<Shader>::iterator shader = shaders.begin(); shader!=shaders.end(); shader++)
    {
        shader->attachTo(*this);
    }
    link();
}

void ProgramObject::addShader(Shader &shader)
{
    shader.attachTo(*this);
    link();
}

void ProgramObject::link()
{
    //glLinkProgram(programHandle);
}
