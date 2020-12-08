
#include "Precompiled.h"
#include <random>
using namespace CK::DD;

// �޽�
const std::size_t GameEngine::CharacterMesh = std::hash<std::string>()("SM_Character");

// ���� ������Ʈ
const std::string GameEngine::PlayerGo("Player");

// �ؽ���
const std::size_t GameEngine::DiffuseTexture = std::hash<std::string>()("Diffuse");
const std::string GameEngine::SteveTexturePath("MyResource.png");

struct GameObjectCompare
{
	bool operator()(const std::unique_ptr<GameObject>& lhs, std::size_t rhs)
	{
		return lhs->GetHash() < rhs;
	}
};

void GameEngine::OnScreenResize(const ScreenPoint& InScreenSize)
{
	// ȭ�� ũ���� ����
	_ScreenSize = InScreenSize;
	_MainCamera.SetViewportSize(_ScreenSize);
}

bool GameEngine::Init()
{
	// �̹� �ʱ�ȭ�Ǿ� ������ �ʱ�ȭ �������� ����.
	if (_IsInitialized)
	{
		return true;
	}

	// ȭ�� ũ�Ⱑ �ùٷ� �����Ǿ� �ִ��� Ȯ��
	if (_ScreenSize.HasZero())
	{
		return false;
	}

	if (!_InputManager.IsInputReady())
	{
		return false;
	}

	if (!LoadResources())
	{
		return false;
	}

	if (!LoadScene())
	{
		return false;
	}

	_IsInitialized = true;
	return true;
}

bool GameEngine::LoadResources()
{
	// �޽� ������ �ε�
	Mesh& characterMesh = CreateMesh(GameEngine::CharacterMesh);

	constexpr float halfSize = 0.5f;
	constexpr int vertexCount = 45;
	constexpr int triangleCount = 15;
	constexpr int indexCount = triangleCount * 3;

	auto& v = characterMesh.GetVertices();
	auto& i = characterMesh.GetIndices();
	auto& uv = characterMesh.GetUVs();
	auto& color = characterMesh.GetColors();

	v = {
		Vector2(-1.f, 10.f) * halfSize,
		Vector2(0.f, 8.f) * halfSize,
		Vector2(1.f, 10.f) * halfSize,
		Vector2(-1.5f, 8.f) * halfSize,
		Vector2(0.f, 4.5f) * halfSize,
		Vector2(1.5f, 8.f) * halfSize,
		Vector2(-3.5f, 8.f) * halfSize,
		Vector2(-2.5f,6.f) * halfSize,
		Vector2(-1.5f, 8.f)* halfSize,
		Vector2(1.5f, 8.f)* halfSize,
		Vector2(2.5f, 6.f)* halfSize,
		Vector2(3.5f, 8.f)* halfSize,
		Vector2(-2.5f, 6.f)* halfSize,
		Vector2(-3.25f, 3.f)* halfSize,
		Vector2(-1.75f, 3.f)* halfSize,
		Vector2(0.f, 4.5f)* halfSize,
		Vector2(-1.5f, 1.f)* halfSize,
		Vector2(1.5f, 1.f)* halfSize,
		Vector2(-3.25f, 3.f)* halfSize,
		Vector2(-2.5f, 0.f)* halfSize,
		Vector2(-1.75f, 3.f)* halfSize,
		Vector2(2.5f, 6.f)* halfSize,
		Vector2(1.75f, 3.f)* halfSize,
		Vector2(3.25f, 3.f)* halfSize,
		Vector2(1.75f, 3.f)*halfSize,
		Vector2(2.5f, 0.f)*halfSize,
		Vector2(3.25f, 3.f)*halfSize,
		Vector2(-1.5f, 1.f)* halfSize,
		Vector2(-2.5f, -4.f)*halfSize,
		Vector2(-0.5f, -4.f)* halfSize,
		Vector2(-2.5f, -4.f)* halfSize,
		Vector2(-1.5f, -9.f)* halfSize,
		Vector2(-0.5f, -4.f)* halfSize,
		Vector2(1.5f, 1.f)* halfSize,
		Vector2(0.5f, -4.f)*halfSize,
		Vector2(2.5f, -4.f)*halfSize,
		Vector2(0.5f, -4.f)*halfSize,
		Vector2(1.5f, -9.f)*halfSize,
		Vector2(2.5f, -4.f)*halfSize,
		Vector2(-1.5f, -9.f)*halfSize,
		Vector2(-1.0f, -10.f)*halfSize,
		Vector2(-2.5f, -10.f)*halfSize,
		Vector2(1.5f, -9.f)*halfSize,
		Vector2(2.5f, -10.f)*halfSize,
		Vector2(1.0f, -10.f)*halfSize,

	};

	uv = {
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
		Vector2(0.125f, 0.75f),
		Vector2(0.125f, 0.875f),
		Vector2(0.25f, 0.875f),
	};

	color = {
		LinearColor::Green,
		LinearColor::Green,
		LinearColor::Green,
		LinearColor::Blue,
		LinearColor::Blue,
		LinearColor::Blue,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Blue,
		LinearColor::Blue,
		LinearColor::Blue,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Gray,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Black,
		LinearColor::Magenta,
		LinearColor::Magenta,
		LinearColor::Magenta,
		LinearColor::Magenta,
		LinearColor::Magenta,
		LinearColor::Magenta,
	};

	i = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14,
		15, 16, 17,
		18, 19, 20,
		21, 22, 23,
		24, 25, 26,
		27, 28, 29,
		30, 31, 32,
		33, 34, 35,
		36, 37, 38,
		39, 40, 41,
		42, 43, 44,
	};


	characterMesh.CalculateBounds();

	// �ؽ��� �ε�
	Texture& diffuseTexture = CreateTexture(GameEngine::DiffuseTexture, GameEngine::SteveTexturePath);
	if (!diffuseTexture.IsIntialized())
	{
		return false;
	}

	return true;
}

bool GameEngine::LoadScene()
{
	static const float playerScale = 30.f;

	// �¾��
	GameObject& goPlayer = CreateNewGameObject(GameEngine::PlayerGo);
	goPlayer.SetMesh(GameEngine::CharacterMesh);
	goPlayer.GetTransform().SetWorldScale(Vector2::One * playerScale);
	goPlayer.GetTransform().SetWorldRotation(0.f);

	return true;
}


Mesh& GameEngine::CreateMesh(const std::size_t& InKey)
{
	auto meshPtr = std::make_unique<Mesh>();
	_Meshes.insert({ InKey, std::move(meshPtr) });
	return *_Meshes.at(InKey).get();
}

Texture& GameEngine::CreateTexture(const std::size_t& InKey, const std::string& InTexturePath)
{
	auto texturePtr = std::make_unique<Texture>(InTexturePath);
	_Textures.insert({ InKey, std::move(texturePtr) });
	return *_Textures.at(InKey).get();
}

GameObject& GameEngine::CreateNewGameObject(const std::string& InName)
{
	std::size_t inHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), inHash, GameObjectCompare());

	auto newGameObject = std::make_unique<GameObject>(InName);
	if (it != _Scene.end())
	{
		std::size_t targetHash = (*it)->GetHash();
		if (targetHash == inHash)
		{
			// �ߺ��� Ű �߻�. ����.
			assert(false);
			return GameObject::Invalid;
		}
		else if (targetHash < inHash)
		{
			_Scene.insert(it + 1, std::move(newGameObject));
		}
		else
		{
			_Scene.insert(it, std::move(newGameObject));
		}
	}
	else
	{
		_Scene.push_back(std::move(newGameObject));
	}

	return GetGameObject(InName);
}

GameObject& GameEngine::GetGameObject(const std::string& InName)
{
	std::size_t targetHash = std::hash<std::string>()(InName);
	const auto it = std::lower_bound(SceneBegin(), SceneEnd(), targetHash, GameObjectCompare());

	return (it != _Scene.end()) ? *(*it).get() : GameObject::Invalid;
}
