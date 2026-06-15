#version 460 core
uniform mat4 projection;
uniform mat4 view;

layout (location = 0) in vec3 aPos;

out vec3 ViewPos;
out vec3 TexCoords;

void main() {
    TexCoords = aPos;
    ViewPos = vec3(view * vec4(aPos, 1.0));
    gl_Position = (projection * view * vec4(aPos, 1.0)).xyww;
}  