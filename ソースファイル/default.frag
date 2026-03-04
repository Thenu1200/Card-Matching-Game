#version 330 core

out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 currentPos;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;




uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform vec4 lightColor;

uniform vec3 lightPos;

uniform vec3 cameraPos;

vec4 pointLight()
{
   vec3 lightVec = lightPos - currentPos;
   float dist = length(lightVec);
   float a = 1.0;
   float b = 0.04;
   float inten = 5.0f / (a * dist * dist + b * dist + 1.0f);

   float ambient = 0.20f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightVec);
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   float specularLight = 0.50f;
   vec3 viewDirection = normalize(cameraPos - currentPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
   float specular = specularAmount * specularLight;

   return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

vec4 directLight()
{
   float ambient = 0.20f;

   vec3 normal = normalize(Normal);
   // Should come from the opposite direction you want the light to come from
   vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   float specularLight = 0.50f;
   vec3 viewDirection = normalize(cameraPos - currentPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
   float specular = specularAmount * specularLight;

   return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
   float outerCone = 0.90f;
   float innerCone = 0.95f;

   float ambient = 0.20f;

   vec3 normal = normalize(Normal);
   vec3 lightDirection = normalize(lightPos - currentPos);
   float diffuse = max(dot(normal, lightDirection), 0.0f);

   float specularLight = 0.50f;
   vec3 viewDirection = normalize(cameraPos - currentPos);
   vec3 reflectionDirection = reflect(-lightDirection, normal);
   float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
   float specular = specularAmount * specularLight;

   float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
   float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

   return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main()
{
   FragColor = pointLight();
}