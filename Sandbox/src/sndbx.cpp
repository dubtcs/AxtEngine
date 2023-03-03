
// CLIENT

#include "sndbx.h"
#include <axt/EntryPoint.h>

#include "layers/Sand2D.h"
#include "layers/Sand3D.h"
#include "layers/Sandpaper.h"

// ENTRY POINT
axt::App* axt::CreateApp() {
	return new Sandbox{};
}

// APP
Sandbox::Sandbox() {
	AXT_TRACE("DWJWDJWAJ");
	PushLayer(new Sand2D{});
}
