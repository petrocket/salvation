uniform sampler2D dayTex;
uniform sampler2D nightTex;

uniform float time;

varying vec3 vertexNormal;
varying vec3 lightDir;


void main()
{
    float NdotL  = max(dot(normalize(vertexNormal), normalize(lightDir)), 0.0);
    
    vec2 uv = gl_TexCoord[0].st;
    uv.x = uv.x - time * 1.0 / 70.0;
    vec3 day = texture2D(dayTex, uv).rgb;
    vec3 night = texture2D(nightTex, uv).rgb;

    gl_FragColor.rgb = (day * NdotL) + (night * (1.0 - NdotL));
    gl_FragColor.a = 1.0;
}

