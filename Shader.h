#include <fstream>
#include <string>

class ProgramObject;

class Shader
{
public:
    Shader(std::string filename);
    Shader(std::ifstream &file);
    virtual ~Shader()=0;
private:
    void loadFile(std::ifstream &file);
    virtual void loadShader()=0;
    virtual void attachTo(ProgramObject &obj)=0;

    std::string shaderString;

    friend class ProgramObject;
};
