varying vec3 l, n;
uniform vec4 lightPos;

void main ( void )
{
	vec3 p = vec3 ( gl_ModelViewMatrix * gl_Vertex );
	l = normalize ( vec3 (lightPos) - p );
	n = normalize ( gl_NormalMatrix * gl_Normal );
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}