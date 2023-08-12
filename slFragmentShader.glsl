#version 330 core

in vec3 norm;
in vec3 fragPos;

out vec4 sunColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Ambient lighting
    float strength = 0.5f;
    vec3 ambient = strength * lightColor;

    // Diffuse lighting
    vec3 normz = normalize(norm);
    vec3 lightDirection = normalize(lightPos - fragPos);

    float diff = max(dot(normz, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specIntensity = 0.99f;
    float highlight = 8.0f;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normz);

    // Specular component
    float specComponent = pow(max(dot(viewDir, reflectDirection), 0.0), highlight);
    vec3 specular = specIntensity * specComponent * lightColor; // Fix the typo here

    // Calculate (Phong lighting) result
    vec3 brightLightColor = lightColor * 2.0;

    vec3 phong = (ambient + diffuse + specular) * objectColor * brightLightColor;
    sunColor = vec4(phong, 1.0f);
}
