#version 430

layout (location = 0) in vec3 position;
layout (location = 1) uniform float time;
layout (location = 2) uniform mat4 cameraMat;
layout (location = 3) in vec2 uv;
layout (location = 4) in vec3 normal;
// 5 is camera
layout (location = 6) uniform mat4 modelWorldTransform;

out vec3 color;
out vec3 norm;
out vec3 pos;
out vec2 fuv;


void main()
{
	// gl_Position = vec4(position, 1);
	gl_Position = cameraMat * modelWorldTransform * vec4(position, 1);

	color = vec3(.5, .5, .5);

	norm = transpose(inverse(mat3(modelWorldTransform))) * normal; // Mulpily by modelWorldTransform
	pos = position * mat3(modelWorldTransform); // Mulpily by modelWorldTransform
	fuv = uv;
}