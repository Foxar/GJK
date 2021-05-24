#version 330 core
out vec4 FragColor;
uniform vec4 fragmentColor;
in vec3 ourColor;

void main()
{
    FragColor = fragmentColor;
    //FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}