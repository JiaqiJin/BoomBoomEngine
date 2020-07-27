#version 330 core
in vec3 FragPos;
in vec2 Texcoord;
in vec3 Normal;
in vec3 Color;
in vec4 FragPosLightSpace;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform bool receiveShadow;
uniform vec3 cameraPos;
uniform DirLight dirLight;
uniform sampler2D image;

out vec4 fragColor;

void main(){
  	// diffuse texture.
	vec3 color = texture(image,Texcoord).rgb;
    // ambient
    vec3 ambient = dirLight.ambient * color;
    // diffuse
    vec3 lightDir = dirLight.direction;
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * dirLight.diffuse * color;
    // specular
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);

    vec3 specular = dirLight.specular * color * spec;

    fragColor = vec4(ambient + diffuse + specular, 1.0f);
    //fragColor = clamp(fragColor, vec4(0.1), vec4(1));
	// gamma correction.
	const float gamma = 2.2f;
	fragColor.rgb = pow(fragColor.rgb,vec3(1.0f/gamma));
}