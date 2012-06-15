#include <gear2d/gear2d.h>
#include <cstdlib>
#include <cmath>


using namespace gear2d;

/**
 * @file asteroid.cc
 * Component to handle asteroids. Asteroids breaks when kicked
 * and spawns other asteroids
 *
 * @b parameters
 * @li @c asteroid.type Size of this asteroid (big, medium, small). @b string
 * 	When hit, one big asteroid will split into medium. One medium into
 *  small, and small will be destroyed.
 * @li @c asteroid.fragmentation How many smaller asteroids this asteroid should be split @b int
 */

enum asteroidtype {
	big,
	medium,
	small,
	none
};

namespace gear2d {
		template<> asteroidtype eval<asteroidtype>(std::string t, asteroidtype def) {
			if (t == "big") return big;
			if (t == "medium") return medium;
			if (t == "small") return small;
			return def;
	}
}

class asteroid : public component::base {
	private:
		asteroidtype t;
		int frags;
		bool spawning;
		float timeleft;
		
		static int asteroidno;
		
		gear2d::link<float> x, y, w, h;
		
	private:
		void explode() {
			std::string target;
			if (t == big) target = "asteroid16";
			else if (t == medium) target = "asteroid8";
			else return;
			
			const int & side = raw<int>("collider.collision.side");
			
			object::id obj;
			asteroid * com;
			const float & xspeed = raw<float>("x.speed");
			const float & yspeed = raw<float>("y.speed");
			float increment = 360.0f/frags;
			
			for (int i = 0; i < frags; i++) {
				if (asteroidno >= 60) break;
				obj = spawn(target);
				com = (asteroid *) obj->component("asteroid");
				if (com == 0) continue;

				// setup x and y
				com->write("x", read<float>("x"));
				com->write("y", read<float>("y"));
				
				// left or right
				com->write("y.speed", sinf((i*increment)*M_PI/180.0f)*10.0f + rand() % 20);
				com->write("x.speed", cosf((i*increment)*M_PI/180.0f)*10.0f + rand() % 20);
			}
			
			if (t == big) {
				object::id otherbig = spawn(owner->name());
				write("explode.playing", true);
// 				otherbig->component("asteroid")->write<float>("x", -31);
// 				otherbig->component("asteroid")->write<float>("y.speed", 10.0f);
			}
		}
		
	public:
		asteroid() : t(none), spawning(false) { }
		virtual component::family family() { return "asteroid"; }
		virtual component::type type() { return "asteroid"; }
		virtual std::string depends() { return "spatial/space2d collider/collider2d dynamics/rigidbody2d kinematics/kinematic2d renderer/renderer"; }
		
		void collide(parameterbase::id pid, component::base * lastwrite, object::id pidowner) {
			component::base * other = read<component::base *>("collider.collision");
			if (lastwrite == this) return;
			explode();
			destroy();
		}
		
		virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id pidowner) {
// 			if (pid == "collider.collision") {
// 				component::base * other = read<component::base *>("collider.collision");
// 				if (lastwrite == this) return;
// 				explode();
// 				destroy();
// 			}
		}
		
		virtual void setup(object::signature & sig) {
			bind("asteroid.type", t);
			bind("asteroid.fragmentation", frags);
			t = eval(sig["asteroid.type"], small);
			frags = eval(sig["asteroid.fragmentation"], 2);
			hook("collider.collision", (component::call)&asteroid::collide);
			x = fetch<float>("x");
			y = fetch<float>("y");
			w = fetch<float>("w");
			h = fetch<float>("h");
			x = rand() % read<int>("renderer.w"); 
			y = rand() % read<int>("renderer.h");
// 			write<float>("x", w*1.0f);
// 			write<float>("y", h*1.0f);
			if (t == big) {
				write("roid.alpha", 0.0f);
				spawning = true;
				write("x.speed", (rand() % 2) == 0 ? -10.0f : 10.0f);
				write("y.speed", (rand() % 2) == 0 ? -10.0f : 10.0f);
			}
			if (t == small) {
				timeleft = 5.0f;
			}
			asteroidno++;
		}
		
		virtual void update(float dt) {
			if (x + w < 0) x = raw<int>("renderer.w") - raw<float>("w");
			else if (x > raw<int>("renderer.w")) x = 0.0f;
			if (y + h < 0) y = raw<int>("renderer.h") - raw<float>("h");
			else if (y > raw<int>("renderer.h")) y = 0.0f;
// 			cout << "xis: " << x << endl;
			if (t == big) {
				if (spawning) {
					float alpha = read<float>("roid.alpha");
					alpha += dt;
					clamp(alpha, 0.0f, 1.0f);
					if (alpha >= 1.0f) spawning = false;
					write("roid.alpha", alpha);
				}
			}
			if (t == small) {
				timeleft -= dt;
				clamp(timeleft, 0.0f, 5.0f);
				write("roid.alpha", timeleft / 5.0f);
				if (timeleft <= 0.0f) destroy();
			}
		}
		
		virtual ~asteroid() {
			asteroidno--;
		}
};

int asteroid::asteroidno = 0;

extern "C" {
	component::base * build() { return new asteroid; }
}