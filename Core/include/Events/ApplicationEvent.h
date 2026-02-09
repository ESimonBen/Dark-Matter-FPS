#pragma once
#include "Event.h"
#include <sstream>

namespace Core {
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(std::uint32_t width, std::uint32_t height)
			: m_Width(width), m_Height(height)
		{}

		std::uint32_t GetWidth() const {
			return m_Width;
		}

		std::uint32_t GetHeight() const {
			return m_Height;
		}

		static EventType GetStaticType() {
			return EventType::WindowResize;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "WindowResize";
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryApplication;
		}

	private:
		std::uint32_t m_Width, m_Height;
	};

	class WindowCloseEvent : public Event {
	public:
		static EventType GetStaticType() {
			return EventType::WindowClose;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "WindowClose";
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryApplication;
		}
	};

	class WindowFocusEvent : public Event {
	public:
		static EventType GetStaticType() {
			return EventType::WindowFocus;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "WindowFocus";
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryApplication;
		}
	};

	class WindowLostFocusEvent : public Event {
	public:
		static EventType GetStaticType() {
			return EventType::WindowLostFocus;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "WindowLostFocus";
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryApplication;
		}
	};

	class WindowMoveEvent : public Event {
	public:
		WindowMoveEvent(int x, int y)
			: m_XPos(x), m_YPos(y)
		{}

		int GetWindowX() const {
			return m_XPos;
		}

		int GetWindowY() const {
			return m_YPos;
		}

		static EventType GetStaticType() {
			return EventType::WindowMove;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "WindowMove";
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowMoveEvent: " << m_XPos << ", " << m_YPos;
			return ss.str();
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryApplication;
		}

	private:
		int m_XPos, m_YPos;
	};

	class AppTickEvent : public Event {
	public:
		static EventType GetStaticType() {
			return EventType::AppTick;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "AppTick";
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryApplication;
		}
	};
}