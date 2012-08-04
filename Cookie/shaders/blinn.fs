varying vec3 l, n;
varying vec3 v, h;

void main ( void )
{
	const float ka = 0.2;
	const float kd = 1.0;
	const float ks = 1.0;
	const float specPower = 15.0;
	const vec4  specColor = vec4 ( 0.7, 0.7, 0.7, 1.0 );

	vec3 n2 = normalize ( n );
	vec3 l2 = normalize ( l );
	vec3 h2 = normalize ( h );

	vec4 diff = gl_Color * ( ka + kd * max ( dot ( l2, n2 ), 0.0 ) );
	vec4 spec = specColor * ks * pow ( max ( dot ( h2, n2 ), 0.0 ), specPower );

	gl_FragColor = diff + spec;
}