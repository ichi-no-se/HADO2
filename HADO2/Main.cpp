﻿#include <Siv3D.hpp>

class Ball {
public:
	Circle circle;
	Color color;
	Vec2 velocity;
	void Draw() {
		circle.draw(color);
	}
	void accelerate(Vec2 a) {
		velocity += a;
	}
	void move() {
		circle.center += velocity;
	}
	void attenuate() {
		velocity *= 0.95;
	}
};

class PlayerBall : public Ball {
public:
	void accelerate() {
		if (circle.center.distanceFrom(Cursor::Pos()) < 10) {
			velocity *= 0.9;
		}
		else {
			Vec2 b;
			b.x = cos(atan2(Cursor::Pos().y - circle.center.y, Cursor::Pos().x - circle.center.x)) * 0.4;
			b.y = sin(atan2(Cursor::Pos().y - circle.center.y, Cursor::Pos().x - circle.center.x)) * 0.4;
			velocity += b;
		}
	}
};

class Hado {
public:
	Vec2 center;
	int radius, power;
	bool hitred, hitblue, hitwhite;
	Color color;
	void draw() {

	}
	void hit() {

	}
};


void init(PlayerBall& Red, Ball& Blue, Ball& White, int& RedHado, int& BlueHado) {
	Red.circle.center = {300, 200};
	Red.velocity = {0, 0};
	Red.circle.r = 10;
	Red.color = Palette::Red;


	RedHado = BlueHado = 0;
}

void Main() {
	Scene::SetBackground(Palette::Black);
	Graphics::SetTargetFrameRateHz(60);
	const Font font(60);
	const Array<Line> FieldTop {
		Line(100, 50, 700, 50), Line(100, 175, 60, 175), Line(700, 175, 740, 175)
	};
	const Array<Line> FieldBottom {
		Line(100, 400, 700, 400), Line(100, 275, 60, 275), Line(700, 275, 740, 275)
	};
	const Array<Line> FieldLeft{
		Line(100, 50, 100, 175), Line(60, 175, 60, 275), Line(100, 275, 100, 400)
	};
	const Array<Line> FieldRight {
		Line(700, 50, 700, 175), Line(740, 175, 740, 275), Line(700, 275, 700, 400)
	};
	Array<Hado> HadoArray;
	PlayerBall Red;
	Ball Blue, White;
	int RedHado, BlueHado;
	init(Red, Blue, White, RedHado, BlueHado);
	while (System::Update()) {
		Red.Draw();
		Circle(Cursor::Pos(), 5).draw(Color(255, 0, 0, 100));
		for (const Line& l : FieldTop) {
			l.draw(3);
			if (l.intersects(Red.circle)) {
				Red.velocity.y = -Red.velocity.y;
				Red.circle.center.y += 1;
			}
		}
		for (const Line& l : FieldBottom) {
			l.draw(3);
			if (l.intersects(Red.circle)) {
				Red.velocity.y = -Red.velocity.y;
				Red.circle.center.y -= 1;
			}
		}
		for (const Line& l : FieldLeft) {
			l.draw(3);
			if (l.intersects(Red.circle)) {
				Red.velocity.x = -Red.velocity.x;
				Red.circle.center.x += 1;
			}
		}
		for (const Line& l : FieldRight) {
			l.draw(3);
			if (l.intersects(Red.circle)) {
				Red.velocity.x = -Red.velocity.x;
				Red.circle.center.x -= 1;
			}
		}
		for (Hado& h : HadoArray) {
			h.draw();
			h.hit();
		}

		if (MouseL.pressed()) {
			RedHado++;
		}
		if (MouseL.up()) {
			RedHado = 0;
			//ゆっくり減るよう あとで調整
		}
		RedHado = Min(RedHado, 250);

		Red.accelerate();
		Red.move();
		Red.attenuate();
		Rect(100, 420, 250, 40).drawFrame(0, 5);
		Rect(100, 420, RedHado, 40).draw(Palette::Red);
	}
}