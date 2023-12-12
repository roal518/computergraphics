#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D outTexture; 

uniform vec3 lightPos;
uniform vec3 lightColor; 
uniform vec4 objectColor;
uniform vec3 viewPos;
uniform float lightPower;

void main()
{	
	/*앰비언트 구하는 공식*/
	float ambientLight = 0.5f; //색이 진해진다.
	vec3 ambient = ambientLight * lightColor; //주변 조명 값, 앰비언트에서 주변 조명 값은 1.0f으로 적용된다. (게임수학에서 배울 때 이러한 이유 때문에 사용하지 않는다고 했었다)

	vec3 normalVector = normalize (Normal); //노말값을 정규화한다. 노말값들은 전부 정규화 되야 함. 그렇지 않으면 제대로 출력할 수 없다.
	vec3 lightDir = normalize(lightPos - FragPos); //표면과 조명의 위치로 조명의 방향을 결정한다. 광원 벡터 생각하자.
	float diffuseLight = max(dot (normalVector, lightDir), 0.0f);//노멀벡터와 광원 벡터의 내적(음수가 나올 수 없도록 최소값 0.0으로 제한)
	vec3 diffuse = lightPower * diffuseLight * lightColor; //산란 반사 조명 값 = 산란 반사 값 * 조명 색상 값	

	int shininess = 5; //광택 계수
	float specularStrength = 1.0f;//광택 세기
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect (-lightDir, normalVector);	
	float specularLight =pow( max (dot (viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = specularStrength * specularLight * lightColor;

	//vec3 result = (ambient + diffuse + specular);//최종 색상 = (주변 조명 + 산란 반사 조명 + 거울 반사 조명) * 객체 색상 
    vec4 result = vec4(ambient + diffuse + specular, 1.0f) * objectColor;//최종 색상 = (주변 조명 + 산란 반사 조명 + 거울 반사 조명) * 객체 색상 
	FragColor = texture(outTexture, TexCoord);
	FragColor = vec4 (result) * FragColor;
}
