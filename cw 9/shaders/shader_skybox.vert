#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 transformation;

void main()
{
    vec4 pos = transformation * vec4(aPos, 1.0f);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    TexCoord = vec3(aPos.x, aPos.y, -aPos.z);
}
