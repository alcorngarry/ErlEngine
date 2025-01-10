#version 330 core

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform bool selected;
uniform samplerCube skybox;

//for board spaces, will changes when creating new textures for them
uniform int objectId;

void main()
{
    if (selected)
    {
        FragColor = vec4(1.0, 0.0, 0.0, 0.0);
    }
    else {
        if (objectId == 11 || objectId == 0)
        {
            float ambientStrength = 0.2;
            float specularStrength = 0.5;
            vec3 ambient = ambientStrength * lightColor;

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

            FragColor = vec4(ambient + diffuse + spec, 1.0) * texture(texture_diffuse1, texCoord);
        }
        else if (objectId == 1)
        {
            float ambientStrength = 0.2;
            float specularStrength = 0.5;
            vec3 ambient = ambientStrength * lightColor;

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

            FragColor = vec4(ambient + diffuse + spec, 1.0) * texture(texture_diffuse1, texCoord) * vec4(0.0, 0.5, 0.0, 0.0);
        }
        else if (objectId == 2)
        {
            float ambientStrength = 0.2;
            float specularStrength = 0.5;
            vec3 ambient = ambientStrength * lightColor;

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

            FragColor = vec4(ambient + diffuse + spec, 1.0) * texture(texture_diffuse1, texCoord) * vec4(0.0, 1.0, 0.0, 0.0);
        }
        else if (objectId == 3)
        {
            float ambientStrength = 0.2;
            float specularStrength = 0.5;
            vec3 ambient = ambientStrength * lightColor;

            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            vec3 reflectDir = reflect(-lightDir, norm);

            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

            FragColor = vec4(ambient + diffuse + spec, 1.0) * texture(texture_diffuse1, texCoord) * vec4(1.0, 0.0, 0.0, 0.0);
        }
    }
}