#include "Shader.h"

#include <vector>

#include <GL/gl.h>

class ProgramObject
{
public:
    ProgramObject(std::vector<Shader> &shaders);
    void addShader(Shader &shader);
private:
    void link();
    
    GLuint programHandle;
};
