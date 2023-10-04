﻿#pragma once

#include "../Types.h"

namespace ECS {
	class SystemInterface {
		friend class SystemManager;
	protected:
		SystemInterface() = default;
		virtual ~SystemInterface() = default;

	public:
		virtual void preUpdate(float dt) = 0;
		virtual void update(float dt) = 0;
		virtual void postUpdate(float dt) = 0;

	private:
		//variables should be set through systemManager
		float  mTimeSinceLastUpdate = 0.f;
		float  mUpdateInterval = 0.f;

		uint16_t mPriority = 0;

		bool	 mEnabled = true;
	};

	template <class T>
	class System : public SystemInterface {
	public:
		inline static const ECSType STATIC_SYSTEM_TYPE_ID = StaticTypeCounter<SystemInterface>::get<T>();

		void preUpdate(float dt) override {}
		void update(float dt) override {}
		void postUpdate(float dt) override {}
	};
}
