#version 330 core

in vec3 FragPos;     // Fragment position in world coordinates
in vec3 Normal;      // Normal vector at the fragment

out vec4 FragColor;

uniform vec3 lightDirectionFront; // Direction of the front directional light
uniform vec3 lightDirectionBack;  // Direction of the back directional light
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;

void main()
{
    // Calculate ambient lighting for both light sources
    vec3 ambientFront = ambientStrength * lightColor;
    vec3 ambientBack = ambientStrength * lightColor;

    // Calculate the direction from the fragment to both light sources
    vec3 lightDirFront = normalize(-lightDirectionFront); // Negative because it's a light direction
    vec3 lightDirBack = normalize(-lightDirectionBack);

    // Calculate diffuse lighting for both light sources
    float diffFront = max(dot(Normal, lightDirFront), 0.0);
    vec3 diffuseFront = diffFront * lightColor;

    float diffBack = max(dot(Normal, lightDirBack), 0.0);
    vec3 diffuseBack = diffBack * lightColor;

    // Calculate the view direction (camera direction)
    vec3 viewDir = normalize(-FragPos);

    // Calculate specular lighting using the Phong reflection model for both light sources
    vec3 reflectDirFront = reflect(-lightDirFront, Normal);
    float specFront = pow(max(dot(viewDir, reflectDirFront), 0.0), shininess);
    vec3 specularFront = specularStrength * specFront * lightColor;

    vec3 reflectDirBack = reflect(-lightDirBack, Normal);
    float specBack = pow(max(dot(viewDir, reflectDirBack), 0.0), shininess);
    vec3 specularBack = specularStrength * specBack * lightColor;

    // Combine lighting components from both light sources
    vec3 result = (ambientFront + diffuseFront + specularFront) * objectColor + (ambientBack + diffuseBack + specularBack) * objectColor;

    // Output the final fragment color
    FragColor = vec4(result, 1.0);
}