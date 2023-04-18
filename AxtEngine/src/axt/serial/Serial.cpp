#include <pch.h>

#include "Serial.h"

#include "axt/world/components/all.h"
#include <yaml-cpp/yaml.h>

#define CMP_HEADER(x) out << YAML::Key << x << YAML::Value; out << YAML::BeginMap
#define CMP_FOOTER(...) out << YAML::EndMap

static constexpr const char* gDescriptionCmpKey{ "Description" };
static constexpr const char* gCameraCmpKey{ "Camera" };
static constexpr const char* gTransformCmpKey{ "Transform" };
static constexpr const char* gSpriteCmpKey{ "Sprite" };
static constexpr const char* gHeirarchyCmpKey{ "Heirarchy" };

namespace axt::serial
{
	using namespace necs;

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	static Ref<Scene> gScene{ nullptr };

	static void PackEntity(Entity& i, YAML::Emitter& out)
	{
		out << YAML::BeginMap;

		out << YAML::Key << "Entity" << YAML::Value << "GUID_PLACEHOLDER";

		if (gScene->HasComponent<Description>(i))
		{
			CMP_HEADER(gDescriptionCmpKey);

			Description& d{ gScene->GetComponent<Description>(i) };
			out << YAML::Key << "Name" << YAML::Value << d.Name.c_str();

			CMP_FOOTER();
		}

		if (gScene->HasComponent<Camera>(i))
		{
			CMP_HEADER(gCameraCmpKey);

			Camera& c{ gScene->GetComponent<Camera>(i) };
			out << YAML::Key << "AspectRatio" << YAML::Value << c.AspectRatio;
			out << YAML::Key << "Zoom" << YAML::Value << c.Zoom;

			CMP_FOOTER();
		}

		if (gScene->HasComponent<Transform>(i))
		{
			CMP_HEADER(gTransformCmpKey);

			Transform& t{ gScene->GetComponent<Transform>(i) };
			out << YAML::Key << "Position" << YAML::Value << t.Position;
			out << YAML::Key << "Rotation" << YAML::Value << t.Rotation;

			CMP_FOOTER();
		}

		if (gScene->HasComponent<Sprite>(i))
		{
			CMP_HEADER(gSpriteCmpKey);

			Sprite& s{ gScene->GetComponent<Sprite>(i) };
			out << YAML::Key << "Color" << YAML::Value << s.Color;
			out << YAML::Key << "Size" << YAML::Value << s.Size;

			CMP_FOOTER();
		}

		if (gScene->HasComponent<Heirarchy>(i))
		{
			CMP_HEADER(gHeirarchyCmpKey);

			Heirarchy& h{ gScene->GetComponent<Heirarchy>(i) };
			out << YAML::Key << "Parent" << YAML::Value << h.Parent;
			out << YAML::Key << "Children" << YAML::Value << h.Children;

			// replace children with a loop to get the guids

			CMP_FOOTER();
		}

		out << YAML::EndMap;
	}

	static void UnpackEntity(Entity& i, YAML::Emitter& out)
	{

	}

	bool Pack(const std::string& filepath, Ref<Scene>& scene)
	{
		gScene = scene;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "SCENE_NAME";

		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		for (Entity i : scene->GetIDHandler())
		{
			PackEntity(i, out);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream write{ filepath };
		write << out.c_str();

		gScene = nullptr;
		return true;
	}

	bool Unpack(const std::string& filepath, Ref<Scene>& scene)
	{
		gScene = scene;

		std::ifstream infile{ filepath };
		std::stringstream stream{};

		stream << infile.rdbuf();



		gScene = nullptr;
		return true;
	}
}