#include <pch.h>

#include "Serial.h"

#include "axt/world/components/all.h"
#include <yaml-cpp/yaml.h>

#define CMP_HEADER(x) out << YAML::Key << x << YAML::Value; out << YAML::BeginMap
#define CMP_FOOTER(...) out << YAML::EndMap

inline static constexpr const char* gDescriptionCmpKey{ "Description" };
inline static constexpr const char* gCameraCmpKey{ "Camera" };
inline static constexpr const char* gTransformCmpKey{ "Transform" };
inline static constexpr const char* gSpriteCmpKey{ "Sprite" };
inline static constexpr const char* gGraphCmpKey{ "GraphData" };

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

	static Ref<GameWorld> gWorld{ nullptr };

	static void PackEntity(const Entity& entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;

		GraphData& gdata{ gWorld->GetComponent<GraphData>(entity) };
		out << YAML::Key << "Entity" << YAML::Value << gdata.ID;

		if (gWorld->HasComponent<Description>(entity))
		{
			CMP_HEADER(gDescriptionCmpKey);

			Description& d{ gWorld->GetComponent<Description>(entity) };
			out << YAML::Key << "Name" << YAML::Value << d.Name.c_str();

			CMP_FOOTER();
		}

		if (gWorld->HasComponent<Camera>(entity))
		{
			CMP_HEADER(gCameraCmpKey);

			Camera& c{ gWorld->GetComponent<Camera>(entity) };
			out << YAML::Key << "AspectRatio" << YAML::Value << c.AspectRatio;
			out << YAML::Key << "Zoom" << YAML::Value << c.Zoom;

			CMP_FOOTER();
		}

		if (gWorld->HasComponent<Transform>(entity))
		{
			CMP_HEADER(gTransformCmpKey);

			Transform& t{ gWorld->GetComponent<Transform>(entity) };
			out << YAML::Key << "Position" << YAML::Value << t.Position;
			out << YAML::Key << "Rotation" << YAML::Value << t.Rotation;

			CMP_FOOTER();
		}

		if (gWorld->HasComponent<Sprite>(entity))
		{
			CMP_HEADER(gSpriteCmpKey);

			Sprite& s{ gWorld->GetComponent<Sprite>(entity) };
			out << YAML::Key << "Color" << YAML::Value << s.Color;
			out << YAML::Key << "Size" << YAML::Value << s.Size;

			CMP_FOOTER();
		}

		// ALL ENTITIES HAVE A GRAPHDATA COMPONENT
		CMP_HEADER(gGraphCmpKey);

		out << YAML::Key << "ID" << YAML::Value << gdata.ID;
		out << YAML::Key << "Parent" << YAML::Value << gdata.Parent;
		//out << YAML::Key << "Children" << YAML::Value << gdata.Children;

		if (gdata.Children.size() > 0)
		{
			out << YAML::Key << "Children" << YAML::Value;
			out << YAML::BeginSeq;
			for (axt::UUID& id : gdata.Children)
			{
				PackEntity(gWorld->GetEntityFromUUID(id), out);
			}
			out << YAML::EndSeq;
		}

		CMP_FOOTER();

		out << YAML::EndMap;
	}

	bool Pack(const std::string& filepath, Ref<GameWorld>& world)
	{
		gWorld = world;

		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "SCENE_NAME";

		// World root is ALWAYS the first entity
		GraphData& gdataRoot{ world->GetComponent<GraphData>(world->GetWorldRoot()) };
		out << YAML::Key << "WorldRoot" << YAML::Value << gdataRoot.ID;
		//PackEntity(world->GetWorldRoot(), out);

		out << YAML::Key << "Entities" << YAML::Value;

		out << YAML::BeginSeq;

		GraphData& gdata{ world->GetComponent<GraphData>(world->GetWorldRoot()) };
		for (axt::UUID& id : gdata.Children)
		{
			PackEntity(gWorld->GetEntityFromUUID(id), out);
		}

		// World root is ALWAYS the first entity, so we can move +1 to the next
		/*for (auto it{world->GetScene()->GetIDHandler().begin() + 1}; it < world->GetScene()->GetIDHandler().end(); it++)
		{
			PackEntity(*it, out);
		}*/
		out << YAML::EndSeq;

		out << YAML::EndMap;

		std::ofstream write{ filepath };
		write << out.c_str();

		gWorld = nullptr;
		return true;
	}

	static void UnpackEntity(const Ref<GameWorld>& world, const YAML::Node& info)
	{
		YAML::Node graph{ info[gGraphCmpKey] };

		if (!graph)
		{
			AXT_WARN("No graph data found for entity: {0}", info["Entity"].as<axt::UUID>());
			return;
		}

		axt::UUID parent{ graph["Parent"].as<axt::UUID>() };
		axt::UUID myID{ graph["ID"].as<axt::UUID>() };

		Entity entity{ world->LoadEntity(parent, myID) };

		GraphData& gdata{ world->GetComponent<GraphData>(entity) };

		gdata.Parent = graph["Parent"].as<axt::UUID>();

		YAML::Node children{ graph["Children"] };
		if (children)
		{
			for (YAML::Node childNode : children)
			{
				UnpackEntity(world, childNode);
			}
		}

		YAML::Node desc{ info[gDescriptionCmpKey] };
		if (desc)
		{
			world->Attach<Description>(entity, desc["Name"].as<std::string>());
		}

		YAML::Node spr{ info[gSpriteCmpKey] };
		if (spr)
		{
			Sprite& s{ world->Attach<Sprite>(entity) };
			YAML::Node size{ spr["Size"] };
			s.Size = { size[0].as<float>(), size[1].as<float>() };

			YAML::Node color{ spr["Color"] };
			s.Color = { color[0].as<float>(), color[1].as<float>(), color[2].as<float>(), color[3].as<float>() };
		}

		YAML::Node trans{ info[gTransformCmpKey] };
		if (trans)
		{
			Transform& t{ world->Attach<Transform>(entity) };

			YAML::Node position{ trans["Position"] };
			t.Position = { position[0].as<float>(), position[1].as<float>(), position[2].as<float>() };

			YAML::Node rotation{ trans["Rotation"] };
			t.Rotation = { rotation[0].as<float>(), rotation[1].as<float>(), rotation[2].as<float>() };
		}

		YAML::Node cam{ info[gCameraCmpKey] };
		if (cam)
		{
			Camera& cmp{ world->Attach<Camera>(entity, cam["AspectRatio"].as<float>()) };
			world->SetActiveCamera(entity);
		}

	}

	Ref<GameWorld> Unpack(const std::string& filepath)
	{
		std::ifstream infile{ filepath };
		std::stringstream stream{};

		stream << infile.rdbuf();

		YAML::Node data{ YAML::Load(stream.str()) };
		if (!data["Scene"])
		{
			AXT_WARN("No scene data found!");
			return nullptr;
		}

		YAML::Node rootNode{ data["WorldRoot"] };
		if (!rootNode)
		{
			AXT_WARN("No world root found in scene!");
			return nullptr;
		}

		axt::UUID rootID{ rootNode.as<axt::UUID>() };
		Ref<GameWorld> rWorld{ NewRef<GameWorld>(rootID) };

		YAML::Node entities{ data["Entities"] };
		if (entities)
		{
			for (YAML::Node entity : entities)
			{
				UnpackEntity(rWorld, entity);
			}
		}
		return rWorld;
	}

}