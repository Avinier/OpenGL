#pragma once
#include <GL/glew.h>

#include"VertexArray.h"
#include"IndexBuffer.h"
#include"Shader.h"

//Error handling macro
#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

bool GLLogCall(const char* func, const char* file, int line);

class Renderer
{
public:
    void Clear();
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
