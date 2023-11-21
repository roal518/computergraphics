# version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Color;
layout (location = 2) in vec3 in_Normal;

out vec3 out_Normal;
out vec3 out_lightvec;
out vec3 out_Color;
out vec3 world_Position;
out vec3 out_viewDir;
uniform mat4 modelMatrix;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;
uniform mat4 lightMatrix;
uniform vec3 lightPosition;
uniform vec3 viewDir;

void main (void){
	vec3 result = vec3(modelMatrix * vec4(in_Position, 1.0));
	gl_Position = projectionTransform * viewTransform * vec4(result, 1.0);
	out_Color = in_Color;
	out_Normal = vec3(modelMatrix * vec4(in_Position, 1.0));
	out_lightvec = vec3(lightMatrix * vec4(lightPosition, 1.0));
	vec3 out_viewDir = vec3(viewTransform *  vec4(viewDir,1.0));

}