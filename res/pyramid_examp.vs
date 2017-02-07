#version 430 core
 
layout (location = 0) in vec3 position;
 
// per-instance attribute
layout (location = 1) in vec3 color;
 
// per-instance transformation
layout (location = 2) in mat4 trans;
 
uniform mat4 projection;
 
out vec4 col;
 
void main() {
    gl_Position = projection * trans * vec4(position, 1.0f);
    col = vec4(color, 1.0f);
}