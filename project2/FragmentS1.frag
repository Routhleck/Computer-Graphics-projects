#version 330  
out vec4 FragColor; 

in vec2 Tcoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 PosRelativeToCam;

uniform sampler2D texture1;
vec3 lightPos = vec3(-4.0, 2.0, 0.2);
vec3 lightpos_2 = vec3 (0.7, 0.63, 0.8);
vec3 lightpos_3 = vec3 (-1.1, 0.63, 0.27);
vec3 lightpos_4 = vec3 (0.22f, 0.06f, 0.62f);

uniform vec3 viewPos; 
uniform samplerCube skybox;

uniform vec3 Kd;
uniform vec3 Kd2;
uniform vec3 Kd3;
uniform vec3 Ld;
uniform vec3 Ld2;
uniform vec3 Ld3;
uniform vec3 Ld4;
uniform bool ambientLight_enable;
uniform float ambientStrength;
uniform bool diffuseLight_enable;
uniform bool specularLight_enable;



uniform bool fog_display;
uniform float fog_maxdist;
uniform float fog_mindist;
uniform vec4 fog_colour;



void main()                                                               
{               

//AMBIENT
vec3 ambient = ambientStrength * Kd ;


//DIFFUSE
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * Ld * Kd;

//SPECULAR
float specularStrength = 0.3;
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * Ld * Kd;

//DIFFUSE_2
lightDir = normalize(lightpos_2 - FragPos);
diff = max(dot(norm, lightDir), 0.0);
diffuse = diffuse + (diff * Ld2 * Kd2);

//SPECULAR 2
reflectDir = reflect(-lightDir, norm);
spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
specular += specularStrength * spec * Ld2 * Kd2;

//DIFFUSE_3
lightDir = normalize(lightpos_3 - FragPos);
diff = max(dot(norm, lightDir), 0.0);
diffuse = diffuse + (diff * Ld3 * Kd2);

//SPECULAR_3
reflectDir = reflect(-lightDir, norm);
spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
specular += specularStrength * spec * Ld3 * Kd2;

//DIFFUSE_4
lightDir = normalize(lightpos_4 - FragPos);
diff = max(dot(norm, lightDir), 0.0);
diffuse = diffuse + (diff * Ld4 * Kd3);

//SPECULAR_4
reflectDir = reflect(-lightDir, norm);
spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
specular += specularStrength * spec * Ld4 * Kd3;

if (ambientLight_enable == false) ambient = vec3(0.0f);
if (diffuseLight_enable == false) diffuse = vec3(0.0f);
if (specularLight_enable == false) specular = vec3(0.0f);

vec3 final = diffuse + ambient + specular;

FragColor = vec4(final, 1.0f) *  texture(texture1, Tcoord);	

//FOG FACTOR CALCULATION
float dist = length(PosRelativeToCam.xyz);
float fog_factor = (fog_maxdist - dist) / (fog_maxdist - fog_mindist);
fog_factor = clamp(fog_factor, 0.0, 1.0);

if (fog_display) FragColor = mix(fog_colour,FragColor,fog_factor);

}


