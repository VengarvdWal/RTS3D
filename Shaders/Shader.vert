#version 300 es                          			
precision mediump float;		  			

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;

out vec4 vCol;
out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
	gl_Position = projection * view * model * vec4(vPos, 1.0);
	vCol = vec4(clamp(vPos, 0.0, 1.0), 1.0);

	texCoord = tex;

	normal = mat3(transpose(inverse(model))) * norm;

	fragPos = (model * vec4(vPos, 1.0)).xyz;
}