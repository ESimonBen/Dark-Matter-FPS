#pragma once
#include "Event.h"
#include <sstream>

namespace Core {
	class MouseMoveEvent : public Event {
	public:
		MouseMoveEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y)
		{}

		float GetX() const {
			return m_MouseX;
		}

		float GetY() const {
			return m_MouseY;
		}

		static EventType GetStaticType() {
			return EventType::MouseMove;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "MouseMove";
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryMouse | EventCategoryInput;
		}

	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrollEvent : public Event {
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{}

		float GetXOffset() const {
			return m_XOffset;
		}

		float GetYOffset() const {
			return m_YOffset;
		}

		static EventType GetStaticType() {
			return EventType::MouseScroll;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "MouseScroll";
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseScrollEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryMouse | EventCategoryInput;
		}

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event {
	public:
		int GetMouseButton() const {
			return m_Button;
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput;
		}

	protected:
		MouseButtonEvent(int button)
			: m_Button(button)
		{}

		int m_Button;
	};

	class MouseButtonPressEvent : public MouseButtonEvent {
	public:
		MouseButtonPressEvent(int button)
			: MouseButtonEvent(button)
		{}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonPressEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() {
			return EventType::MouseButtonPress;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "MouseButtonPress";
		}
	};

	class MouseButtonReleaseEvent : public MouseButtonEvent {
	public:
		MouseButtonReleaseEvent(int button)
			: MouseButtonEvent(button)
		{
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseButtonReleaseEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() {
			return EventType::MouseButtonRelease;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "MouseButtonRelease";
		}
	};
}