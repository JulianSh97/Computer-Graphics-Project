#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool DrawLight;
uniform bool HasVt;
uniform mat4 LightTransformation;
uniform int lightType;
uniform bool reflection;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;
	
	if(HasVt==true)
		fragTexCoords = texCoords;
	else
	{
		fragTexCoords=orig_fragPos.xy;
		//float r=sqrt(pow(orig_fragPos.x,2)+pow(orig_fragPos.y,2)+pow(orig_fragPos.z,2));
		//fragTexCoords.x =atan((sqrt(pow(orig_fragPos.x,2)+pow(orig_fragPos.y,2)))/orig_fragPos.z);			
		//fragTexCoords.y = atan(orig_fragPos.y/orig_fragPos.x);
	}
	if(reflection==true)
	{
		fragNormal = mat3(transpose(inverse(model))) * normal;
	}

	gl_PointSize=10.0f;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
	if(DrawLight==true)
	{
		if(lightType==1)
			gl_Position= projection * view * vec4(pos, 1.0f);
		else
			gl_Position= projection * view *  LightTransformation * vec4(pos, 1.0f);
	}
}