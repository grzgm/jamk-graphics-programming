#include "QuadNode.h"

void QuadNode::Setup(IRenderer &renderer, GLuint program, GLuint texture, float *vertexData, int32_t vertexStride)
{
    glUseProgram(program);

    GLint postition = glGetAttribLocation(program, "position");
    GLint uv = glGetAttribLocation(program, "uv");

    glEnableVertexAttribArray(postition);
    glEnableVertexAttribArray(uv);

    glVertexAttribPointer(postition, 3, GL_FLOAT, GL_FALSE, vertexStride, vertexData);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, vertexStride, vertexData + 3);

    renderer.SetTexture(program, texture, 0, "texture01");
}

void QuadNode::Render(IRenderer &renderer, GLuint program)
{
    const glm::mat4 mvp = renderer.GetProjectionMatrix() * renderer.GetViewMatrix() * GetWorldMatrix();

    OpenGLRenderer::SetUniformMatrix4(program, "mvpMatrix", mvp);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    // render all child nodes
    Node::Render(renderer, program);
}
