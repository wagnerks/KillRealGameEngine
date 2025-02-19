#pragma once
#include <mutex>
#include <json/value.h>

#include "TypeName.h"
#include "core/ECSHandler.h"
#include "mathModule/Forward.h"

namespace SFE::PropertiesModule {
	class PropertiesSystem {
	public:
		static ecss::EntityId loadScene(std::string_view path);

		static void applyProperties(ecss::EntityId entity, const Json::Value& properties);

		static void fillTree(ecss::EntityId entity, const Json::Value& properties);

		static Json::Value serializeEntity(const ecss::EntityId entity);

		template<class T>
		static void deserializeProperty(const ecss::EntityId entity, const Json::Value& properties);

		template<class T>
		static void serializeProperty(const ecss::EntityId entity, Json::Value& properties);
	private:
	};

	template <class T>
	void PropertiesSystem::deserializeProperty(const ecss::EntityId entity, const Json::Value& properties) {
		if (!ECSHandler::registry().contains(entity)) {
			return;
		}
		auto name = TypeName<T>::name().data();
		if (properties.isMember(name)) {
			ECSHandler::registry().addComponent<T>(entity, entity)->deserialize(properties[name]);
		}
	}

	template <class T>
	void PropertiesSystem::serializeProperty(const ecss::EntityId entity, Json::Value& properties) {
		if (entity == ecss::INVALID_ID) {
			return;
		}

		if (auto component = ECSHandler::registry().getComponent<T>(entity)) {
			component->serialize(properties[TypeName<T>::name().data()]);
		}

	}

	namespace JsonUtils {
		inline const Json::Value* getValue(const Json::Value& json, std::string_view memberId) {
			if (json.isMember(memberId.data())) {
				return &json[memberId.data()];
			}

			return nullptr;
		}

		inline const Json::Value* getValueArray(const Json::Value& json, std::string_view memberId) {
			if (auto val = getValue(json, memberId)) {
				if (val->isArray()) {
					return val;
				}
			}

			return nullptr;
		}

		inline Math::Vec3 getVec3(const Json::Value& json) {
			if (json.isArray()) {
				return { json[0].asFloat(), json[1].asFloat(), json[2].asFloat() };
			}
			return {};
		}

		inline Math::Vec2 getVec2(const Json::Value& json) {
			if (json.isArray()) {
				return { json[0].asFloat(), json[1].asFloat() };
			}
			return {};
		}
	}
}
