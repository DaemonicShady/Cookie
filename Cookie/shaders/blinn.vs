varying vec3 l, n;
varying vec3 v, h;

uniform vec4 lightPos;
uniform vec4 eyePos;

void main ( void )
{
	vec3 p = vec3 ( gl_ModelViewMatrix * gl_Vertex );

	l = normalize ( vec3 ( lightPos ) - p );
	n = normalize ( gl_NormalMatrix * gl_Normal );
	v = normalize ( vec3 ( eyePos ) - p );
	h = normalize ( l + v );
	gl_FrontColor = gl_Color;

	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}