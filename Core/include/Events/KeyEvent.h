#pragma once
#include "Event.h"
#include <sstream>

namespace Core {
	class KeyEvent : public Event {
	public:
		int GetKeyCode() const {
			return m_KeyCode;
		}

		int GetScanCode() const {
			return m_ScanCode;
		}

		virtual int GetCategoryFlags() const override {
			return EventCategoryKeyboard | EventCategoryInput;
		}

	protected:
		KeyEvent(int keycode, int scancode)
			: m_KeyCode(keycode), m_ScanCode(scancode)
		{}

		int m_KeyCode;
		int m_ScanCode;
	};

	class KeyPressEvent : public KeyEvent {
	public:
		KeyPressEvent(int keycode, int scancode, bool repeat)
			: KeyEvent(keycode, scancode), m_Repeat(repeat)
		{}

		int GetRepeat() const {
			return m_Repeat;
		}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressEvent: " << m_KeyCode << " (Repeated: " << (m_Repeat ? "True" : "False") << ")";
			return ss.str();
		}

		static EventType GetStaticType() {
			return EventType::KeyPress;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "KeyPress";
		}

	private:
		bool m_Repeat;
	};

	class KeyReleaseEvent : public KeyEvent {
	public:
		KeyReleaseEvent(int keycode, int scancode)
			: KeyEvent(keycode, scancode)
		{}

		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_KeyCode;
			return ss.str();
		}

		static EventType GetStaticType() {
			return EventType::KeyRelease;
		}

		virtual EventType GetEventType() const override {
			return GetStaticType();
		}

		virtual const char* GetName() const override {
			return "KeyRelease";
		}
	};
}