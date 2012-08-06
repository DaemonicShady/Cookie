uniform vec3 eyePos;
uniform vec3 up;
uniform vec3 right;

void main (void)
{
    vec4 vertex = gl_Vertex + vec4 ( (gl_MultiTexCoord0.x - 0.5) * right +
                                     (gl_MultiTexCoord0.y - 0.5) * up, 0.0 );

    gl_TexCoord [0] = gl_MultiTexCoord0;
    gl_Position     = gl_ModelViewProjectionMatrix * vertex;

    float d     = distance ( gl_Position.xyz, eyePos ) / 50.0;
    float alphaScale = 0.9 / ( 1.0 + d ) + 0.1;
    gl_FrontColor.a  = 1.0 * alphaScale;
}