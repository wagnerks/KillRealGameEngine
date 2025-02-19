﻿#pragma once
#include "assetsModule/modelModule/Model.h"

namespace SFE::ComponentsModule {
	struct MeshComponent {
		struct MeshData {
			unsigned int vaoId = 0;
			int verticesCount = 0;
			int indicesCount = 0;
		};

		Graph<MeshData> meshGraph;

		AssetsModule::Model* meshModel = nullptr;
	};
}

using SFE::ComponentsModule::MeshComponent;