#version 330 core
#define PHONGSHADING 999
#define WIREFRAME 990
#define TEXTURED 900
#define REFLECTION 909
struct Material
{
	sampler2D textureMap;
	vec3 diffuseColor;
	vec3 specularColor;
	vec3 ambientColor;
	float alpha;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};

// We set this field's properties from the C++ code
uniform Material material;
uniform bool DrawLight;
uniform vec3 eye;                     
uniform vec3 AmbientColor[10];       
uniform vec3 DiffuseColor[10];
uniform vec3 SpecularColor[10];
uniform vec3 lightsPositions[10];
uniform mat4 lightTransformations[10];
uniform vec3 lightsDirections[10];
uniform float lightsTypes[10];
uniform int LightsNumber;
uniform int ColorMethod;
uniform samplerCube skybox;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 lightPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;

// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	vec3 I,Ia=vec3(0.f,0.f,0.f),Id=vec3(0.f,0.f,0.f),Is=vec3(0.f,0.f,0.f);

	if(DrawLight==true)
		frag_color = vec4(1.f,1.f,1.f,1.f);
	else
	{
		if(ColorMethod==PHONGSHADING)	
		{
			for(int i=0;i<LightsNumber;i++)
			{
				vec3 lightpos=vec3(lightTransformations[i]* vec4(lightsPositions[i],1.f));
				if(lightsTypes[i]==0)
					I=normalize(fragPos-lightpos[i]);
				else
					I=lightsDirections[i];
				vec3 Acolor=AmbientColor[i],Dcolor=DiffuseColor[i],Scolor=SpecularColor[i];
				Ia+=vec3(Acolor.x*material.ambientColor.x,Acolor.y*material.ambientColor.y,Acolor.z*material.ambientColor.z);
				
				vec3 temp=vec3(Dcolor.x * material.diffuseColor.x, Dcolor.y * material.diffuseColor.y, Dcolor.z * material.diffuseColor.z);
				float IdotN = dot(-(fragNormal), I);
				Id+=temp * IdotN;

				float alpha = material.alpha;
				temp = vec3(Scolor.x * material.specularColor.x, Scolor.y * material.specularColor.y, Scolor.z * material.specularColor.z);
				vec3 r = (2.f * dot(-fragNormal, I) * fragNormal - I);
				float Power = pow(max(0.0f, dot((r), (eye))), alpha);
				Is+=(temp * Power);
			}
			frag_color = vec4((Is+Id+Ia),1.f);
		}
		if(ColorMethod==WIREFRAME)
		{
			frag_color=vec4(material.ambientColor,1.f);
		}
		if(ColorMethod==TEXTURED)
			frag_color=vec4(textureColor,1.f);
		if(ColorMethod==REFLECTION)
		{
		    vec3 I = normalize(fragPos - eye);
		    vec3 R = reflect(I,normalize(fragNormal));
		    frag_color = vec4(0.2,0.2,0.2,0)+vec4(texture(skybox, R).rgb, 1.0);
		}
	}	
}
