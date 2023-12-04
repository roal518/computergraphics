#version 330 core
in vec3 out_lightPosition;
in vec3 out_Color;
in vec3 out_Normal;
out vec4 FragColor;
uniform int light_Mode;
void main(void){
	vec3 result = out_Color;
	float ambientLight_str = 0.4;
	vec3 ambient = ambientLight_str *vec3(1.0,0.9,0.8);
	vec3 normalVec = normalize(out_Normal);
	vec3 lightVec = normalize(out_lightPosition - out_Normal);
	float brightness = max(dot(lightVec,normalVec),0.0);
	float diffuse = brightness * 3.0f;
	
	if (light_Mode == 0){
		result = (diffuse+ambient)*out_Color ; 
	}
	FragColor = vec4(result, 1.0);
} 