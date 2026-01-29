attribute vec4 fontCoords;
 
uniform vec4 color;
 
varying vec4 fragColor;
varying vec2 fontTexCoords;

void main()
{
    fragColor = color;
    fontTexCoords = fontCoords.zw;
    gl_Position = vec4(fontCoords.xy, 0.0, 1.0);
}