uniform sampler2D cloudTex;

uniform float time;
varying vec3 vertexNormal;
varying vec3 lightDir;


void main()
{
    float NdotL  = max(dot(normalize(vertexNormal), normalize(lightDir)), 0.0);
    
    vec2 cloudUV = gl_TexCoord[0].st;
    cloudUV.x = cloudUV.x - time * 1.0 / 60.0;
    //cloudUV.y = cloudUV.y - time * 1.0 / 10.0;
    vec3 clouds = texture2D(cloudTex, cloudUV).rgb;

    gl_FragColor.rgb = clouds * NdotL;
    gl_FragColor.a = max(clouds.z,max(clouds.x,clouds.y));
}

