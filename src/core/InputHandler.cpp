﻿#include "InputHandler.h"

#include <algorithm>

#include "Engine.h"
#include "imgui_internal.h"

namespace SFE::CoreModule {
	void InputProvider::subscribe(InputObserver* observer) {
		const auto it = std::find(mKeyObservers.cbegin(), mKeyObservers.cend(), observer);
		if (it == mKeyObservers.cend()) {
			mKeyObservers.push_back(observer);
		}
	}

	void InputProvider::unsubscribe(InputObserver* observer) {
		erase_if(mKeyObservers, [observer](auto obs) {
			return obs == observer;
		});
	}

	void InputProvider::fireEvent(InputKey key, InputEventType type) const {
		if (ImGui::GetCurrentContext()->IO.WantCaptureKeyboard) {
			return;
		}

		for (const auto observer : mKeyObservers) {
			if (observer->onKeyEvent) {
				observer->onKeyEvent(key, type);
			}
		}
	}

	void InputProvider::fireEvent(Math::DVec2 mousePos, MouseButton key, InputEventType type) const {
		if (ImGui::GetCurrentContext()->IO.WantCaptureMouse) {
			return;
		}

		for (const auto observer : mKeyObservers) {
			if (observer->onMouseBtnEvent) {
				observer->onMouseBtnEvent(mousePos, key, type);
			}
		}
	}

	void InputProvider::fireEvent(Math::DVec2 mousePos, Math::DVec2 mouseOffset) const {
		if (ImGui::GetCurrentContext()->IO.WantCaptureMouse) {
			return;
		}

		for (const auto observer : mKeyObservers) {
			if (observer->onMouseEvent) {
				observer->onMouseEvent(mousePos, mouseOffset);
			}
		}
	}

	void InputProvider::fireEvent(Math::DVec2 scrollOffset) const {
		if (ImGui::GetCurrentContext()->IO.WantCaptureMouse) {
			return;
		}

		for (const auto observer : mKeyObservers) {
			if (observer->onScrollEvent) {
				observer->onScrollEvent(scrollOffset);
			}
		}
	}

	InputObserver::InputObserver(const InputObserver& other) {
		InputProvider::instance()->subscribe(this);
	}

	InputObserver::InputObserver(InputObserver&& other) noexcept {
		InputProvider::instance()->unsubscribe(&other);
		InputProvider::instance()->subscribe(this);
	}

	InputObserver& InputObserver::operator=(const InputObserver& other) {
		if (this == &other)
			return *this;
			
		InputProvider::instance()->subscribe(this);
		return *this;
	}

	InputObserver& InputObserver::operator=(InputObserver&& other) noexcept {
		if (this == &other)
			return *this;

		InputProvider::instance()->unsubscribe(&other);
		InputProvider::instance()->subscribe(this);
		return *this;
	}

	InputObserver::InputObserver() {
		InputProvider::instance()->subscribe(this);
	}

	InputObserver::~InputObserver() {
		InputProvider::instance()->unsubscribe(this);
	}

	void InputHandler::keyCallback(GLFWwindow* /*window*/, int key, int scancode, int action, int mods) {
		InputProvider::instance()->fireEvent(static_cast<InputKey>(key), static_cast<InputEventType>(action));
	}

	void InputHandler::mouseCallback(GLFWwindow* /*window*/, double xPos, double yPos) {
		if (ImGui::GetCurrentContext()->IO.WantCaptureMouse) {
			return;
		}

		InputProvider::instance()->fireEvent({ xPos, yPos}, { xPos - mMousePos.x, mMousePos.y - yPos });
		mMousePos = { xPos, yPos };
	}

	void InputHandler::scrollCallback(GLFWwindow* /*window*/, double xOffset, double yOffset) {
		InputProvider::instance()->fireEvent({ xOffset, yOffset });
	}

	void InputHandler::mouseBtnInput(GLFWwindow* /*window*/, int btn, int act, int mode) {
		InputProvider::instance()->fireEvent(mMousePos, static_cast<MouseButton>(btn), static_cast<InputEventType>(act));
	}

	void InputHandler::init() {
		glfwSetInputMode(Engine::instance()->getMainWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		glfwSetKeyCallback(Engine::instance()->getMainWindow(), &InputHandler::keyCallback);
		glfwSetMouseButtonCallback(Engine::instance()->getMainWindow(), &InputHandler::mouseBtnInput);
		glfwSetCursorPosCallback(Engine::instance()->getMainWindow(), &InputHandler::mouseCallback);
		glfwSetScrollCallback(Engine::instance()->getMainWindow(), &InputHandler::scrollCallback);
	}
}