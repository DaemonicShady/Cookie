uniform sampler2D grassMap;

void main (void)
{
//    if ( gl_Color.a < 0.5 )
//        discard;

    vec4 grassColor = texture2D ( grassMap, gl_TexCoord [0].st );

    grassColor.a *= gl_Color.a;

    if ( grassColor.a < 0.5 )
        discard;

    gl_FragColor = grassColor;
}