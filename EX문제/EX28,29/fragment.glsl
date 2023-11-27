#version 330 core

in vec3 out_lightvec;
in vec3 out_Color;
in vec3 out_Normal;
in vec3 out_viewDir;

out vec4 FragColor;

uniform int light_Mode;
uniform vec3 light_Color;
uniform float light_Str;
void main(void){
	//ambient
	float ambientLight_str = 0.3;
	vec3 ambient = ambientLight_str * light_Color;
	//diffuse
	vec3 normalVec = normalize(out_Normal);
	float brightness = max(dot(out_lightvec,normalVec),0.0);
	vec3 diffuse = light_Str * brightness * light_Color ;
	//specular
	int shininess = 64;
	vec3 viewDir = normalize(out_viewDir-out_Normal);
	vec3 reflectDir = reflect(-viewDir,normalVec);
	float specularLight = max(dot(viewDir,reflectDir),0.0);
	specularLight = pow(specularLight,shininess);
	vec3 specular =  light_Str * specularLight * light_Color;

	vec3 result = ambient * out_Color;
	if (light_Mode == 0){
		result = (diffuse + ambient + specular) * out_Color;  
	}
	FragColor = vec4(result, 1.0);
}
