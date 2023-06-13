#include "Particles.h"

// 初始化雪花粒子
void createParticles() {
	particles temp;
	temp.alive = particles_alive;
	temp.life = particles_life;
	temp.fade = float(rand() % particles_fadeMax) / 1000.0f + 0.003f;
	temp.xpos = (float)(rand() % 20) - 10;
	temp.ypos = 8.0;
	temp.zpos = (float)(rand() % 10) + 35;
	temp.red = 0.5;
	temp.green = 0.5;
	temp.blue = 1.0;
	temp.vel = 0.0;
	temp.gravity = particles_gravity;// 重力加速度

	for (int i = 0; i < MAX_PARTICLES; i++) {
		par_sys.push_back(temp);
	}

}

// 为单个粒子重新初始化
void respawnByIndex(int index) {
	par_sys[index].alive = particles_alive;
	par_sys[index].life = particles_life;
	par_sys[index].fade = float(rand() % particles_fadeMax) / 1000.0f + 0.003f;
	par_sys[index].xpos = (float)(rand() % 20) - 10;
	par_sys[index].ypos = 8.0;
	par_sys[index].zpos = (float)(rand() % 10) + 35;
	par_sys[index].red = 0.5;
	par_sys[index].green = 0.5;
	par_sys[index].blue = 1.0;
	par_sys[index].vel = 0.0;
	par_sys[index].gravity = particles_gravity;// 重力加速度
}

// // 为所有粒子设置存活状态
// void setAlive(bool alive) {
// 	for (int i = 0; i < MAX_PARTICLES; i++) {
// 		par_sys[i].alive = alive;
// 	}
// }

// // 为所有粒子设置寿命
// void setLife(float life) {
// 	for (int i = 0; i < MAX_PARTICLES; i++) {
// 		par_sys[i].life = life;
// 	}
// }

// // 为所有粒子设置衰减速度
// void setFade(int fadeMAX) {
// 	for (int i = 0; i < MAX_PARTICLES; i++) {
// 		par_sys[i].fade = float(rand() % fadeMAX) / 1000.0f + 0.003f;
// 	}
// }

// // 为所有粒子设置重力
// void setGravity(float gravity) {
// 	for (int i = 0; i < MAX_PARTICLES; i++) {
// 		par_sys[i].gravity = gravity;
// 	}
// }


void updateParticles() {
	std::vector <particles>().swap(par_sys);
	createParticles();
	// setAlive(alive);
	// setLife(life);
	// setFade(fadeMAX);
	// setGravity(gravity);
}