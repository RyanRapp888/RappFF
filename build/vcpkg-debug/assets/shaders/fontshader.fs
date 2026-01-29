#version 140

varying vec4 fragColor;
varying vec2 fontTexCoords;
 
uniform sampler2D tex;
 
void main() 
{
   gl_FragColor = vec4(1.0, 1.0, 1.0, texture2D(tex, fontTexCoords).a) * fragColor;
}