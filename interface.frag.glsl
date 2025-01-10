 #version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D texture1;
uniform bool selected;

void main()
{
    if (selected)
    {
        //FragColor = vec4(1.0, 0.0, 0.0, 0.0);
        FragColor = texture(texture1, TexCoords) * vec4(1.0, 0.0, 0.0, 0.0);
    }
    else {
        FragColor = texture(texture1, TexCoords);
    }
}
