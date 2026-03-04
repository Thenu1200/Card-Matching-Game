#version 330 core

// Position/coordinates
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Textures
layout (location = 3) in vec2 aTex;


// Outputs the current position for the Fragment Shader
out vec3 currentPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;

// The Camera Matrix
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

uniform mat4 transform;

void main()
{
   // Calculates current position
   currentPos = vec3(transform * model * translation * -rotation * scale * vec4(aPos, 1.0f));
   // Assigns the normal from the Vertex Data to "Normal" 
   mat3 normalMatrix = transpose(inverse(mat3(transform * model * translation * -rotation * scale)));
   Normal = normalize(normalMatrix * aNormal);
   // Assigns the colors from the Vertex Data to "color"
   color = aColor;
   // Assigns the texture coordinates from the Vertex Data to "texCoord"
   texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
   
   // Outputs the positions/coordinates of all vertices
   gl_Position = camMatrix * vec4(currentPos, 1.0);
   
}