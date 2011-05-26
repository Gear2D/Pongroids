/**
 * @file bar.cc
 * @b family bar
 * @b type bar
 * 
 * Controls the bar behaviours */

#include <gear2d/gear2d.h>
using namespace gear2d;

class bar : public component::base {
	public:
		virtual component::family family() { return "bar"; }
		virtual component::type type() { return "bar"; }
		virtual std::string depends() { return "collider audiosource"; }
		virtual void handle(parameterbase::id pid, component::base * last, object::id pidowner);
		virtual void setup(object::signature & sig);
		virtual void update(float dt);
};


void bar::handle(parameterbase::id pid, component::base* last, object::id pidowner) {
	if (pid == "collider.collision") {
		component::base * c = read<component::base *>("collider.collision");
		if (c->raw<std::string>("collider.tag") == "pong") write("blip.playing", true);
	}
}

void bar::setup(object::signature & sig) {
	hook("collider.collision");
}

void bar::update(float dt) {
	
}


extern "C" {
	component::base * build() { return new bar(); }
}