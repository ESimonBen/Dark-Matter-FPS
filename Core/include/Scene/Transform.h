#pragma once
#include "Math/Types.h"

namespace Core {
	class Transform {
	public:
		Vec3 position{ 0.0f };
		Vec3 rotation{ 0.0f };
		Vec3 scale{ 1.0f };

		Mat4 GetMatrix() const;
	};
}