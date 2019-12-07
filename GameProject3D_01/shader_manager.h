#ifndef SHADER_MANAGER_H_
#define SHADER_MANAGER_H_

class ShaderManager
{
public:

	// シェーダーマネージャー初期化
	static void Init();

	// 全てのシェーダーの終了処理
	static void Uninit();

	// テンプレートクラス（追加）
	template <typename T>
	static T* AddShader(const char* VertexShader, const char* PixelShader)
	{
		T* shader = new T();
		shader->Init(VertexShader, PixelShader);
		m_Shaders.push_back(shader);
		return shader;
	}

	// 指定のシェーダーをゲット
	template <typename T>
	static T* const GetShader()
	{
		for (ShaderBase* shader : m_Shaders) {

			// typeid()は型を調べる
			if (typeid(*shader) == typeid(T)) {
				return (T*)shader;
			}
		}
	}

	// インスタンスを取得
	static ShaderManager& GetInstance() {

		ShaderManager instance;
		return instance;
	}


private:
	static std::vector<ShaderBase*>	m_Shaders;

};

#endif // !SHADER_MANAGER_H_

