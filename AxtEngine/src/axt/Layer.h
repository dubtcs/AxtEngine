#pragma once

#include <pch.h>
#include "Core.h"
#include "events/Event.h"

namespace axt {

	class AXT_API Layer {
	public:
		Layer(const std::string& newName = "Layer");
		virtual ~Layer();
		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		//virtual void OnEvent(Event& event) {};
		virtual void OnImGuiRender() {};
		const std::string& GetName() const { return name; }
	protected:
		std::string name;
	};

}
