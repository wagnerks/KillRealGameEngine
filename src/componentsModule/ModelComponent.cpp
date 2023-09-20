﻿#include "ModelComponent.h"

#include <algorithm>

#include "assetsModule/modelModule/ModelLoader.h"
#include "ecsModule/EntityManager.h"
#include "assetsModule/modelModule/Model.h"


using namespace Engine::ComponentsModule;

void LODData::setLodLevel(size_t lodLevel) {
	mLodLevel = lodLevel;
}

size_t LODData::getLodLevel() const {
	return mLodLevel;
}

const std::vector<float>& LODData::getLodLevelValues() {
	return mLodLevelValues;
}

void LODData::addLodLevelValue(float lodLevel) {
	mLodLevelValues.emplace_back(lodLevel);
	std::ranges::sort(mLodLevelValues, [](auto& a, auto& b) { return a > b; });
}

float LODData::getCurrentLodValue() const {
	return mCurrentLodValue;
}

void LODData::setCurrentLodValue(float currentLodValue) {
	mCurrentLodValue = currentLodValue;
}


ModelComponent::ModelComponent() {}

void ModelComponent::addMeshData(std::vector<AssetsModule::ModelObj> meshData) {
	mModel = std::move(meshData);
}

const AssetsModule::ModelObj& ModelComponent::getModel() {
	return getModel(mLOD.getLodLevel());
}

AssetsModule::ModelObj& ModelComponent::getModel(size_t LOD) {
	if (mModel.empty()) {
		static AssetsModule::ModelObj empty;
		return empty;
	}

	if (mModel.size() < LOD) {
		return mModel.back();
	}

	return mModel[LOD];
}

const AssetsModule::ModelObj& ModelComponent::getModelLowestDetails() {
	if (mModel.empty()) {
		static AssetsModule::ModelObj empty;
		return empty;
	}

	return mModel.back();
}

void ModelComponent::setModel(std::vector<AssetsModule::ModelObj> data) {
	mModel = std::move(data);
}

void ModelComponent::serialize(Json::Value& data) {
	data["ModelPath"] = mPath;
}

void ModelComponent::deserialize(const Json::Value& data) {
	AssetsModule::Model* model = nullptr;
	if (data.isMember("ModelPath")) {
		mPath = data["ModelPath"].asString();
		model = AssetsModule::ModelLoader::instance()->load(mPath);
	}

	if (model) {
		addMeshData(model->getAllLODs());
	}
}