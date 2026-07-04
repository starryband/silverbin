#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 cam_matrix;
uniform mat4 model;
uniform float outlining;

void main() 
{
    // 1. Transform the position and normal into World Space first
    vec3 worldPos = vec3(model * vec4(aPos, 1.0f));
    
    // We only want the direction of the normal, stripped of translation
    vec3 worldNormal = normalize(mat3(transpose(inverse(model))) * aNormal); 
    
    // 2. Extrude the vertex along its normal in world space
    vec3 crntPos = worldPos + worldNormal * outlining;
    
    // 3. Project to screen space
    gl_Position = cam_matrix * vec4(crntPos, 1.0f);
}