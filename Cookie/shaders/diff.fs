varying vec3 l, n;

void main ( void )
{
	const vec4 color = vec4 ( 0.5, 0.0, 0.0, 1.0 );
	const float ka = 0.2;
	const float kd = 1.0;

	vec3 n2 = normalize ( n );
	vec3 l2 = normalize ( l );

	vec4 diff = color * ( ka + kd * max ( dot (n2, l2), 0.0 ) );
	gl_FragColor = diff;
}