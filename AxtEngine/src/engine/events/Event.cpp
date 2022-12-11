
#include "Event.h"

namespace axt {

	void EventHandler::Fire(Event& ev) {
		ev.isHandled = true;
	}

}
