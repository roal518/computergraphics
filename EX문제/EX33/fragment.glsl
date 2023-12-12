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
	/*�ں��Ʈ ���ϴ� ����*/
	float ambientLight = 0.5f; //���� ��������.
	vec3 ambient = ambientLight * lightColor; //�ֺ� ���� ��, �ں��Ʈ���� �ֺ� ���� ���� 1.0f���� ����ȴ�. (���Ӽ��п��� ��� �� �̷��� ���� ������ ������� �ʴ´ٰ� �߾���)

	vec3 normalVector = normalize (Normal); //�븻���� ����ȭ�Ѵ�. �븻������ ���� ����ȭ �Ǿ� ��. �׷��� ������ ����� ����� �� ����.
	vec3 lightDir = normalize(lightPos - FragPos); //ǥ��� ������ ��ġ�� ������ ������ �����Ѵ�. ���� ���� ��������.
	float diffuseLight = max(dot (normalVector, lightDir), 0.0f);//��ֺ��Ϳ� ���� ������ ����(������ ���� �� ������ �ּҰ� 0.0���� ����)
	vec3 diffuse = lightPower * diffuseLight * lightColor; //��� �ݻ� ���� �� = ��� �ݻ� �� * ���� ���� ��	

	int shininess = 5; //���� ���
	float specularStrength = 1.0f;//���� ����
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect (-lightDir, normalVector);	
	float specularLight =pow( max (dot (viewDir, reflectDir), 0.0f), shininess);
	vec3 specular = specularStrength * specularLight * lightColor;

	//vec3 result = (ambient + diffuse + specular);//���� ���� = (�ֺ� ���� + ��� �ݻ� ���� + �ſ� �ݻ� ����) * ��ü ���� 
    vec4 result = vec4(ambient + diffuse + specular, 1.0f) * objectColor;//���� ���� = (�ֺ� ���� + ��� �ݻ� ���� + �ſ� �ݻ� ����) * ��ü ���� 
	FragColor = texture(outTexture, TexCoord);
	FragColor = vec4 (result) * FragColor;
}
