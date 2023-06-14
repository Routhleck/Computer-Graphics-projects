#version 330                                                                
																			
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 TexCoord; 

out vec3 FragPos;
out vec3 Normal;
out vec2 Tcoord;
out vec4 PosRelativeToCam;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()                                                                     
{                                                                                

	PosRelativeToCam = view * model * vec4(vPosition, 1.0); //view matrix * world position
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	Tcoord = TexCoord;
	FragPos = vec3(model * vec4(vPosition, 1.0));
	Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
}
