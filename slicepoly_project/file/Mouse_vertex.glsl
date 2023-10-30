# version 330 core

layout (location = 2) in vec3 in_Mouse_Position;

uniform mat4 modelMatrix;
void main (void){

	gl_Position =  modelMatrix * vec4(in_Mouse_Position,1.0);
}