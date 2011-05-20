#include <gear2d/gear2d.h>

#include <iostream>
using namespace std;
using namespace gear2d;


class pong : public component::base {
	public:
		virtual component::family family() { return "pong"; }
		virtual component::type type() { return "type"; }
		virtual string depends() { return "spatial kinetics dynamics"; }
		virtual void setup(object::signature & sig) {
			hook("collider.collision");
		}
		
		virtual void handle(parameterbase::id pid, component::base * lastwrite) {
			if (pid == "collider.collision") {
				component::base * c = read<component::base*>(pid);
				cout << c->read<string>("collider.tag") << endl;
				if (c->read<string>("collider.tag") == "bar") {
					cout << read<float>("collider.collision.speed.x") << endl;
					add("x.speed", 2.0f);
					add("y.speed", read<float>("collider.collision.speed.y")*0.5f);
				}
			}
		}
		
		virtual void update(float dt) {
			write("grid.position.x", raw<float>("x") / 10.0f - 640);
			write("grid.position.y", raw<float>("y") / 10.0f - 480);
// 			add("x.speed",  read<float>("x.speed") * dt/50);
		}
		
};

extern "C" {
	component::base * build() { return new pong; }
}