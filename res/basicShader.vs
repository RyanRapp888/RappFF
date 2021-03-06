#version 430 core

attribute vec3 position;
attribute vec2 texCoord;
attribute vec3 normal;
attribute mat4 trans;

varying vec2 texCoord0;
varying vec3 normal0;

uniform mat4 MVP;
uniform mat4 Normal;

void main()
{
	gl_Position = MVP * trans * vec4(position, 1.0);
	texCoord0 = texCoord;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
}