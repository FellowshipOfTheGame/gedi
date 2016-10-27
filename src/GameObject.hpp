#pragma once

#include "Vec2.hpp"
#include "Connection.hpp"

using ID = uintptr_t;

/// GameObject: o objeto na sua main que tem as infos gerais necessárias, diga-se
// um ID e seu transform
struct GameObject {
	Vec2 position;
	Vec2 rotation;
	const ID id { reinterpret_cast<ID> (this) };
};

BuffNSize getBuffer (const GameObject & x) {
	return getBuffer (x.id);
}
