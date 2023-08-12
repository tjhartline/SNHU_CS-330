#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;
in vec3 ViewDir;

out vec4 FragColor;

uniform sampler2D sphereTexture;
uniform sampler2D coneTexture;
uniform sampler2D planeTexture;
uniform vec3 lightPos; // Position of the light source
uniform int objectType;

uniform vec3 updatedViewDir; // New view direction from keyboard input
uniform vec3 updatedReflectionDir; // New reflection direction from keyboard input

void main()
{
    vec4 sphereTex = texture(sphereTexture, texCoord);
    vec4 coneTex = texture(coneTexture, texCoord);
    vec4 planeTex = texture(planeTexture, texCoord);

    // Calculate normalized normal and light direction
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Calculate shadow factor (0.0 for surfaces in shadow, 1.0 for surfaces in light)
    float shadowFactor = dot(norm, lightDir);
    shadowFactor = clamp(shadowFactor, 0.0, 1.0);

    vec4 finalColor;

    if (objectType == 0) { // Red sphere
        finalColor = sphereTex * vec4(1.0, 0.0, 0.0, 1.0) * (0.3 + 0.7 * shadowFactor);
    }
    else if (objectType == 1) { // Cone
        finalColor = coneTex * (0.3 + 0.7 * shadowFactor);
    }
    else if (objectType == 2) { // Plane
        vec2 reflectionTexCoord = vec2(0.5, 0.5) + updatedReflectionDir.xy * 0.5; // Adjust as needed
        vec4 reflectedColor = texture(planeTexture, reflectionTexCoord);

        float reflectionFactor = max(dot(updatedReflectionDir, updatedViewDir), 0.0);
        finalColor = mix(planeTex, reflectedColor, reflectionFactor) * (0.3 + 0.7 * shadowFactor);
    }
    else if (objectType == 3) { // Yellow sphere
        vec2 reflectionTexCoord = vec2(0.5, 0.5) + updatedReflectionDir.xy * 0.5; // Adjust as needed
        vec4 reflectedColor = texture(planeTexture, reflectionTexCoord);

        float reflectionFactor = max(dot(updatedReflectionDir, updatedViewDir), 0.0);
        finalColor = mix(sphereTex, reflectedColor, reflectionFactor) * (0.3 + 0.7 + 0.6 * shadowFactor);
    }

    FragColor = finalColor;
}