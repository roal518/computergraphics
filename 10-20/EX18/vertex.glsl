# version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
out vec3 out_Color;
out vec3 world_Position;
uniform mat4 modelMatrix;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main (void){
   //projectionTransform *viewTransform *
	gl_Position =projectionTransform* viewTransform * modelMatrix * vec4(in_Position,1.0);
	out_Color = in_Color;
}