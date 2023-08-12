#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aColor;
layout(location = 3) in vec3 aNormal;  // Assuming you have the normal attribute

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 yellowSpherePos;

out vec3 FragPos;
out vec3 Normal;
out vec3 ourColor;
out vec2 texCoord;
out vec3 ViewDir;

void main()
{
    // Apply the model matrix to the vertex position
    vec4 worldPos = model * vec4(aPos, 1.0);

    // Translate the position of the vertex by the position of the yellow sphere
    worldPos.xyz += yellowSpherePos;

    // Calculate the final position of the vertex in view space
    gl_Position = projection * view * worldPos;

    FragPos = worldPos.xyz;
    ourColor = aColor;
    texCoord = aTexCoord;

    // Transform normal and view direction to world space
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalize(-normalMatrix * aNormal);

    // Calculate the view direction in world space
    ViewDir = normalize(vec3(view * vec4(1.0, 1.0, 0.0, 1.0)) - worldPos.xyz);
}

