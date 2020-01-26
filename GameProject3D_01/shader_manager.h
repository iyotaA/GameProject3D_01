#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

class ShaderManager
{
public:

	// �V�F�[�_�[�}�l�[�W���[������
	static void Init();

	// �S�ẴV�F�[�_�[�̏I������
	static void Uninit();

	// �e���v���[�g�N���X�i�ǉ��j
	template <typename T>
	static T* AddShader(const char* VertexShader, const char* PixelShader)
	{
		T* shader = new T();
		shader->Init(VertexShader, PixelShader);
		m_Shaders.push_back(shader);
		return shader;
	}

	// �w��̃V�F�[�_�[���Q�b�g
	template <typename T>
	static T* const GetShader()
	{
		for (CShaderBase* shader : m_Shaders) {

			// typeid()�͌^�𒲂ׂ�
			if (typeid(*shader) == typeid(T)) {

				T* shaderInstance = new T;
				shaderInstance;

				return (T*)shader;
			}
		}
	}

	// �C���X�^���X���擾
	static ShaderManager& GetInstance() {

		ShaderManager instance;
		return instance;
	}


private:
	//struct
	//{
	//	CShaderBase* shader
	//};

	static std::vector<CShaderBase*>	m_Shaders;
	static std::string m_Files;
};

#endif // !SHADER_MANAGER_H_

