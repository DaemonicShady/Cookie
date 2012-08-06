uniform sampler2D grassMap;

void main (void)
{
    vec4 grassColor = texture2D ( grassMap, gl_TexCoord [0].st );

    if ( gl_Color.a < 0.5 )
        discard;

    gl_FragColor = grassColor;
}