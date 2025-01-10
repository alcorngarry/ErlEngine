#version 330 core

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{
    float ambientStrength = 0.2;
    float specularStrength = 1.0;
    vec3 ambient = ambientStrength * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), specularStrength), 256);

    vec4 result = vec4(ambient + diffuse + spec, 1.0) * texture(texture_diffuse1, texCoord) * vec4(texture(skybox, reflectDir).rgb, 1.0);

    FragColor = result;
}