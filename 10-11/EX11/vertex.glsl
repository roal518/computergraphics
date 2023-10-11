# version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
out vec3 out_Color;
in vec2 inControlPoints[75];
uniform int n;
vec2 calculateBezier(float t){
	vec2 position =vec2(0.0,0.0);
	for (int i = 0; i < 75; i++) {
		position += pow(1.0 - t,(75 - 1 - i)) * inControlPoints[i];
	}
	return position;
}
void main (void){

	gl_Position = vec4(in_Position,1.0);
	//gl_Position = vec4(in_Position,1.0f);	
	out_Color = in_Color;	
}