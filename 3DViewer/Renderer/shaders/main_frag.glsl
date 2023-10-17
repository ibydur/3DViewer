#version 330 core

in vec3 FragPos;     // Fragment position in world coordinates
in vec3 Normal;      // Normal vector at the fragment

out vec4 FragColor;

uniform vec3 lightDirection; // Direction of the directional light
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;

void main()
{
    // Calculate ambient lighting
    vec3 ambient = ambientStrength * lightColor;

    // Calculate the direction from the fragment to the light source
    vec3 lightDir = normalize(-lightDirection); // Negative because it's a light direction

    // Calculate diffuse lighting
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Calculate the view direction (camera direction)
    vec3 viewDir = normalize(-FragPos);

    // Calculate the reflection direction (used for specular lighting)
    vec3 reflectDir = reflect(-lightDir, Normal);

    // Calculate specular lighting using the Phong reflection model
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine all lighting components
    vec3 result = (ambient + diffuse + specular) * objectColor;

    // Output the final fragment color
    FragColor = vec4(result, 1.0);
}