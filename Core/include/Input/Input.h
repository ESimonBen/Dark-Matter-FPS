#pragma once
#include "Math/Types.h"

namespace Core {
	enum class Key {
		W, A, S, D,
		Space, LeftShift,
		Escape
	};

	class Input {
	public:
		static void SetKey(Key key, bool pressed);
		static bool IsKeyDown(Key key);
		static bool IsKeyPressed(Key key);

		static void SetMouseDelta(float dx, float dy);
		static Vec2 GetMouseDelta();
		static void EndFrame();
	};
}