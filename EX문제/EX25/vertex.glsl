# version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in vec3 in_Normal;
out vec3 out_Color;
out vec3 out_Normal;
out vec3 world_Position;
out vec3 out_lightPosition;
uniform mat4 modelMatrix;
uniform vec3 lightPosition;

void main (void){
	
	world_Position = vec3(modelMatrix*vec4(in_Position,1.0));
	gl_Position = vec4(world_Position,1.0);
	out_Color = in_Color;
	out_Normal = vec3(modelMatrix*vec4(in_Normal,1.0));
	out_lightPosition = lightPosition; 
}