#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

class ShaderManager
{
public:
	enum
	{
		SHADER_DEFOULT,
		SHADER_PER_PIXEL,
	};


	// �V�F�[�_�[�}�l�[�W���[������
	static void Init();

	// �S�ẴV�F�[�_�[�̏I������
	static void Uninit()
	{
		for (ShaderBase* shader : m_Shaders) {

			shader->Uninit();
			delete shader;
		}

		// ���X�g�̃N���A
		m_Shaders.clear();
	}

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
		for (ShaderBase* shader : m_Shaders) {

			// typeid()�͌^�𒲂ׂ�
			if (typeid(*shader) == typeid(T)) {
				return (T*)shader;
			}
		}
	}

private:
	static std::vector<ShaderBase*>	m_Shaders;

};

#endif // !SHADER_MANAGER_H_

