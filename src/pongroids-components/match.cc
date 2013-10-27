/**
 * @file match.cc
 * @b family match
 * @b type match
 * @author Leonardo Guilherme de Freitas
 * 
 * Controls aspects of the match like score, asteroids on screen,
 * and other code */

#include "gear2d.h"
using namespace gear2d;

#include <cstdio>

class match : public component::base {
	private:
		class player {
			public:
				int score;
				player()
				: score(0)
				{
					
				}
		};
	private:
		component::base * leftwall;
		component::base * rightwall;
		component::base * leftfield;
		component::base * rightfield;
		component::base * pong;
		component::base * background;
		player p1;
		player p2;
		
	private:
		void placepong(float x = 316.0f, float y = 256.0f) {
			pong->write("x", x);
			pong->write("y", y);
			pong->write("x.speed", 1.0f);
			pong->write("y.speed", 1.0f);
		}
	public:
		match() 
		: leftwall(0)
		, rightwall(0)
		, leftfield(0)
		, rightfield(0)
		, pong(0)
		, background(0)
		{
			
		}
		virtual component::family family() { return "match"; }
		virtual component::type type() { return "match"; }
		virtual void handle(parameterbase::id pid, component::base * last, object::id pidowner) {
			if (pid != "collider.collision") return;
			char score[5];
			if (pidowner == leftwall->owner) {
				p2.score++;
				sprintf(score, "%d", p2.score);
				write("score2.text", std::string(score));
				add("score2.zoom", 0.1f);
				placepong(140, 236);
			} else if (pidowner == rightwall->owner) {
				p1.score++;
				sprintf(score, "%d", p1.score);
				write("score1.text", std::string(score));
				add("score1.zoom", 0.1f);
				placepong(500, 236);
			}
		}
		virtual void setup(object::signature & sig) {
			object::id oid = 0;
			oid = spawn("left");
			leftwall = oid->component("spatial");
			hook(leftwall, "collider.collision");
			
			oid = spawn("right");
			rightwall = oid->component("spatial");
			hook(rightwall, "collider.collision");
			
			oid = spawn("pong");
			pong = oid->component("spatial");
			placepong(140, 236);
			
			oid = locate("background");
			background = oid->component("spatial");
			
			spawn("asteroid32");
		}
		
		virtual void update(float dt) {
			if (background->read<int>("background.clip.h") + background->read<float>("y") - 480 <= 0) {
				background->write<float>("y.speed", 0);
			}
		}
};

g2dcomponent(match, match, match);
