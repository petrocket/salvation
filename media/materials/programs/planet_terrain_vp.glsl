uniform vec4 lightPos; 
uniform mat4 worldMatrix;
uniform mat4 worldViewProjMatrix;

varying vec3 lightDir;
varying vec3 vertexNormal;

void main()
{
	vertexNormal = (worldMatrix * vec4(gl_Normal,0.0)).xyz;

    lightDir = normalize(lightPos.xyz - (gl_Vertex * lightPos.w).xyz);
        
    // pass the vertex position through unchanged
    gl_Position = worldViewProjMatrix * gl_Vertex;
    
    // pass UV co-ords through unchanged
    gl_TexCoord[0] = gl_MultiTexCoord0;
}

